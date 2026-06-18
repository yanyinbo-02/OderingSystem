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


//QString::number(m_orderIdCounter++) 巧妙利用了后缀自增运算符。
//其工作原理是：先取出当前计数器的整数值（例如第一次为 1001），
//通过 QString::number() 转换为文本类型作为当前订单的唯一识别号（Order ID）；
//紧接着将计数器累加 1（变为 1002），为下一个订单的生成做准备。
void OrderManager::addGroupedOrder(const QList<OrderItem> &items, const QString &summary) {
    m_orders.append({QString::number(m_orderIdCounter++), items, summary});
}

QList<OrderModel> OrderManager::getOrders() {
    return m_orders;
}

