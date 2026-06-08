// 核心业务联动引擎
#ifndef SYSTEM_ENGINE_H
#define SYSTEM_ENGINE_H

#include <QObject>
#include <QString>
#include <QList>
#include <QDateTime>

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

class SystemEngine : public QObject {
    Q_OBJECT
public:
    explicit SystemEngine(QObject *parent = nullptr);
    virtual ~SystemEngine() = default;

    // 【核心修正】移除了导致编译重定义冲突的 {} 内联体，完全交由 system_engine.cpp 实现
    QList<DishModel> getAllDishes();
    double calculatePrice(const QString &dishName, const QString &memberId);
    bool createOrder(const QString &dishName, const QString &memberId);
    QList<CommentModel> getSortedComments(int index);
    QList<OrderModel> getHistoryOrders();
    bool duplicateOrderFromHistory(const QString &itemText);
    QList<QueueNode> getQueueData(int type);
    void customerJoinQueue(const QString &id, int type);
    void callNextCustomer(int type);

signals:
    void queueStatusChanged();
    void menuDataChanged();
};

#endif // SYSTEM_ENGINE_H