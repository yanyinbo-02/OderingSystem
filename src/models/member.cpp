//会员模型
#include "member.h"

MemberManager::MemberManager(){
}

//同样以局部静态变量的形式提供单例实例，保证全系统所有扣款计算和积分变动操作的都是同一套会员状态数据
MemberManager& MemberManager::instance() {
    static MemberManager inst;
    return inst;
}

double MemberManager::getDiscount(const QString &memberId) {
    if (memberId.isEmpty()) {
        return 1.0; // 非会员不享受折扣
    }

    // 从映射表中根据卡号获取对应积分，若该会员是第一次消费（未在 Map 中记录），value() 会安全返回默认值 0
    int points = m_memberPoints.value(memberId, 0);

    // 会员等级折扣动态折算机制
    // 根据积分区间映射不同的阶梯式优惠折扣
    if (points <= 100) {
        return 0.95; // 普通会员：95折
    } else if (points <= 500) {
        return 0.90; // 黄金会员：9折
    } else {
        return 0.85; // 钻石会员：85折
    }
}

void MemberManager::addPoints(const QString &memberId, double finalPrice) {
    if (memberId.isEmpty()) return;
    
    // 会员每消费一元积一分，采用向下取整原则
    m_memberPoints[memberId] += static_cast<int>(finalPrice);
}
int MemberManager::getPoints(const QString &memberId) const {
    // 使用 value() 保证 const 函数的只读安全性，未找到时返回 0 分
    return m_memberPoints.value(memberId, 0);
}