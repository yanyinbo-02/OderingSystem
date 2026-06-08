#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QInputDialog>
#include "../controllers/system_engine.h"

//主界面

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // 将当前实例的指针 this（即当前的逻辑窗口 MainWindow）作为参数传进去。
    // setupUi 内部会执行 new QPushButton(this)、new QVBoxLayout(this) 等操作，
    // 真正地把可视化设计器里拖拽的所有控件创建出来，并“画”在当前这个 MainWindow 窗口上。
    ui->setupUi(this);

    // 初始化核心控制器指针
    m_engine = new SystemEngine(this);

    // 初始化界面配置并绑定联动信号
    initUIStyleAndConnections();

    // 首次加载数据
    refreshDishMenuUI();
    refreshQueueUI();
    loadHistoryOrders();
}

void MainWindow::initUIStyleAndConnections()
{
    // 【核心联动机制】当控制器层通知模型更新时，视图层自动触发对应刷新函数
    connect(m_engine, &SystemEngine::queueStatusChanged, this, &MainWindow::refreshQueueUI);
    connect(m_engine, &SystemEngine::menuDataChanged, this, &MainWindow::refreshDishMenuUI);

    // 设置一些表格的基本样式
    ui->tableWidgetMenu->setColumnCount(5);
    ui->tableWidgetMenu->setHorizontalHeaderLabels({"菜品名称", "分类", "原价", "附加说明", "属性标记"});
    
    ui->tableWidgetQueue->setColumnCount(3);
    ui->tableWidgetQueue->setHorizontalHeaderLabels({"排队序号", "客户标识", "进入时间"});
}

// ==========================================
// 功能一 & 功能四：菜单显示与推荐标记呈现
// ==========================================
void MainWindow::refreshDishMenuUI()
{
    ui->tableWidgetMenu->setRowCount(0);
    
    // 从控制器获取全部菜品列表（控制器内部已处理完 Top 5 爆款推荐算法）
    QList<DishModel> dishList = m_engine->getAllDishes();

    for (const auto &dish : dishList) {
        int row = ui->tableWidgetMenu->rowCount();
        ui->tableWidgetMenu->insertRow(row);

        ui->tableWidgetMenu->setItem(row, 0, new QTableWidgetItem(dish.name));
        ui->tableWidgetMenu->setItem(row, 1, new QTableWidgetItem(dish.category));
        ui->tableWidgetMenu->setItem(row, 2, new QTableWidgetItem(QString::number(dish.price, 'f', 2)));
        ui->tableWidgetMenu->setItem(row, 3, new QTableWidgetItem(dish.description));

        // 功能四核心：销量和评价最好的5个菜品，其排序属性会被额外显示在菜单属性中
        QTableWidgetItem *attrItem = new QTableWidgetItem();
        if (dish.isTopFive) {
            attrItem->setText("🔥 爆款推荐 (Top 5)");
            attrItem->setForeground(Qt::red); // 高亮显示
        } else {
            attrItem->setText("常规");
        }
        ui->tableWidgetMenu->setItem(row, 4, attrItem);
    }
}

// ==========================================
// 功能二：根据选择的菜品与会员等级计算消费价格
// ==========================================
void MainWindow::on_btnCalculatePrice_clicked()
{
    int selectedRow = ui->tableWidgetMenu->currentRow();
    if (selectedRow < 0) {
        QMessageBox::warning(this, "提示", "请先在菜单表格中选择一项菜品！");
        return;
    }

    QString dishName = ui->tableWidgetMenu->item(selectedRow, 0)->text();
    QString memberId = ui->lineEditMemberId->text().trimmed();

    // 调用控制器计算实际消费价格（考虑会员级别、折扣、积分累积逻辑）
    double finalPrice = m_engine->calculatePrice(dishName, memberId);
    
    // 显示到界面上
    ui->lblFinalPrice->setText(QString("实际消费价格: ￥%1").arg(finalPrice, 0, 'f', 2));
}

void MainWindow::on_btnSubmitOrder_clicked()
{
    int selectedRow = ui->tableWidgetMenu->currentRow();
    if (selectedRow < 0) return;

    QString dishName = ui->tableWidgetMenu->item(selectedRow, 0)->text();
    QString memberId = ui->lineEditMemberId->text().trimmed();

    // 提交订单，控制器内部会把实际消费记入会员等级积分累积
    bool success = m_engine->createOrder(dishName, memberId);
    if (success) {
        QMessageBox::information(this, "成功", "点单成功，积分已累积！");
        loadHistoryOrders(); // 刷新点餐历史列表
    }
}

// ==========================================
// 功能三：点餐评价及其先后排序显示
// ==========================================
void MainWindow::on_comboCommentSort_currentIndexChanged(int index)
{
    ui->listWidgetComments->clear();
    
    // index 代表排序规则：0-按评分从高到低，1-按留言时间先后
    QList<CommentModel> sortedComments = m_engine->getSortedComments(index);

    for (const auto &comment : sortedComments) {
        QString itemText = QString("[%1星] %2 \n(留言时间: %3)")
                            .arg(comment.score)
                            .arg(comment.content)
                            .arg(comment.time.toString("yyyy-MM-dd hh:mm"));
        ui->listWidgetComments->addItem(itemText);
    }
}

// ==========================================
// 功能五：点餐记忆，根据购买记录快速复制创建订单
// ==========================================
void MainWindow::loadHistoryOrders()
{
    ui->listWidgetHistory->clear();
    // 获取历史购买记录
    QList<OrderModel> history = m_engine->getHistoryOrders();
    for (const auto &order : history) {
        ui->listWidgetHistory->addItem(QString("订单号: %1 | 菜品: %2")
                                       .arg(order.orderId).arg(order.dishName));
    }
}

void MainWindow::on_copyHistoryOrder_clicked()
{
    QListWidgetItem *selectedItem = ui->listWidgetHistory->currentItem();
    if (!selectedItem) {
        QMessageBox::warning(this, "提示", "请选择一条历史记录进行复制！");
        return;
    }

    // 提取选中的历史订单文本
    QString itemText = selectedItem->text(); 
    
    // 调用控制器克隆并快速创建新订单逻辑
    bool success = m_engine->duplicateOrderFromHistory(itemText);
    if (success) {
        QMessageBox::information(this, "快捷点餐", "已成功通过历史记录快速复制并创建新订单！");
        loadHistoryOrders();
    }
}

// ==========================================
// 功能六：排队显示，刷新后实时显示进度
// ==========================================
void MainWindow::refreshQueueUI()
{
    ui->tableWidgetQueue->setRowCount(0);

    // 依据当前的 Tab 选择展示“预约排队”还是“取餐排队” (假设 0:预约, 1:取餐)
    int currentQueueType = ui->tabWidgetQueueType->currentIndex();
    
    // 从控制器拿取实时队列模型数据
    QList<QueueNode> queueData = m_engine->getQueueData(currentQueueType);

    for (int i = 0; i < queueData.size(); ++i) {
        int row = ui->tableWidgetQueue->rowCount();
        ui->tableWidgetQueue->insertRow(row);

        // 渲染进度信息
        ui->tableWidgetQueue->setItem(row, 0, new QTableWidgetItem(QString("第 %1 位").arg(i + 1)));
        ui->tableWidgetQueue->setItem(row, 1, new QTableWidgetItem(queueData[i].id));
        ui->tableWidgetQueue->setItem(row, 2, new QTableWidgetItem(queueData[i].joinTime.toString("hh:mm:ss")));
    }
    
    // 界面上方同步更新当前总排队人数
    ui->lblQueueCount->setText(QString("当前排队总数: %1 人").arg(queueData.size()));
}

void MainWindow::on_btnJoinQueue_clicked()
{
    QString id = QInputDialog::getText(this, "加入队列", "请输入您的手机号/订单号:");
    if (id.isEmpty()) return;

    int currentQueueType = ui->tabWidgetQueueType->currentIndex();
    
    // 界面向控制器发送入队指令
    m_engine->customerJoinQueue(id, currentQueueType);
    // 注意：此处不需要手动调 refreshQueueUI()，因为 initUIStyleAndConnections 中绑定的信号会自动触发它
}

void MainWindow::on_btnCallNext_clicked()
{
    int currentQueueType = ui->tabWidgetQueueType->currentIndex();
    // 商家叫号出队
    m_engine->callNextCustomer(currentQueueType);
}

MainWindow::~MainWindow() {
    delete ui;
}