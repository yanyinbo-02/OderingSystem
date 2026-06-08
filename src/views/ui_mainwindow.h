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
    
    // 导航
    QListWidget *sidebarNav;
    QStackedWidget *stackedWidget;

    // 页面1：点餐大厅
    QWidget *pageMenu;
    QHBoxLayout *menuPageLayout;
    QTableWidget *tableWidgetMenu;
    QGroupBox *groupBilling;
    QVBoxLayout *billingLayout;
    QLineEdit *lineEditMemberId;
    QPushButton *btnCalculatePrice;
    QLabel *lblFinalPrice;
    QPushButton *btnSubmitOrder;

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
        MainWindow->resize(950, 650);
        
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
        
        // ================= 页面1：点餐大厅 =================
        pageMenu = new QWidget();
        menuPageLayout = new QHBoxLayout(pageMenu);
        
        tableWidgetMenu = new QTableWidget(pageMenu);
        tableWidgetMenu->setObjectName(QString::fromUtf8("tableWidgetMenu"));
        tableWidgetMenu->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        menuPageLayout->addWidget(tableWidgetMenu, 3);

        groupBilling = new QGroupBox("会员结算中心", pageMenu);
        billingLayout = new QVBoxLayout(groupBilling);
        billingLayout->setSpacing(15);
        
        billingLayout->addWidget(new QLabel("请输入会员卡号:"));
        lineEditMemberId = new QLineEdit(groupBilling);
        lineEditMemberId->setObjectName(QString::fromUtf8("lineEditMemberId"));
        lineEditMemberId->setPlaceholderText("留空视作散客办理");
        billingLayout->addWidget(lineEditMemberId);

        btnCalculatePrice = new QPushButton("计算消费价格", groupBilling);
        btnCalculatePrice->setObjectName(QString::fromUtf8("btnCalculatePrice"));
        billingLayout->addWidget(btnCalculatePrice);

        lblFinalPrice = new QLabel("实际消费价格: ￥0.00", groupBilling);
        lblFinalPrice->setStyleSheet("font-weight: bold; font-size: 14px; color: #2E7D32;");
        billingLayout->addWidget(lblFinalPrice);

        btnSubmitOrder = new QPushButton("提交订单付款", groupBilling);
        btnSubmitOrder->setObjectName(QString::fromUtf8("btnSubmitOrder"));
        btnSubmitOrder->setStyleSheet("background-color: #1976D2; color: white; font-weight: bold; height: 35px;");
        billingLayout->addWidget(btnSubmitOrder);
        billingLayout->addStretch();
        
        menuPageLayout->addWidget(groupBilling, 1);
        stackedWidget->addWidget(pageMenu);

        // ================= 页面2：历史与评价 =================
        pageHistory = new QWidget();
        historyPageLayout = new QHBoxLayout(pageHistory);
        historySplitter = new QSplitter(Qt::Horizontal, pageHistory);

        // 历史复制部分
        groupHistory = new QGroupBox("点餐记忆 (双击或选中复制)");
        historyLayout = new QVBoxLayout(groupHistory);
        listWidgetHistory = new QListWidget(groupHistory);
        listWidgetHistory->setObjectName(QString::fromUtf8("listWidgetHistory"));
        historyLayout->addWidget(listWidgetHistory);
        copyHistoryOrder = new QPushButton("🔄 复制该订单再来一单", groupHistory);
        copyHistoryOrder->setObjectName(QString::fromUtf8("copyHistoryOrder"));
        historyLayout->addWidget(copyHistoryOrder);
        historySplitter->addWidget(groupHistory);

        // 评价过滤展示
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
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "智能点单管理系统 (MVC架构演示)", nullptr));
    }
};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
}

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H