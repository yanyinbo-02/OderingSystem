//排队管理模型
#include "queue.h"

//：构造函数将两个代表“是否属于初次界面加载/呼叫”的布尔状态位（m_reserveFirstCall 和 m_pickupFirstCall）
//初始化为安全值 true
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
        // 生成排队号：前缀 + 3位数字（不足左补0）
        QString queueId = QString("%1%2").arg(prefix).arg(i, 3, 10, QChar('0'));
        
        // 计算排队时间：从当前时间往前推移，模拟不同的入队时间
        int timeOffsetSeconds = (-20 + i) * 60;
        QDateTime queueTime = now.addSecs(timeOffsetSeconds);
        
        // 创建排队节点并加入队列
        queue.append(QueueNode{queueId, queueTime});
    }
}

QList<QueueNode> QueueManager::getQueue(int type) {
    // 根据类型选择对应的队列和首次调用标记
    QList<QueueNode> *targetQueue;
    bool *isFirstCall;
    
    if (type == 0) {
        // 预约队列
        targetQueue = &m_reserveQueue;
        isFirstCall = &m_reserveFirstCall;
    } else {
        // 取餐队列
        targetQueue = &m_pickupQueue;
        isFirstCall = &m_pickupFirstCall;
    }

    // 初次调用时，标记已读过，直接返回原队列
    if (*isFirstCall) {
        *isFirstCall = false;
    } else {
        // 非首次调用时，每次移除前两个客户（模拟他们已完成业务离开）
        for (int i = 0; i < 2; ++i) {
            if (!targetQueue->isEmpty()) {
                targetQueue->removeFirst();
            }
        }
    }
    
    return *targetQueue;
}

//动态挂载接口。当用户在前台点击结账并付款成功后，自动化连带触发此业务。
//根据出餐性质定位到取餐或预约队列指针，
//将用户的识别号与当前即时捕获的秒级系统时间封装成节点，追加（.append()）到队伍的末尾（尾部入队）
void QueueManager::joinQueue(const QString &id, int type) {
    QList<QueueNode> *targetQueue = (type == 0) ? &m_reserveQueue : &m_pickupQueue;
    targetQueue->append(QueueNode{id, QDateTime::currentDateTime()});
}

//模拟商户后厨或前台的“叫号消单”操作。先进行严谨的 .isEmpty() 容器空状态安全检查以防止指针越界崩溃，
//随后直接调用 removeFirst() 从队列头部移除当前等待顺位为第 1 名的客户
void QueueManager::callNext(int type) {
    QList<QueueNode> *targetQueue = (type == 0) ? &m_reserveQueue : &m_pickupQueue;
    if (!targetQueue->isEmpty()) {
        targetQueue->removeFirst();
    }
}