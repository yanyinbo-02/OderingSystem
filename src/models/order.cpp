//订单与历史记录模型
#include "order.h"
#include <QRegularExpression>

OrderManager::OrderManager() : m_orderIdCounter(1001) {
    // 预置复合型的历史大单消费记录，假装过往有丰富的真实购买打包数据
    QList<OrderItem> sampleItems1;
    sampleItems1.append({"招牌红烧肉", 1});
    sampleItems1.append({"经典波霸奶茶", 2});
    addGroupedOrder(sampleItems1, "招牌红烧肉(x1), 经典波霸奶茶(x2)");

    QList<OrderItem> sampleItems2;
    sampleItems2.append({"宫保鸡丁", 1});
    sampleItems2.append({"抹茶冰淇淋", 1});
    addGroupedOrder(sampleItems2, "宫保鸡丁(x1), 抹茶冰淇淋(x1)");
}

OrderManager& OrderManager::instance() {
    static OrderManager inst;
    return inst;
}

void OrderManager::addGroupedOrder(const QList<OrderItem> &items, const QString &summary) {
    m_orders.append({QString::number(m_orderIdCounter++), items, summary});
}

QList<OrderModel> OrderManager::getOrders() {
    return m_orders;
}

