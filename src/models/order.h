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
    
    // 获取历史订单列表
    QList<OrderModel> getOrders();
    
    // 从界面提取的文本中反向克隆创建订单
    bool duplicateFromText(const QString &itemText);

private:
    OrderManager();
    QList<OrderModel> m_orders;
    int m_orderIdCounter;
};

#endif // ORDER_H