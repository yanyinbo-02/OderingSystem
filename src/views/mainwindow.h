#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QTableWidgetItem>
#include "../controllers/system_engine.h" // 引入控制层纽带

//所有控件基类
//QMainWindow 是 Qt 专门为主窗口设计的类，
//它默认集成了菜单栏（Menu Bar）、工具栏（Tool Bars）
//锚接窗口（Dock Widgets）和状态栏（Status Bar）
//自定义的窗口必须了解其定义才能进行继承。
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    //宏，生成用于实现信号和槽机制的代码
    Q_OBJECT

public:
    //构造函数
    //接收一个指向父窗口的指针
    //= nullptr：C++ 的默认实参。
    //如果创建窗口时不指定父对象（即传入 nullptr），则该窗口将作为独立顶层窗口显示。
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // 功能一 & 功能四：初始化与刷新菜品列表（包含分类与推荐属性额外显示）
    void refreshDishMenuUI();

    // 功能二：会员点单计算价格槽函数
    void on_btnCalculatePrice_clicked();
    void on_btnSubmitOrder_clicked();

    // 功能三：评价排序切换槽函数
    void on_comboCommentSort_currentIndexChanged(int index);

    // 功能五：点餐记忆历史快速复制
    void on_copyHistoryOrder_clicked();

    // 功能六：排队显示与实时刷新
    void refreshQueueUI();
    void on_btnJoinQueue_clicked();
    void on_btnCallNext_clicked();

private:
//是由 Qt 的 UI 设计器（.ui 文件）通过 uic 工具自动生成的纯界面类，
//只负责绘制皮肤和控件摆放。
    Ui::MainWindow *ui;
    //核心控制器指针
    SystemEngine *m_engine;

    //内部UI初始化辅助函数
    void initUIStyleAndConnections();
    void loadHistoryOrders();
};
#endif // MAINWINDOW_H