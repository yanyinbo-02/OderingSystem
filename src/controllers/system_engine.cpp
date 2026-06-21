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
bool SystemEngine::createGroupedOrder(const QMap<QString, int> &cart, const QString &memberId) {
    //非零输入校验：如果购物车数据为空，则直接返回 false，表示订单创建失败
    if (cart.isEmpty()) return false;

    //实例化一个动态数组，存储 OrderItem 结构体
    QList<OrderItem> orderItems;
    //Qt 专属的字符串列表类
    QStringList summaryParts;
    double totalOrderFinalPrice = 0.0;

    // 遍历整个购物车缓冲区
    //cart.constBegin() 返回一个指向容器中第一个元素的只读（常量）迭代器
    //auto 关键字由编译器自动推导迭代器的具体类型（此处实际类型为 QMap<QString, int>::const_iterator）
    //只要当前迭代器 it 没有到达容器的尾部指针，循环就继续执行
    //前置递增运算符将迭代器移动到当前元素的下一个邻接位置
    for (auto it = cart.constBegin(); it != cart.constEnd(); ++it) {
        //在循环域内构造临时结构体，利用迭代器的成员函数 it.key() 获取 Map 的键（菜名），
        //it.value() 获取值（数量），最后调用 append() 将对象压入链表

        //将无序/松散的映射表关系转化成强类型的业务模型对象 OrderItem
        OrderItem item;
        item.dishName = it.key();
        item.count = it.value();
        orderItems.append(item);

        // 拼接摘要字符串，生成人类易读的消费条目片段。
        //QString::arg() 链式占位符替换
        //第一个 .arg() 自动替换 %1（菜名），
        //第二个 .arg() 自动替换 %2（数量并完成 int 到 QString 的隐式转换）
        summaryParts.append(QString("%1(x%2)").arg(item.dishName).arg(item.count));

        // 计算折后单价并累加到总额，以便进行积分计算
        double singleFinalPrice = calculatePrice(item.dishName, memberId);
        totalOrderFinalPrice += (singleFinalPrice * item.count);
    }

    // 获取全局唯一的订单管理实例
    //summaryParts.join(", ")：将原本彼此独立的字符串数组用 ", "（逗号+空格）首尾相连，融合成一个单一的 QString
    //它将刚才打包的所有菜品明细 orderItems 和最终压平的摘要送入历史订单库中持久化
    OrderManager::instance().addGroupedOrder(orderItems, summaryParts.join(", "));
    
    // 2. 会员全单积分动态累加
    MemberManager::instance().addPoints(memberId, totalOrderFinalPrice);

    //emit:Qt 扩展关键字（底层被转换为宏，编译时由 MOC 处理），用于发射信号
    //menuDataChanged()：定义在类元数据中的无参信号（Signal）
    emit menuDataChanged();
    return true;
}

//模拟生成一组餐饮系统的评价数据，并根据前端传入的筛选索引（评分或时间）进行动态排序
QList<CommentModel> SystemEngine::getSortedComments(int index) {
    // 局部变量声明
    //初始化一个空的 QList 容器，命名为 comments，用于临时存放和处理接下来的模拟评论数据
    QList<CommentModel> comments;
    //获取当前操作系统的绝对日期和时间，作为模拟历史评论发表时间的计算基准
    QDateTime now = QDateTime::currentDateTime();
    //硬编码了五个不同评论
    //comments.append(...)：调用 QList 的成员函数，将一个新元素追加到列表末尾。
    //列表初始化语法，直接构造 CommentModel 结构体实例（包含三个字段：评分、内容、时间）。
    //now.addDays(-2) / now.addSecs(...)：调用 QDateTime 的成员函数。通过传入负数，实现对基准时间向前推算
    comments.append(CommentModel{5, "招牌红烧肉肥而不腻", now.addDays(-2)});
    comments.append(CommentModel{4, "服务态度很好。", now.addDays(-1)});
    comments.append(CommentModel{5, "每次来必点宫保鸡丁", now.addSecs(-5 * 3600)});
    comments.append(CommentModel{3, "排队的人稍微有点多。", now.addSecs(-2 * 3600)});
    comments.append(CommentModel{2, "鱼肉稍微有点腥", now.addSecs(-30 * 60)});

    // 排序逻辑：0-按评分从高到低，1-按留言时间先后
    //引入自标准库 <algorithm> 的高效排序算法
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
    //返回已经排好序的评论数据列表
    return comments;
}

MemberInfoModel SystemEngine::getMemberInfo(const QString &memberId) {
    MemberInfoModel info;
    if (memberId.isEmpty()) {
        info.levelName = "散客顾客";
        info.discount = 1.0;
        info.points = 0;
        return info;
    }

    // 调用底层单例获取真实的积分与折扣，保证业务逻辑的高内聚
    info.points = MemberManager::instance().getPoints();
    info.discount = MemberManager::instance().getDiscount(memberId);

    // 严格对齐底层的阶梯式优惠折扣规则进行文本映射
    if (info.points <= 100) {
        info.levelName = "普通会员";
    } else if (info.points <= 500) {
        info.levelName = "黄金会员";
    } else {
        info.levelName = "钻石VIP会员";
    }

    return info;
}

//得到历史记录
QList<OrderModel> SystemEngine::getHistoryOrders() {
    return OrderManager::instance().getOrders();
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

void SystemEngine::serveDish() {
    // 强制操作取餐队列 (type 1)
    QueueManager::instance().callNext(1);
    // 发送队列改变信号，驱动 UI 刷新
    emit queueStatusChanged();
}