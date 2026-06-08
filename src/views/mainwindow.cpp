/* src/views/mainwindow.cpp */
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QInputDialog>
#include "../controllers/system_engine.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 初始化业务控制器
    m_engine = new SystemEngine(this);

    // 配置联动信号
    initUIStyleAndConnections();

    // 首次加载全看板静态/动态数据
    refreshDishMenuUI();
    refreshQueueUI();
    loadHistoryOrders();
}

void MainWindow::initUIStyleAndConnections()
{
    // 模型层数据更新回传信号绑定
    connect(m_engine, &SystemEngine::queueStatusChanged, this, &MainWindow::refreshQueueUI);
    connect(m_engine, &SystemEngine::menuDataChanged, this, &MainWindow::refreshDishMenuUI);

    // 配置菜单表格元数据
    ui->tableWidgetMenu->setColumnCount(5);
    ui->tableWidgetMenu->setHorizontalHeaderLabels({"菜品名称", "分类", "基础原价", "附加说明", "状态标记"});
    ui->tableWidgetMenu->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidgetMenu->setSelectionMode(QAbstractItemView::SingleSelection);
    
    // 配置排队进程表格元数据
    ui->tableWidgetQueue->setColumnCount(3);
    ui->tableWidgetQueue->setHorizontalHeaderLabels({"排队位置", "客户流水/手机号", "入队时间"});
    ui->tableWidgetQueue->setSelectionBehavior(QAbstractItemView::SelectRows);

    // 预注入评价排序下拉框映射
    ui->comboCommentSort->addItem("🔥 按评分由高到低", 0);
    ui->comboCommentSort->addItem("⏰ 按留言时间先后", 1);
}

// 导航栏联动
void MainWindow::on_sidebarNav_currentRowChanged(int row)
{
    if (row >= 0) {
        ui->stackedWidget->setCurrentIndex(row);
    }
}

// 队列分类Tab切换触发刷新
void MainWindow::on_tabWidgetQueueType_currentChanged(int index)
{
    Q_UNUSED(index);
    refreshQueueUI();
}

// ==========================================
// 功能一 & 功能四：菜单呈现与爆款高亮
// ==========================================
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
            attrItem->setText("🔥 爆款推荐 (Top 5)");
            attrItem->setForeground(QBrush(QColor("#D32F2F"))); 
            attrItem->setFont(QFont("Microsoft YaHei", 9, QFont::Bold));
        } else {
            attrItem->setText("常规菜品");
            attrItem->setForeground(QBrush(QColor("#757575")));
        }
        ui->tableWidgetMenu->setItem(row, 4, attrItem);
    }
}

// ==========================================
// 功能二：多级会员阶梯计算与点单持久化
// ==========================================
void MainWindow::on_btnCalculatePrice_clicked()
{
    int selectedRow = ui->tableWidgetMenu->currentRow();
    if (selectedRow < 0) {
        QMessageBox::warning(this, "预计算校验失败", "请先在左侧菜单网格中选择您要购买的菜品！");
        return;
    }

    QString dishName = ui->tableWidgetMenu->item(selectedRow, 0)->text();
    QString memberId = ui->lineEditMemberId->text().trimmed();

    double finalPrice = m_engine->calculatePrice(dishName, memberId);
    ui->lblFinalPrice->setText(QString("实际消费价格: ￥%1").arg(finalPrice, 0, 'f', 2));
}

void MainWindow::on_btnSubmitOrder_clicked()
{
    int selectedRow = ui->tableWidgetMenu->currentRow();
    if (selectedRow < 0) {
        QMessageBox::warning(this, "支付中断", "未选定购买目标，无法提交订单。");
        return;
    }

    QString dishName = ui->tableWidgetMenu->item(selectedRow, 0)->text();
    QString memberId = ui->lineEditMemberId->text().trimmed();

    double finalPrice = m_engine->calculatePrice(dishName, memberId);
    bool success = m_engine->createOrder(dishName, memberId);
    
    if (success) {
        QString msg = QString("点单成功！\n实付金额：￥%1\n系统已自动为您累积积分 +%2 并在后台更新会员等级。")
                      .arg(finalPrice, 0, 'f', 2)
                      .arg(static_cast<int>(finalPrice));
        QMessageBox::information(this, "账单结清", msg);
        loadHistoryOrders(); 
    }
}

// ==========================================
// 功能三：条件多级检索排序评价显示
// ==========================================
void MainWindow::on_comboCommentSort_currentIndexChanged(int index)
{
    if (index < 0) return;
    ui->listWidgetComments->clear();
    
    QList<CommentModel> sortedComments = m_engine->getSortedComments(index);

    for (const auto &comment : sortedComments) {
        QString stars = QString("★").repeated(comment.score) + QString("☆").repeated(5 - comment.score);
        QString itemText = QString("[%1] 评分:%2星\n💬 评语: %3\n⏱ 评价时间: %4\n-----------------------------------------")
                            .arg(stars)
                            .arg(comment.score)
                            .arg(comment.content)
                            .arg(comment.time.toString("yyyy-MM-dd hh:mm:ss"));
        ui->listWidgetComments->addItem(itemText);
    }
}

// ==========================================
// 功能五：基于历史购买记录逆向解包克隆订单
// ==========================================
void MainWindow::loadHistoryOrders()
{
    ui->listWidgetHistory->clear();
    QList<OrderModel> history = m_engine->getHistoryOrders();
    for (const auto &order : history) {
        ui->listWidgetHistory->addItem(QString("订单号: %1 | 菜品: %2").arg(order.orderId).arg(order.dishName));
    }
}

void MainWindow::on_copyHistoryOrder_clicked()
{
    QListWidgetItem *selectedItem = ui->listWidgetHistory->currentItem();
    if (!selectedItem) {
        QMessageBox::warning(this, "复制中止", "请先在上方列表中选中一项历史订单轨迹！");
        return;
    }

    bool success = m_engine->duplicateOrderFromHistory(selectedItem->text());
    if (success) {
        QMessageBox::information(this, "逆向克隆成功", "已通过历史记忆直接派生出一条完全一致的新消费账单！");
        loadHistoryOrders();
    }
}

// ==========================================
// 功能六：排队动态消解看板仿真
// ==========================================
void MainWindow::refreshQueueUI()
{
    ui->tableWidgetQueue->setRowCount(0);
    int currentQueueType = ui->tabWidgetQueueType->currentIndex();
    
    // 调用控制器。底层模型的 getQueue(type) 逻辑保证非首次调用时前排自动出队两名模拟用户
    QList<QueueNode> queueData = m_engine->getQueueData(currentQueueType);

    for (int i = 0; i < queueData.size(); ++i) {
        int row = ui->tableWidgetQueue->rowCount();
        ui->tableWidgetQueue->insertRow(row);

        QTableWidgetItem *posItem = new QTableWidgetItem(QString("第 %1 位").arg(i + 1));
        if (i < 2) {
            posItem->setBackground(QBrush(QColor("#E8F5E9"))); // 临近办理高亮
            posItem->setForeground(QBrush(QColor("#2E7D32")));
        }
        ui->tableWidgetQueue->setItem(row, 0, posItem);
        ui->tableWidgetQueue->setItem(row, 1, new QTableWidgetItem(queueData[i].id));
        ui->tableWidgetQueue->setItem(row, 2, new QTableWidgetItem(queueData[i].joinTime.toString("hh:mm:ss")));
    }
    
    ui->lblQueueCount->setText(QString("当前队列剩余总等待数: %1 人").arg(queueData.size()));
}

// 手动点击刷新按钮，驱动底层模型消费 2 人
void MainWindow::on_btnRefreshQueue_clicked()
{
    refreshQueueUI(); 
}

void MainWindow::on_btnJoinQueue_clicked()
{
    bool ok;
    QString id = QInputDialog::getText(this, "自助排队申请", 
                                       "请输入您的手机号或外卖单号作为唯一识别标识:", 
                                       QLineEdit::Normal, "", &ok);
    if (!ok || id.trimmed().isEmpty()) return;

    int currentQueueType = ui->tabWidgetQueueType->currentIndex();
    m_engine->customerJoinQueue(id.trimmed(), currentQueueType);
}

void MainWindow::on_btnCallNext_clicked()
{
    int currentQueueType = ui->tabWidgetQueueType->currentIndex();
    m_engine->callNextCustomer(currentQueueType);
}

MainWindow::~MainWindow() {
    delete ui;
}