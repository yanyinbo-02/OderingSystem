/* src/views/ui_mainwindow.h */
#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *mainLayout;
    
    // 导航栏
    QListWidget *sidebarNav;
    QStackedWidget *stackedWidget;

    // 页面1：点餐大厅（升级为购物车联动模式）
    QWidget *pageMenu;
    QHBoxLayout *menuPageLayout;
    QGroupBox *groupMenuTable;
    QVBoxLayout *menuTableLayout;
    QTableWidget *tableWidgetMenu;
    QPushButton *btnAddToCart;
    
    QGroupBox *groupBilling;
    QVBoxLayout *billingLayout;
    QLineEdit *lineEditMemberId;
    QLabel *lblMemberLevel; // 自动化显示当前会员等级
    QTableWidget *tableWidgetCart; // 购物车列表
    QLabel *lblOriginalPrice; // 原价总额
    QLabel *lblDiscountPrice; // 优惠金额
    QLabel *lblFinalPrice; // 实付应付
    QPushButton *btnSubmitOrder; // 付款并自动排队

    // 页面2：历史订单与评价
    QWidget *pageHistory;
    QHBoxLayout *historyPageLayout;
    QSplitter *historySplitter;
    QGroupBox *groupHistory;
    QVBoxLayout *historyLayout;
    QListWidget *listWidgetHistory;
    QPushButton *copyHistoryOrder;
    QGroupBox *groupComments;
    QVBoxLayout *commentsLayout;
    QComboBox *comboCommentSort;
    QListWidget *listWidgetComments;

    // 页面3：排队看板
    QWidget *pageQueue;
    QVBoxLayout *queuePageLayout;
    QHBoxLayout *queueTopLayout;
    QLabel *lblQueueCount;
    QPushButton *btnRefreshQueue;
    QTabWidget *tabWidgetQueueType;
    QWidget *tabReserve;
    QWidget *tabPickup;
    QTableWidget *tableWidgetQueue;
    QHBoxLayout *queueBottomLayout;
    QPushButton *btnJoinQueue;
    QPushButton *btnCallNext;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1100, 700);
        
        centralwidget = new QWidget(MainWindow);
        mainLayout = new QHBoxLayout(centralwidget);
        mainLayout->setSpacing(10);
        mainLayout->setContentsMargins(10, 10, 10, 10);

        // --- 左侧导航栏 ---
        sidebarNav = new QListWidget(centralwidget);
        sidebarNav->setObjectName(QString::fromUtf8("sidebarNav"));
        sidebarNav->setMaximumWidth(180);
        sidebarNav->addItem("🛒 点餐大厅");
        sidebarNav->addItem("📜 历史与评价");
        sidebarNav->addItem("👥 排队看板");
        mainLayout->addWidget(sidebarNav);

        // --- 右侧多页容器 ---
        stackedWidget = new QStackedWidget(centralwidget);
        
        // ================= 页面1：点餐大厅 (全新重构) =================
        pageMenu = new QWidget();
        menuPageLayout = new QHBoxLayout(pageMenu);
        
        // 左侧菜品区
        groupMenuTable = new QGroupBox("电子菜单", pageMenu);
        menuTableLayout = new QVBoxLayout(groupMenuTable);
        tableWidgetMenu = new QTableWidget(groupMenuTable);
        tableWidgetMenu->setObjectName(QString::fromUtf8("tableWidgetMenu"));
        tableWidgetMenu->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        menuTableLayout->addWidget(tableWidgetMenu);
        
        btnAddToCart = new QPushButton("➕ 将选中菜品加入购物车", groupMenuTable);
        btnAddToCart->setObjectName(QString::fromUtf8("btnAddToCart"));
        btnAddToCart->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold; height: 30px;");
        menuTableLayout->addWidget(btnAddToCart);
        menuPageLayout->addWidget(groupMenuTable, 4);

        // 右侧购物车与结算中心
        groupBilling = new QGroupBox("专属购物车 & 结算中心", pageMenu);
        billingLayout = new QVBoxLayout(groupBilling);
        billingLayout->setSpacing(10);
        
        billingLayout->addWidget(new QLabel("会员卡号 (输入后自动识别):"));
        lineEditMemberId = new QLineEdit(groupBilling);
        lineEditMemberId->setObjectName(QString::fromUtf8("lineEditMemberId"));
        lineEditMemberId->setPlaceholderText("请输入卡号进行无感登录");
        billingLayout->addWidget(lineEditMemberId);

        lblMemberLevel = new QLabel("当前身份: 散客顾客 (无优惠)", groupBilling);
        lblMemberLevel->setStyleSheet("color: #E65100; font-weight: bold;");
        billingLayout->addWidget(lblMemberLevel);

        billingLayout->addWidget(new QLabel("购物车明细 (双击某行可移出):"));
        tableWidgetCart = new QTableWidget(groupBilling);
        tableWidgetCart->setObjectName(QString::fromUtf8("tableWidgetCart"));
        tableWidgetCart->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        billingLayout->addWidget(tableWidgetCart);

        // 价格计算拆解明细
        lblOriginalPrice = new QLabel("商品原价总额: ￥0.00", groupBilling);
        billingLayout->addWidget(lblOriginalPrice);

        lblDiscountPrice = new QLabel("会员折扣减免: -￥0.00", groupBilling);
        lblDiscountPrice->setStyleSheet("color: #D32F2F;");
        billingLayout->addWidget(lblDiscountPrice);

        lblFinalPrice = new QLabel("应付金额: ￥0.00", groupBilling);
        lblFinalPrice->setStyleSheet("font-weight: bold; font-size: 16px; color: #2E7D32;");
        billingLayout->addWidget(lblFinalPrice);

        btnSubmitOrder = new QPushButton("💳 确认付款 (自动排队)", groupBilling);
        btnSubmitOrder->setObjectName(QString::fromUtf8("btnSubmitOrder"));
        btnSubmitOrder->setStyleSheet("background-color: #1976D2; color: white; font-weight: bold; height: 40px; font-size: 14px;");
        billingLayout->addWidget(btnSubmitOrder);
        
        menuPageLayout->addWidget(groupBilling, 3);
        stackedWidget->addWidget(pageMenu);

        // ================= 页面2：历史与评价 =================
        pageHistory = new QWidget();
        historyPageLayout = new QHBoxLayout(pageHistory);
        historySplitter = new QSplitter(Qt::Horizontal, pageHistory);

        groupHistory = new QGroupBox("点餐记忆 (历史消费)");
        historyLayout = new QVBoxLayout(groupHistory);
        listWidgetHistory = new QListWidget(groupHistory);
        listWidgetHistory->setObjectName(QString::fromUtf8("listWidgetHistory"));
        historyLayout->addWidget(listWidgetHistory);
        copyHistoryOrder = new QPushButton("🔄 复制该订单再来一单", groupHistory);
        copyHistoryOrder->setObjectName(QString::fromUtf8("copyHistoryOrder"));
        historyLayout->addWidget(copyHistoryOrder);
        historySplitter->addWidget(groupHistory);

        groupComments = new QGroupBox("菜品评价看板");
        commentsLayout = new QVBoxLayout(groupComments);
        comboCommentSort = new QComboBox(groupComments);
        comboCommentSort->setObjectName(QString::fromUtf8("comboCommentSort"));
        commentsLayout->addWidget(comboCommentSort);
        listWidgetComments = new QListWidget(groupComments);
        listWidgetComments->setObjectName(QString::fromUtf8("listWidgetComments"));
        commentsLayout->addWidget(listWidgetComments);
        historySplitter->addWidget(groupComments);

        historyPageLayout->addWidget(historySplitter);
        stackedWidget->addWidget(pageHistory);

        // ================= 页面3：排队看板 =================
        pageQueue = new QWidget();
        queuePageLayout = new QVBoxLayout(pageQueue);

        queueTopLayout = new QHBoxLayout();
        lblQueueCount = new QLabel("当前排队总数: 0 人", pageQueue);
        lblQueueCount->setStyleSheet("font-size: 14px; font-weight: bold;");
        queueTopLayout->addWidget(lblQueueCount);
        queueTopLayout->addStretch();
        
        btnRefreshQueue = new QPushButton("🔄 刷新排队进度", pageQueue);
        btnRefreshQueue->setObjectName(QString::fromUtf8("btnRefreshQueue"));
        queueTopLayout->addWidget(btnRefreshQueue);
        queuePageLayout->addLayout(queueTopLayout);

        tabWidgetQueueType = new QTabWidget(pageQueue);
        tabWidgetQueueType->setObjectName(QString::fromUtf8("tabWidgetQueueType"));
        tabReserve = new QWidget();
        tabWidgetQueueType->addTab(tabReserve, "预约排队进程");
        tabPickup = new QWidget();
        tabWidgetQueueType->addTab(tabPickup, "现场取餐进程");
        queuePageLayout->addWidget(tabWidgetQueueType);

        tableWidgetQueue = new QTableWidget(pageQueue);
        tableWidgetQueue->setObjectName(QString::fromUtf8("tableWidgetQueue"));
        tableWidgetQueue->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        queuePageLayout->addWidget(tableWidgetQueue);

        queueBottomLayout = new QHBoxLayout();
        btnJoinQueue = new QPushButton("➕ 申请排队入队", pageQueue);
        btnJoinQueue->setObjectName(QString::fromUtf8("btnJoinQueue"));
        queueBottomLayout->addWidget(btnJoinQueue);
        btnCallNext = new QPushButton("🔔 商家叫号出队", pageQueue);
        btnCallNext->setObjectName(QString::fromUtf8("btnCallNext"));
        queueBottomLayout->addWidget(btnCallNext);
        queuePageLayout->addLayout(queueBottomLayout);

        stackedWidget->addWidget(pageQueue);

        mainLayout->addWidget(stackedWidget);
        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);
        sidebarNav->setCurrentRow(0);
        stackedWidget->setCurrentIndex(0);

        QMetaObject::connectSlotsByName(MainWindow);
    }

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "高端智能多功能点单管理系统", nullptr));
    }
};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
}

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H