/* src/views/mainwindow.cpp */
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
//初始化列表。首先调用基类构造函数；接着在堆内存中实例化
    : QMainWindow(parent)
    //将其指针赋给私有变量 ui。
    , ui(new Ui::MainWindow)
{
    //将当前 MainWindow 实例的指针传递给 UI 组合对象，在内部完成所有控件的创建、层级挂载和基础几何布局的初始化。
    ui->setupUi(this);
    //在堆上创建控制器实例
    m_engine = new SystemEngine(this);

    //调用私有初始化辅助函数，配置表格样式并手动绑定非标准信号。
    initUIStyleAndConnections();

    // 首次引导加载
    //从控制器中拉取初始的菜单数据、队列状态以及历史消费记录
    refreshDishMenuUI();
    refreshQueueUI();
    loadHistoryOrders();
    refreshCommentsUI();
}

void MainWindow::initUIStyleAndConnections()
{
    //将控制器的队列突变信号（queueStatusChanged）绑定到本窗体的刷新槽（refreshQueueUI）。
    //当后台排队人数由于仿真叫号或新客入队发生改变时，UI 会被动动响应刷新。
    connect(m_engine, &SystemEngine::queueStatusChanged, this, &MainWindow::refreshQueueUI);
    //同理，当后台菜单数据变动时（menuDataChanged），自动重新渲染电子菜单。
    connect(m_engine, &SystemEngine::menuDataChanged, this, &MainWindow::refreshDishMenuUI);
    //新增：当后台评价数据变动时（commentDataChanged），自动重新渲染评价看板。
    connect(m_engine, &SystemEngine::commentDataChanged, this, &MainWindow::refreshCommentsUI);

    // 配置主电子菜单
    //为菜单网格显式开辟 5 列空间
    ui->tableWidgetMenu->setColumnCount(5);
    //传入一个 QStringList 临时初始化列表，顺序定义表头文本。
    ui->tableWidgetMenu->setHorizontalHeaderLabels({"菜品名称", "分类", "标准原价", "描述", "标记"});
    //将默认的“单元格选中”升级为“整行选中”
    ui->tableWidgetMenu->setSelectionBehavior(QAbstractItemView::SelectRows);
    //设定为单选模式，禁止用户通过 Ctrl 或拖拽同时选中多行菜品，确保单次加购的目标确定性。
    ui->tableWidgetMenu->setSelectionMode(QAbstractItemView::SingleSelection);

    // 配置购物车网格
    ui->tableWidgetCart->setColumnCount(3);
    ui->tableWidgetCart->setHorizontalHeaderLabels({"品名", "数量", "单品小计(折后)"});
    ui->tableWidgetCart->setSelectionBehavior(QAbstractItemView::SelectRows);

    // 配置队列网格
    ui->tableWidgetQueue->setColumnCount(3);
    ui->tableWidgetQueue->setHorizontalHeaderLabels({"等待顺位", "凭证识别号", "入队时间"});
    ui->tableWidgetQueue->setSelectionBehavior(QAbstractItemView::SelectRows);

    //向排序下拉复选框追加可选项
    ui->comboCommentSort->addItem("🔥 按评分由高到低", 0);
    ui->comboCommentSort->addItem("⏰ 按留言时间先后", 1);

    //向评分选择下拉框追加 1~5 星可选项（用于用户提交新评价时选择分数）
    ui->comboRatingScore->addItem("⭐⭐⭐⭐⭐ 5分 非常满意", 5);
    ui->comboRatingScore->addItem("⭐⭐⭐⭐ 4分 满意", 4);
    ui->comboRatingScore->addItem("⭐⭐⭐ 3分 一般", 3);
    ui->comboRatingScore->addItem("⭐⭐ 2分 不满意", 2);
    ui->comboRatingScore->addItem("⭐ 1分 非常不满意", 1);
}

//当左侧侧边栏导航列表（sidebarNav）的当前行发生变化时，该函数被触发。
void MainWindow::on_sidebarNav_currentRowChanged(int row)
{
    //建立起侧边栏索引与右侧层叠多页容器（QStackedWidget）的空间绝对映射。
    //点击第 0 行就切到第 0 页（点餐厅），点击第 2 行就切到第 2 页（排队看板），实现了单窗口多页面的流畅路由切换
    if (row >= 0) ui->stackedWidget->setCurrentIndex(row);
}

void MainWindow::on_tabWidgetQueueType_currentChanged(int index)
{
    Q_UNUSED(index);
    //调用 refreshQueueUI()，根据当前选中的新 Tab 页签类型去重新拉取对应队列的数据。
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

//按钮点击下单
void MainWindow::on_btnAddToCart_clicked()
{
    int row = ui->tableWidgetMenu->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "添加失败", "请先在菜单中点击选中一款菜品。");
        return;
    }
    QString dishName = ui->tableWidgetMenu->item(row, 0)->text();
    m_cart[dishName] = m_cart.value(dishName, 0) + 1;
    //实时计算价格
    recalculateCartPrices();
}

//菜单双击下单
void MainWindow::on_tableWidgetMenu_cellDoubleClicked(int row, int column)
{
    Q_UNUSED(column);
    QString dishName = ui->tableWidgetMenu->item(row, 0)->text();
    m_cart[dishName] = m_cart.value(dishName, 0) + 1;
    //实时计算价格
    recalculateCartPrices();
}

//购物车双击擦除商品
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

    // 直接向控制层索要统一格式的会员状态数据
    MemberInfoModel member = m_engine->getMemberInfo(mid);

    // 根据业务状态进行纯粹的界面渲染
    if (mid.isEmpty()) {
        ui->lblMemberLevel->setText("当前身份: 散客顾客 (无优惠)");
        ui->lblMemberLevel->setStyleSheet("color: #E65100; font-weight: bold;");
    } else {
        // 动态展示真实的等级、精确的分位折扣和积分状态
        QString statusText = QString("验证成功：%1 (享有 %2 折优惠 | 当前积分: %3)")
                             .arg(member.levelName)
                             .arg(member.discount * 10, 0, 'f', 1) // 转换为例如 "8.5" 折
                             .arg(member.points);
        ui->lblMemberLevel->setText(statusText);

        // 动态样式驱动：根据具体的等级渲染对应的视觉主题
        if (member.levelName == "钻石VIP会员") {
            ui->lblMemberLevel->setStyleSheet("color: #7B1FA2; font-weight: bold;"); // 尊贵紫
        } else if (member.levelName == "黄金会员") {
            ui->lblMemberLevel->setStyleSheet("color: #F57C00; font-weight: bold;"); // 黄金橙
        } else {
            ui->lblMemberLevel->setStyleSheet("color: #1976D2; font-weight: bold;"); // 注册蓝
        }
    }

    // 强制驱动购物车级联更新（用户输入卡号时，购物车内的“折后单品小计”及“总应付金额”将实时无感刷新）
    recalculateCartPrices(); 
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

//点击付款并结账，触发该函数
void MainWindow::on_btnSubmitOrder_clicked()
{
    if (m_cart.isEmpty()) {
        QMessageBox::warning(this, "提交中止", "您的购物车空空如也，无法进行结账付款！");
        return;
    }

    QString mid = ui->lineEditMemberId->text().trimmed();
    QString queueIdentity = mid.isEmpty() ? QString("客流水-%1").arg(qrand() % 900 + 100) : mid;

    // 直接一键把整个购物车输入底层，封装生成单ID捆绑订单
    m_engine->createGroupedOrder(m_cart, mid);

    // 自动化连锁机制：触发自动排队业务
    int defaultQueueTab = 1; // 默认加入“现场取餐进程”
    m_engine->customerJoinQueue(queueIdentity, defaultQueueTab);

    // 需求3联动：下单后立即引导评论环节。
    // OrderManager 内部以自增ID顺序 append，故刚生成的订单必然位于历史列表末尾。
    QList<OrderModel> latestHistory = m_engine->getHistoryOrders();
    QString newOrderId = latestHistory.isEmpty() ? "" : latestHistory.last().orderId;

    QMessageBox::StandardButton choice = QMessageBox::information(
        this, "结账并完成排队",
        QString("付款交易成功！\n系统检测到您的出餐类型，已为您分配至【现场取餐】队列。\n识别号：%1\n\n订单号：%2\n是否现在就对本次消费进行评价？")
            .arg(queueIdentity).arg(newOrderId),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

    // 清洗本地购物车缓冲区并强制更新
    m_cart.clear();
    recalculateCartPrices();
    loadHistoryOrders();

    if (choice == QMessageBox::Yes && !newOrderId.isEmpty()) {
        // 用户选择立即评价：锁定订单号，直接跳转到“历史与评价”页面的评价输入区
        m_pendingRateOrderId = newOrderId;
        ui->comboRatingScore->setCurrentIndex(0);
        ui->textEditCommentContent->clear();
        ui->sidebarNav->setCurrentRow(1);      // 切换左侧导航至：📜 历史与评价
        ui->stackedWidget->setCurrentIndex(1);
        ui->textEditCommentContent->setFocus();
    } else {
        // 用户选择稍后评价：维持原有逻辑，路由切票至排队看板
        ui->tabWidgetQueueType->setCurrentIndex(defaultQueueTab);
        ui->sidebarNav->setCurrentRow(2); // 选中“排队看板”页面
    }
}

// =========================================================================
// 其他衍生模块（评价排序检索、订单克隆、队列消解）
// =========================================================================
void MainWindow::on_comboCommentSort_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    // 排序方式发生切换时，统一交由 refreshCommentsUI() 重新拉取并渲染
    refreshCommentsUI();
}

//统一的评价看板渲染函数：既供排序下拉框切换时调用，也供 commentDataChanged 信号自动触发，
//确保“用户刚提交完评价”与“切换排序方式”两条路径渲染的是同一套最新数据，杜绝界面不同步
void MainWindow::refreshCommentsUI()
{
    int sortIndex = ui->comboCommentSort->currentData().isValid()
                         ? ui->comboCommentSort->currentData().toInt()
                         : ui->comboCommentSort->currentIndex();
    if (sortIndex < 0) sortIndex = 0;

    ui->listWidgetComments->clear();
    QList<CommentModel> sortedComments = m_engine->getSortedComments(sortIndex);
    for (const auto &comment : sortedComments) {
        QString stars = QString("★").repeated(comment.score) + QString("☆").repeated(5 - comment.score);
        // 若评价携带了 targetTag（即用户自主提交的真实订单评价），额外标注来源，便于与系统演示数据区分
        QString tagSuffix = comment.targetTag.isEmpty() ? "" : QString(" [订单:%1]").arg(comment.targetTag);
        ui->listWidgetComments->addItem(QString("[%1] %2%3\n⏱ %4")
                                         .arg(stars)
                                         .arg(comment.content)
                                         .arg(tagSuffix)
                                         .arg(comment.time.toString("yyyy-MM-dd hh:mm")));
    }
}

void MainWindow::loadHistoryOrders()
{
    ui->listWidgetHistory->clear();
    QList<OrderModel> history = m_engine->getHistoryOrders();
    for (const auto &order : history) {
        // 订单号加聚合的内容列表
        ui->listWidgetHistory->addItem(QString("【订单号:%1】 🧾 包含明细: %2")
                                       .arg(order.orderId)
                                       .arg(order.summary));
    }
}

void MainWindow::on_copyHistoryOrder_clicked()
{
    int currentRow = ui->listWidgetHistory->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "复制失败", "请先在左侧历史消费网格中点击选择一条打包订单记录。");
        return;
    }

    QList<OrderModel> history = m_engine->getHistoryOrders();
    if (currentRow >= history.size()) return;

    // 获取选中的那组历史大单
    const OrderModel &selectedOrder = history[currentRow];

    for (const auto &item : selectedOrder.items) {
        m_cart[item.dishName] = m_cart.value(item.dishName, 0) + item.count;
    }

    // 强制驱动UI刷新，购物车列表重排、计算折后优惠价和总和
    recalculateCartPrices();

    QMessageBox::information(this, "智能轨迹复原", 
        QString("成功克隆订单 [%1]！\n所含菜品已全部一键同步回滚至您的专属购物车中。\n系统已为您无感切回点餐大厅！")
        .arg(selectedOrder.orderId));

    //自动跳入菜单界面，方便用户继续加菜或直接结账
    ui->sidebarNav->setCurrentRow(0);     // 切换左侧导航至：🛒 点餐大厅
    ui->stackedWidget->setCurrentIndex(0); // 联动的右侧页面切回点餐主画布
}

// =========================================================================
// 需求3：下单后主动评分评论
// =========================================================================

//用户在历史订单列表中点击“⭐ 评价该订单”，锁定本次评价的目标订单号，
//并将焦点引导至评价输入区，提示用户填写评分与正文
void MainWindow::on_btnRateHistoryOrder_clicked()
{
    int currentRow = ui->listWidgetHistory->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "评价失败", "请先在左侧历史消费网格中点击选择一条想要评价的订单记录。");
        return;
    }

    QList<OrderModel> history = m_engine->getHistoryOrders();
    if (currentRow >= history.size()) return;

    const OrderModel &selectedOrder = history[currentRow];
    // 锁定本次待评价的订单号，供下方“提交评价”按钮在实际写入时使用
    m_pendingRateOrderId = selectedOrder.orderId;

    // 默认回填一个 5 分好评，降低用户操作成本；用户仍可自行调整评分与文案
    ui->comboRatingScore->setCurrentIndex(0);
    ui->textEditCommentContent->clear();
    ui->textEditCommentContent->setFocus();

    QMessageBox::information(this, "选定评价对象",
        QString("已锁定订单【%1】作为本次评价对象。\n请在右侧评价区填写评分与内容后点击“✅ 提交评价”。")
        .arg(selectedOrder.orderId));
}

//用户填好评分与评论正文后点击“✅ 提交评价”，将评价真正写入底层 CommentManager
void MainWindow::on_btnSubmitComment_clicked()
{
    if (m_pendingRateOrderId.isEmpty()) {
        QMessageBox::warning(this, "提交失败", "请先在左侧历史订单中点击“⭐ 评价该订单”以选定评价对象。");
        return;
    }

    QString content = ui->textEditCommentContent->toPlainText().trimmed();
    if (content.isEmpty()) {
        QMessageBox::warning(this, "提交失败", "评价内容不能为空，请填写您的真实感受后再提交。");
        return;
    }

    int score = ui->comboRatingScore->currentData().isValid()
                    ? ui->comboRatingScore->currentData().toInt()
                    : 5;
    QString mid = ui->lineEditMemberId->text().trimmed(); // 散客可为空，与全局会员识别机制保持一致

    // 转发至控制层，最终落地到 CommentManager 的动态评价列表
    bool ok = m_engine->addComment(m_pendingRateOrderId, mid, score, content);

    if (ok) {
        QMessageBox::information(this, "评价成功",
            QString("感谢您对订单【%1】的评价！您的反馈已成功录入评价看板。").arg(m_pendingRateOrderId));
        // 评价完成后清空锁定态与输入框，避免误触导致重复提交同一订单
        m_pendingRateOrderId.clear();
        ui->textEditCommentContent->clear();
        // 自动跳转到“历史与评价”页签的评价看板，让用户立即看到自己刚提交的评价
        ui->sidebarNav->setCurrentRow(1);
        ui->stackedWidget->setCurrentIndex(1);
    } else {
        QMessageBox::warning(this, "提交失败", "评价提交未成功，请稍后重试。");
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
    // 调用引擎的商家出餐接口，直接消减“现场取餐”队列
    m_engine->serveDish();
}

void MainWindow::on_btnCallNext_clicked() { m_engine->callNextCustomer(ui->tabWidgetQueueType->currentIndex()); }

MainWindow::~MainWindow() { delete ui; }