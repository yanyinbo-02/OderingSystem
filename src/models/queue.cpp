//排队管理模型
#include "queue.h"

QueueManager::QueueManager() : m_reserveFirstCall(true), m_pickupFirstCall(true) {
    // 初始化时，默认填充10名模拟伪用户排队
    initMockQueue(m_reserveQueue, "A");
    initMockQueue(m_pickupQueue, "B");
}

QueueManager& QueueManager::instance() {
    static QueueManager inst;
    return inst;
}

void QueueManager::initMockQueue(QList<QueueNode> &queue, const QString &prefix) {
    queue.clear();
    QDateTime now = QDateTime::currentDateTime();
    for (int i = 1; i <= 10; ++i) {
        queue.append(QueueNode{QString("%1%2").arg(prefix).arg(i, 3, 10, QChar('0')), now.addSecs((-20 + i) * 60)});
    }
}

QList<QueueNode> QueueManager::getQueue(int type) {
    QList<QueueNode> *targetQueue = (type == 0) ? &m_reserveQueue : &m_pickupQueue;
    bool *isFirstCall = (type == 0) ? &m_reserveFirstCall : &m_pickupFirstCall;

    // 核心仿真要求：初次显示10人，之后每刷新（调用）一次，减少2人
    if (*isFirstCall) {
        *isFirstCall = false; // 消费掉首次加载标记，后续刷新将触发递减
    } else {
        for (int i = 0; i < 2; ++i) {
            if (!targetQueue->isEmpty()) {
                targetQueue->removeFirst(); // 模拟前排客户已完成业务离开
            }
        }
    }
    return *targetQueue;
}

void QueueManager::joinQueue(const QString &id, int type) {
    QList<QueueNode> *targetQueue = (type == 0) ? &m_reserveQueue : &m_pickupQueue;
    targetQueue->append(QueueNode{id, QDateTime::currentDateTime()});
}

void QueueManager::callNext(int type) {
    QList<QueueNode> *targetQueue = (type == 0) ? &m_reserveQueue : &m_pickupQueue;
    if (!targetQueue->isEmpty()) {
        targetQueue->removeFirst();
    }
}