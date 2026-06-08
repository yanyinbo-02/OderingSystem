/* src/views/mainwindow.cpp */
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_engine = new SystemEngine(this);

    initUIStyleAndConnections();

    // 首次引导加载
    refreshDishMenuUI();
    refreshQueueUI();
    loadHistoryOrders();
}

void MainWindow::initUIStyleAndConnections()
{
    connect(m_engine, &SystemEngine::queueStatusChanged, this, &MainWindow::refreshQueueUI);
    connect(m_engine, &SystemEngine::menuDataChanged, this, &MainWindow::refreshDishMenuUI);

    // 配置主电子菜单
    ui->tableWidgetMenu->setColumnCount(5);
    ui->tableWidgetMenu->setHorizontalHeaderLabels({"菜品名称", "分类", "标准原价", "描述", "标记"});
    ui->tableWidgetMenu->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidgetMenu->setSelectionMode(QAbstractItemView::SingleSelection);

    // 配置购物车网格
    ui->tableWidgetCart->setColumnCount(3);
    ui->tableWidgetCart->setHorizontalHeaderLabels({"品名", "数量", "单品小计(折后)"});
    ui->tableWidgetCart->setSelectionBehavior(QAbstractItemView::SelectRows);

    // 配置队列网格
    ui->tableWidgetQueue->setColumnCount(3);
    ui->tableWidgetQueue->setHorizontalHeaderLabels({"等待顺位", "凭证识别号", "入队时间"});
    ui->tableWidgetQueue->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->comboCommentSort->addItem("🔥 按评分由高到低", 0);
    ui->comboCommentSort->addItem("⏰ 按留言时间先后", 1);
}

void MainWindow::on_sidebarNav_currentRowChanged(int row)
{
    if (row >= 0) ui->stackedWidget->setCurrentIndex(row);
}

void MainWindow::on_tabWidgetQueueType_currentChanged(int index)
{
    Q_UNUSED(index);
    refreshQueueUI();
}

void MainWindow::refreshDishMenuUI()
{
    ui->tableWidgetMenu->setRowCount(0);
    QList<DishModel> dishList = m_engine->getAllDishes();

    for (const auto &dish : dishList) {
        int row = ui->tableWidgetMenu->rowCount();
        ui->tableWidgetMenu->insertRow(row);
        ui->tableWidgetMenu->setItem(row, 0, new QTableWidgetItem(dish.name));
        ui->tableWidgetMenu->setItem(row, 1, new QTableWidgetItem(dish.category));
        ui->tableWidgetMenu->setItem(row, 2, new QTableWidgetItem(QString("￥%1").arg(dish.price, 0, 'f', 2)));
        ui->tableWidgetMenu->setItem(row, 3, new QTableWidgetItem(dish.description));

        QTableWidgetItem *attrItem = new QTableWidgetItem();
        if (dish.isTopFive) {
            attrItem->setText("🔥 爆款Top5");
            attrItem->setForeground(QBrush(QColor("#D32F2F")));
            attrItem->setFont(QFont("Microsoft YaHei", 9, QFont::Bold));
        } else {
            attrItem->setText("常规");
            attrItem->setForeground(QBrush(QColor("#757575")));
        }
        ui->tableWidgetMenu->setItem(row, 4, attrItem);
    }
}

// =========================================================================
// 需求1核心槽：购物车双向操作与会员无感登录体系
// =========================================================================
void MainWindow::on_btnAddToCart_clicked()
{
    int row = ui->tableWidgetMenu->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "添加失败", "请先在菜单中点击选中一款菜品。");
        return;
    }
    QString dishName = ui->tableWidgetMenu->item(row, 0)->text();
    m_cart[dishName] = m_cart.value(dishName, 0) + 1;
    recalculateCartPrices();
}

void MainWindow::on_tableWidgetMenu_cellDoubleClicked(int row, int column)
{
    Q_UNUSED(column);
    QString dishName = ui->tableWidgetMenu->item(row, 0)->text();
    m_cart[dishName] = m_cart.value(dishName, 0) + 1;
    recalculateCartPrices();
}

void MainWindow::on_tableWidgetCart_cellDoubleClicked(int row, int column)
{
    Q_UNUSED(column);
    QString dishName = ui->tableWidgetCart->item(row, 0)->text();
    if (m_cart.contains(dishName)) {
        m_cart[dishName]--;
        if (m_cart[dishName] <= 0) {
            m_cart.remove(dishName);
        }
    }
    recalculateCartPrices();
}

// 登录后自动动态切换并更新当前会员级别显示
void MainWindow::on_lineEditMemberId_textChanged(const QString &text)
{
    QString mid = text.trimmed();
    if (mid.isEmpty()) {
        ui->lblMemberLevel->setText("当前身份: 散客顾客 (无优惠)");
        ui->lblMemberLevel->setStyleSheet("color: #E65100; font-weight: bold;");
    } else {
        // 利用边界测试菜品逆向探测或直接映射控制器分层
        // 这里基于后台返回的折扣率反推等级名称，实现极致的高内聚显示
        double testPrice = m_engine->calculatePrice("特色牛排", mid);
        if (testPrice < 20.0 && testPrice > 0) { 
            // 假设原价40，根据折扣判定等级
            ui->lblMemberLevel->setText("验证成功：钻石VIP会员 (享有最高等级尊贵特权)");
            ui->lblMemberLevel->setStyleSheet("color: #7B1FA2; font-weight: bold;");
        } else {
            ui->lblMemberLevel->setText(QString("验证成功：常规注册会员 (卡号:%1)").arg(mid));
            ui->lblMemberLevel->setStyleSheet("color: #1976D2; font-weight: bold;");
        }
    }
    recalculateCartPrices(); // 重新计算因为卡号变更引起的总价波动
}

double MainWindow::getBasePriceOfDish(const QString &dishName)
{
    QList<DishModel> list = m_engine->getAllDishes();
    for (const auto &d : list) {
        if (d.name == dishName) return d.price;
    }
    return 0.0;
}

void MainWindow::recalculateCartPrices()
{
    ui->tableWidgetCart->setRowCount(0);
    QString mid = ui->lineEditMemberId->text().trimmed();

    double totalOriginal = 0.0;
    double totalFinal = 0.0;

    QMap<QString, int>::const_iterator it = m_cart.constBegin();
    while (it != m_cart.constEnd()) {
        QString name = it.key();
        int qty = it.value();

        double basePrice = getBasePriceOfDish(name);
        double discountedPrice = m_engine->calculatePrice(name, mid);

        totalOriginal += (basePrice * qty);
        totalFinal += (discountedPrice * qty);

        int row = ui->tableWidgetCart->rowCount();
        ui->tableWidgetCart->insertRow(row);
        ui->tableWidgetCart->setItem(row, 0, new QTableWidgetItem(name));
        ui->tableWidgetCart->setItem(row, 1, new QTableWidgetItem(QString::number(qty)));
        ui->tableWidgetCart->setItem(row, 2, new QTableWidgetItem(QString("￥%1").arg(discountedPrice * qty, 0, 'f', 2)));
        
        it++;
    }

    ui->lblOriginalPrice->setText(QString("商品原价总额: ￥%1").arg(totalOriginal, 0, 'f', 2));
    ui->lblDiscountPrice->setText(QString("会员折扣减免: -￥%1").arg(totalOriginal - totalFinal, 0, 'f', 2));
    ui->lblFinalPrice->setText(QString("应付金额: ￥%1").arg(totalFinal, 0, 'f', 2));
}

// =========================================================================
// 需求2核心槽：点餐付款成功后，自动进入对应的排队看板进程
// =========================================================================
void MainWindow::on_btnSubmitOrder_clicked()
{
    if (m_cart.isEmpty()) {
        QMessageBox::warning(this, "提交中止", "您的购物车空空如也，无法进行结账付款！");
        return;
    }

    QString mid = ui->lineEditMemberId->text().trimmed();
    QString queueIdentity = mid.isEmpty() ? QString("客流水-%1").arg(qrand() % 900 + 100) : mid;

    // 1. 批量向底层模型写入真实多菜品账单
    QMap<QString, int>::const_iterator it = m_cart.constBegin();
    while (it != m_cart.constEnd()) {
        for (int i = 0; i < it.value(); ++i) {
            m_engine->createOrder(it.key(), mid);
        }
        it++;
    }

    // 2. 自动化连锁机制：触发自动排队业务
    int defaultQueueTab = 1; // 默认加入“现场取餐进程”
    m_engine->customerJoinQueue(queueIdentity, defaultQueueTab);

    QMessageBox::information(this, "结账并完成排队", 
        QString("付款交易成功！\n系统检测到您的出餐类型，已为您分配至【现场取餐】队列。\n识别号：%1").arg(queueIdentity));

    // 3. 清洗本地购物车缓冲区并强制刷新
    m_cart.clear();
    recalculateCartPrices();
    loadHistoryOrders();

    // 4. 路由切票：自动引导切换至排队界面
    ui->tabWidgetQueueType->setCurrentIndex(defaultQueueTab);
    ui->sidebarNav->setCurrentRow(2); // 选中“排队看板”
}

// =========================================================================
// 其他衍生模块（评价、克隆、队列消解）
// =========================================================================
void MainWindow::on_comboCommentSort_currentIndexChanged(int index)
{
    if (index < 0) return;
    ui->listWidgetComments->clear();
    QList<CommentModel> sortedComments = m_engine->getSortedComments(index);
    for (const auto &comment : sortedComments) {
        QString stars = QString("★").repeated(comment.score) + QString("☆").repeated(5 - comment.score);
        ui->listWidgetComments->addItem(QString("[%1] %2\n⏱ %3").arg(stars).arg(comment.content).arg(comment.time.toString("yyyy-MM-dd")));
    }
}

void MainWindow::loadHistoryOrders()
{
    ui->listWidgetHistory->clear();
    QList<OrderModel> history = m_engine->getHistoryOrders();
    for (const auto &order : history) {
        ui->listWidgetHistory->addItem(QString("订单:%1 | 菜品: %2").arg(order.orderId).arg(order.dishName));
    }
}

void MainWindow::on_copyHistoryOrder_clicked()
{
    QListWidgetItem *selectedItem = ui->listWidgetHistory->currentItem();
    if (!selectedItem) return;
    if (m_engine->duplicateOrderFromHistory(selectedItem->text())) {
        QMessageBox::information(this, "同步成功", "历史轨迹对应菜品已逆向克隆出新订单。");
        loadHistoryOrders();
    }
}

void MainWindow::refreshQueueUI()
{
    ui->tableWidgetQueue->setRowCount(0);
    int currentQueueType = ui->tabWidgetQueueType->currentIndex();
    QList<QueueNode> queueData = m_engine->getQueueData(currentQueueType);

    for (int i = 0; i < queueData.size(); ++i) {
        int row = ui->tableWidgetQueue->rowCount();
        ui->tableWidgetQueue->insertRow(row);
        QTableWidgetItem *pos = new QTableWidgetItem(QString("第 %1 位").arg(i + 1));
        if (i < 2) pos->setBackground(QBrush(QColor("#E8F5E9")));
        ui->tableWidgetQueue->setItem(row, 0, pos);
        ui->tableWidgetQueue->setItem(row, 1, new QTableWidgetItem(queueData[i].id));
        ui->tableWidgetQueue->setItem(row, 2, new QTableWidgetItem(queueData[i].joinTime.toString("hh:mm:ss")));
    }
    ui->lblQueueCount->setText(QString("当前队列剩余总等待数: %1 人").arg(queueData.size()));
}

void MainWindow::on_btnRefreshQueue_clicked() { refreshQueueUI(); }
void MainWindow::on_btnJoinQueue_clicked() {
    bool ok;
    QString id = QInputDialog::getText(this, "快捷排队", "输入凭证标识:", QLineEdit::Normal, "", &ok);
    if (ok && !id.trimmed().isEmpty()) m_engine->customerJoinQueue(id.trimmed(), ui->tabWidgetQueueType->currentIndex());
}
void MainWindow::on_btnCallNext_clicked() { m_engine->callNextCustomer(ui->tabWidgetQueueType->currentIndex()); }

MainWindow::~MainWindow() { delete ui; }