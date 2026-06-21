//排队管理模型
#include "queue.h"

//：构造函数将两个代表“是否属于初次界面加载/呼叫”的布尔状态位（m_reserveFirstCall 和 m_pickupFirstCall）
//初始化为安全值 true
QueueManager::QueueManager(){
    // 初始化时，默认填充10名模拟伪用户排队
    initMockQueue(m_reserveQueue, "A");
    initMockQueue(m_pickupQueue, "B");
}

QueueManager& QueueManager::instance() {
    static QueueManager inst;
    return inst;
}

//当系统主界面第一次初始化调用此函数渲染时，直接将数据呈现
void QueueManager::initMockQueue(QList<QueueNode> &queue, const QString &prefix) {
    queue.clear();
    QDateTime now = QDateTime::currentDateTime();
    
    for (int i = 1; i <= 10; ++i) {
        // 生成排队号：前缀 + 3位数字（不足左补0），如 A001, B010
        QString queueId = QString("%1%2").arg(prefix).arg(i, 3, 10, QChar('0'));
        
        // 计算排队时间：从当前时间往前推移，模拟不同的入队先后时间
        int timeOffsetSeconds = (-20 + i) * 60;
        QDateTime queueTime = now.addSecs(timeOffsetSeconds);
        
        // 创建排队节点并加入队列
        queue.append(QueueNode{queueId, queueTime});
    }
}

QList<QueueNode> QueueManager::getQueue(int type) const {
    if (type == 0) {
        return m_reserveQueue;
    } else {
        return m_pickupQueue;
    }
}
//动态挂载接口。当用户在前台点击结账并付款成功后，自动化连带触发此业务。
//根据出餐性质定位到取餐或预约队列指针，
//将用户的识别号与当前即时捕获的秒级系统时间封装成节点，追加（.append()）到队伍的末尾（尾部入队）
void QueueManager::joinQueue(const QString &id, int type) {
    QList<QueueNode> *targetQueue = (type == 0) ? &m_reserveQueue : &m_pickupQueue;
    targetQueue->append(QueueNode{id, QDateTime::currentDateTime()});
}


// src.zip/models/queue.cpp 的部分修改
void QueueManager::callNext(int type) {
    if (type == 0) { // 预约排队
        if (!m_reserveQueue.isEmpty()) {
            // 记录下要被转移的顾客
            QueueNode nextCustomer = m_reserveQueue.first();
            m_reserveQueue.removeFirst();
            // 将该顾客转移到取餐队列
            m_pickupQueue.append(nextCustomer);
        }
    } else { // 现场取餐 (如果商家直接点取餐队列的叫号，依然是直接移除)
        if (!m_pickupQueue.isEmpty()) {
            m_pickupQueue.removeFirst();
        }
    }
}