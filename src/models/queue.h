//排队管理模型
// 排队队列模型
#ifndef QUEUE_H
#define QUEUE_H

#include <QString>
#include <QList>
#include "../controllers/system_engine.h"

class QueueManager {
public:
    static QueueManager& instance();

    // 获取实时队列数据（包含每刷新一次减少2人的仿真逻辑）
    QList<QueueNode> getQueue(int type);
    
    // 客户新加入排队
    void joinQueue(const QString &id, int type);
    
    // 商家呼叫下一位出队
    void callNext(int type);

private:
    QueueManager();
    
    QList<QueueNode> m_reserveQueue; // 预约排队队列 (type == 0)
    QList<QueueNode> m_pickupQueue;  // 取餐排队队列 (type == 1)
    
    bool m_reserveFirstCall;
    bool m_pickupFirstCall;

    void initMockQueue(QList<QueueNode> &queue, const QString &prefix);
};

#endif // QUEUE_H