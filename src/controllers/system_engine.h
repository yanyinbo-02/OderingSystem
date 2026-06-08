//核心业务联动引擎

#ifndef SYSTEM_ENGINE_H
#define SYSTEM_ENGINE_H

#include <QObject>
#include <QString>
#include <QList>
#include <QDateTime>

// 为了让 MainWindow 编译通过，先在这里定义 UI 用到的基础数据结构临时桩（Stub）
struct DishModel {
    QString name;
    QString category;
    double price;
    QString description;
    bool isTopFive;
};

struct CommentModel {
    int score;
    QString content;
    QDateTime time;
};

struct OrderModel {
    QString orderId;
    
    QString dishName;
};

struct QueueNode {
    QString id;
    QDateTime joinTime;
};

// 必须严格确保类名大小写与 mainwindow.h 中完全一致
class SystemEngine : public QObject {
    Q_OBJECT
public:
    explicit SystemEngine(QObject *parent = nullptr) : QObject(parent) {}
    virtual ~SystemEngine() = default;

    // 补齐 MainWindow 中调控制器的成员函数空壳，防止报“未定义成员”错误
    QList<DishModel> getAllDishes() { return QList<DishModel>(); }
    double calculatePrice(const QString &dishName, const QString &memberId) { return 0.0; }
    bool createOrder(const QString &dishName, const QString &memberId) { return true; }
    QList<CommentModel> getSortedComments(int index) { return QList<CommentModel>(); }
    QList<OrderModel> getHistoryOrders() { return QList<OrderModel>(); }
    bool duplicateOrderFromHistory(const QString &itemText) { return true; }
    QList<QueueNode> getQueueData(int type) { return QList<QueueNode>(); }
    void customerJoinQueue(const QString &id, int type) {}
    void callNextCustomer(int type) {}

signals:
    void queueStatusChanged();
    void menuDataChanged();
};

#endif // SYSTEM_ENGINE_H