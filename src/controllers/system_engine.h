// 核心业务联动引擎
#ifndef SYSTEM_ENGINE_H
#define SYSTEM_ENGINE_H

#include <QObject>
#include <QString>
#include <QList>
#include <QDateTime>
#include <QMap>

//这些数据载体在各层之间流转，保证解耦
struct DishModel {
    QString name;  //菜品名称
    QString category;  //菜品分类
    double price;  //标准原价
    QString description;  //菜品描述
    bool isTopFive;  //是否为爆款
}; //菜品模型

struct CommentModel {
    int score;  //评分
    QString content;  //评价正文
    QDateTime time;   //评价发表时间
}; //评论模型

struct OrderItem {
    QString dishName;
    int count;
}; //单个订单内菜品明细结构

struct OrderModel {
    QString orderId;  //自动生成的订单号
    QList<OrderItem> items; // 包含的多菜品明细
    QString summary;
}; //（打包）点单模型

struct QueueNode {
    QString id;  //排队识别号（会员号或流水号）
    QDateTime joinTime;  //入队时间
}; //排队模型

//SystemEngine继承自QObject,能够使用Qt的信号槽机制
class SystemEngine : public QObject {
    //Qt核心宏，编译时被扫描，自动生成处理信号与槽所需底层C++代码
    Q_OBJECT
public:
    explicit SystemEngine(QObject *parent = nullptr); 
    virtual ~SystemEngine() = default;

    // 暴露给 UI (MainWindow) 调用的纯虚/普通函数
    QList<DishModel> getAllDishes();  //菜单获取
    double calculatePrice(const QString &dishName, const QString &memberId);  //价格计算
    bool createGroupedOrder(const QMap<QString, int> &cart, const QString &memberId); //批量提交购物车下单接口
    QList<CommentModel> getSortedComments(int index); //评价排序
    QList<OrderModel> getHistoryOrders(); //获取历史订单
    QList<QueueNode> getQueueData(int type);  //获取排队信息
    void customerJoinQueue(const QString &id, int type);  //加入排队
    void callNextCustomer(int type);  //传唤客户

    //signals:：定义了引擎对外广播的事件
signals:
    void queueStatusChanged();  //队列状态改变信号
    void menuDataChanged();  //菜单订单数据改变信号
};

#endif // SYSTEM_ENGINE_H