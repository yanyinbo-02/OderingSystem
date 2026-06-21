//核心业务联动引擎
#include "system_engine.h"
#include "../models/dish.h"
#include "../models/member.h"
#include "../models/order.h"
#include "../models/queue.h"
#include "../models/comment.h" // 新增引入
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

//评价数据的统一出口：直接委派给 CommentManager 单例，
//该单例内部融合了硬编码的演示性评论与用户下单后主动提交的真实动态评价，
//并按照传入的 index（0-评分优先，1-时间优先）完成排序
QList<CommentModel> SystemEngine::getSortedComments(int index) {
    return CommentManager::instance().getSortedComments(index);
}

//用户提交一条新评价。targetTag 通常传入订单号（也可传菜品名，视前端弹窗设计而定），
//memberId 取自当前登录态（散客可为空），score 与 content 来自评价弹窗的输入控件
bool SystemEngine::addComment(const QString &targetTag, const QString &memberId, int score, const QString &content) {
    bool ok = CommentManager::instance().addComment(targetTag, memberId, score, content);
    if (ok) {
        // 广播评价数据变更信号，驱动 UI 端的评价看板自动刷新
        emit commentDataChanged();
    }
    return ok;
}

MemberInfoModel SystemEngine::getMemberInfo(const QString &memberId) {
    MemberInfoModel info;
    if (memberId.isEmpty()) {
        info.levelName = "散客顾客";
        info.discount = 1.0;
        info.points = 0;
        return info;
    }

    // 调用底层单例获取真实的积分，必须把当前输入的 memberId 传入
    info.points = MemberManager::instance().getPoints(memberId);
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