/* src/views/mainwindow.h */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include <QMap>
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

    // 核心菜单初始化展示
    void refreshDishMenuUI();

    // 需求1：购物车体系控制槽
    void on_btnAddToCart_clicked();
    void on_tableWidgetMenu_cellDoubleClicked(int row, int column);
    void on_tableWidgetCart_cellDoubleClicked(int row, int column); // 双击移出购物车
    void on_lineEditMemberId_textChanged(const QString &text);       // 无感登录检测机制
    void recalculateCartPrices();                                  // 自动动态算价

    // 需求2：订单批量提交并自动转入队列
    void on_btnSubmitOrder_clicked();

    // 评价检索与排序
    void on_comboCommentSort_currentIndexChanged(int index);

    // 历史克隆重现
    void on_copyHistoryOrder_clicked();

    // 队列动态消解与仿真控制
    void refreshQueueUI();
    void on_btnRefreshQueue_clicked();
    void on_btnJoinQueue_clicked();
    void on_btnCallNext_clicked();

private:
    Ui::MainWindow *ui;
    SystemEngine *m_engine;

    // 核心购物车缓冲：键=菜品名称，值=购买数量
    QMap<QString, int> m_cart;

    void initUIStyleAndConnections();
    void loadHistoryOrders();
    double getBasePriceOfDish(const QString &dishName);
};
#endif // MAINWINDOW_H