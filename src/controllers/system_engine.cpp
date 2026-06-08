//核心业务联动引擎
#include "system_engine.h"
#include "../models/dish.h"
#include "../models/member.h"
#include "../models/order.h"
#include "../models/queue.h"
#include <algorithm>

SystemEngine::SystemEngine(QObject *parent) : QObject(parent) {}

QList<DishModel> SystemEngine::getAllDishes() {
    return DishManager::instance().getDishes();
}

double SystemEngine::calculatePrice(const QString &dishName, const QString &memberId) {
    QList<DishModel> dishes = DishManager::instance().getDishes();
    double basePrice = 0.0;
    for (const auto &dish : dishes) {
        if (dish.name == dishName) {
            basePrice = dish.price;
            break;
        }
    }
    // 优惠折扣计算
    double discount = MemberManager::instance().getDiscount(memberId);
    return basePrice * discount;
}

bool SystemEngine::createOrder(const QString &dishName, const QString &memberId) {
    double finalPrice = calculatePrice(dishName, memberId);
    OrderManager::instance().addOrder(dishName);
    // 积分累加
    MemberManager::instance().addPoints(memberId, finalPrice);
    emit menuDataChanged();
    return true;
}

QList<CommentModel> SystemEngine::getSortedComments(int index) {
    // 固定已有的评价数据
    QList<CommentModel> comments;
    QDateTime now = QDateTime::currentDateTime();
    comments.append(CommentModel{5, "味道超级棒，招牌红烧肉肥而不腻！", now.addDays(-2)});
    comments.append(CommentModel{4, "上菜速度挺快的，服务态度很好。", now.addDays(-1)});
    comments.append(CommentModel{5, "每次来必点宫保鸡丁，绝美！", now.addSecs(-5 * 3600)});
    comments.append(CommentModel{3, "环境还可以，但是排队的人稍微有点多。", now.addSecs(-2 * 3600)});
    comments.append(CommentModel{2, "鱼肉稍微有点腥，希望下次能改进下。", now.addSecs(-30 * 60)});

    // 排序逻辑：0-按评分从高到低，1-按留言时间先后
    if (index == 0) {
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

QList<OrderModel> SystemEngine::getHistoryOrders() {
    return OrderManager::instance().getOrders();
}

bool SystemEngine::duplicateOrderFromHistory(const QString &itemText) {
    bool success = OrderManager::instance().duplicateFromText(itemText);
    if (success) {
        emit menuDataChanged();
    }
    return success;
}

QList<QueueNode> SystemEngine::getQueueData(int type) {
    return QueueManager::instance().getQueue(type);
}

void SystemEngine::customerJoinQueue(const QString &id, int type) {
    QueueManager::instance().joinQueue(id, type);
    emit queueStatusChanged();
}

void SystemEngine::callNextCustomer(int type) {
    QueueManager::instance().callNext(type);
    emit queueStatusChanged();
}