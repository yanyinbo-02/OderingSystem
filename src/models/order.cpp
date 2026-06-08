//订单与历史记录模型
#include "order.h"
#include <QRegularExpression>

OrderManager::OrderManager() : m_orderIdCounter(1001) {
    // 预置固定的历史消费记录，假装过往有丰富的购买数据
    addOrder("招牌红烧肉");
    addOrder("经典波霸奶茶");
    addOrder("宫保鸡丁");
}

OrderManager& OrderManager::instance() {
    static OrderManager inst;
    return inst;
}

void OrderManager::addOrder(const QString &dishName) {
    m_orders.append({QString::number(m_orderIdCounter++), dishName});
}

QList<OrderModel> OrderManager::getOrders() {
    return m_orders;
}

bool OrderManager::duplicateFromText(const QString &itemText) {
    // 历史组件传递的标准文本格式为："订单号: 1001 | 菜品: 招牌红烧肉"
    // 利用非贪婪匹配正则表达式精准拉取菜品字段
    QRegularExpression re("菜品:\\s*(.*)");
    QRegularExpressionMatch match = re.match(itemText);
    
    if (match.hasMatch()) {
        QString extractedDishName = match.captured(1).trimmed();
        addOrder(extractedDishName); // 快捷复制下单成功
        return true;
    }
    return false;
}