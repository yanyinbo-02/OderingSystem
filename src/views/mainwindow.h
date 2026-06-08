/* src/views/mainwindow.h */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include "../controllers/system_engine.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // 基础导航路由
    void on_sidebarNav_currentRowChanged(int row);
    void on_tabWidgetQueueType_currentChanged(int index);

    // 功能一 & 功能四：初始化与刷新菜品列表
    void refreshDishMenuUI();

    // 功能二：会员点单计算价格与提交
    void on_btnCalculatePrice_clicked();
    void on_btnSubmitOrder_clicked();

    // 功能三：评价排序切换
    void on_comboCommentSort_currentIndexChanged(int index);

    // 功能五：点餐记忆历史快速复制
    void on_copyHistoryOrder_clicked();

    // 功能六：排队显示与实时刷新模拟
    void refreshQueueUI();
    void on_btnRefreshQueue_clicked(); // 处理每次刷新减少2人的核心槽
    void on_btnJoinQueue_clicked();
    void on_btnCallNext_clicked();

private:
    Ui::MainWindow *ui;
    SystemEngine *m_engine;

    void initUIStyleAndConnections();
    void loadHistoryOrders();
};
#endif // MAINWINDOW_H