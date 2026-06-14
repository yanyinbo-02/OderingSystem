//订单与历史记录模型
// 订单管理模型
#ifndef ORDER_H
#define ORDER_H

#include <QString>
#include <QList>
#include "../controllers/system_engine.h"

class OrderManager {
public:
    static OrderManager& instance();

    // 增加新订单
    void addOrder(const QString &dishName);

    //打包订单录入接口
    void addGroupedOrder(const QList<OrderItem> &items, const QString &summary);
    
    // 获取历史订单列表
    QList<OrderModel> getOrders();

private:
    OrderManager();
    QList<OrderModel> m_orders;
    int m_orderIdCounter;
};

#endif // ORDER_H