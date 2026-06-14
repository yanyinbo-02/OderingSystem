//核心业务联动引擎
#include "system_engine.h"
#include "../models/dish.h"
#include "../models/member.h"
#include "../models/order.h"
#include "../models/queue.h"
//提供排序功能
#include <algorithm>

//system_engine.cpp 是整个点单管理系统的核心业务联动引擎。
//在软件架构中，它扮演了 控制器（Controller）的角色，
//负责向上对接 UI 界面（View），向下调度并串联各个底层的独立数据模型（Model）。

//初始化系统引擎对象
//设置父对象，用于Qt内存管理机制
SystemEngine::SystemEngine(QObject *parent) : QObject(parent) {}

QList<DishModel> SystemEngine::getAllDishes() {
    return DishManager::instance().getDishes();
} //调用菜品管理器的单例实例讲解获取所有菜品数据，实现解耦

//价格计算函数，传入菜品名称与会员号
double SystemEngine::calculatePrice(const QString &dishName, const QString &memberId) {
    //从菜单库中取出所有菜品列表
    QList<DishModel> dishes = DishManager::instance().getDishes();
    double basePrice = 0.0;
    //使用for循环遍历菜单
    for (const auto &dish : dishes) {
        //通过 dish.name == dishName 比对查找目标菜品。
        //一旦匹配成功，把原价赋值给 basePrice，并执行 break 立即跳出循环
        if (dish.name == dishName) {
            basePrice = dish.price;
            break;
        }
    }
    // 优惠折扣计算
    //调度会员管理器，传入会员号。底层会根据该会员的等级动态计算并返回折扣系数
    double discount = MemberManager::instance().getDiscount(memberId);
    //返回最终价格
    return basePrice * discount;
}

//订单创建函数，传入菜品名称与会员号
bool SystemEngine::createOrder(const QString &dishName, const QString &memberId) {
    //首先计算实际消费金额
    double finalPrice = calculatePrice(dishName, memberId);
    //将菜品写入订单管理器的订单列表中
    OrderManager::instance().addOrder(dishName);
    // 积分累加
    MemberManager::instance().addPoints(memberId, finalPrice);
    //向外部广播“菜单/购物车数据改变”信号
    emit menuDataChanged();
    return true;
}

QList<CommentModel> SystemEngine::getSortedComments(int index) {
    // 固定已有的评价数据
    QList<CommentModel> comments;
    QDateTime now = QDateTime::currentDateTime();
    //硬编码了五个不同评论
    comments.append(CommentModel{5, "招牌红烧肉肥而不腻", now.addDays(-2)});
    comments.append(CommentModel{4, "服务态度很好。", now.addDays(-1)});
    comments.append(CommentModel{5, "每次来必点宫保鸡丁", now.addSecs(-5 * 3600)});
    comments.append(CommentModel{3, "排队的人稍微有点多。", now.addSecs(-2 * 3600)});
    comments.append(CommentModel{2, "鱼肉稍微有点腥", now.addSecs(-30 * 60)});

    // 排序逻辑：0-按评分从高到低，1-按留言时间先后
    if (index == 0) {
        //调用 C++ 标准库的高效快速排序算法。第三个参数传入了一个 Lambda 表达式（匿名函数）
        std::sort(comments.begin(), comments.end(), [](const CommentModel &a, const CommentModel &b) {
            return a.score > b.score;
        });
    } else if (index == 1) {
        std::sort(comments.begin(), comments.end(), [](const CommentModel &a, const CommentModel &b) {
            return a.time < b.time;
        });
    }
    return comments;
}

//得到历史记录
QList<OrderModel> SystemEngine::getHistoryOrders() {
    return OrderManager::instance().getOrders();
}

//实现历史订单复用功能
bool SystemEngine::duplicateOrderFromHistory(const QString &itemText) {
    //界面会把用户点击的那一行文本直接丢过来。
    //订单管理器内部会通过正则表达式提取出菜品名称并自动重新下单
    bool success = OrderManager::instance().duplicateFromText(itemText);
    if (success) {
        //发送变动信号
        emit menuDataChanged();
    }
    return success;
}

//获取队列信息
QList<QueueNode> SystemEngine::getQueueData(int type) {
    return QueueManager::instance().getQueue(type);
}

//系统设计了两个队列：type = 0 代表“预约排队”，type = 1 代表“现场取餐”
void SystemEngine::customerJoinQueue(const QString &id, int type) {
    //顾客加入排队。调用 QueueManager 将顾客（会员号或水单号）追加到对应队列的末尾
    QueueManager::instance().joinQueue(id, type);
    //发送队列改变信号
    emit queueStatusChanged();
}

void SystemEngine::callNextCustomer(int type) {
    //商家叫号出队。调用 QueueManager 移除当前队列头部的第一位顾客
    QueueManager::instance().callNext(type);
    emit queueStatusChanged();
}