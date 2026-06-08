//会员模型
#include "member.h"

MemberManager::MemberManager() : m_points(150) {
    // 初始赋予 150 积分（处于黄金会员级别），方便界面渲染观察
}

MemberManager& MemberManager::instance() {
    static MemberManager inst;
    return inst;
}

double MemberManager::getDiscount(const QString &memberId) {
    if (memberId.isEmpty()) {
        return 1.0; // 非会员不享受折扣
    }

    // 根据积分区间映射不同的阶梯式优惠折扣
    if (m_points <= 100) {
        return 0.95; // 普通会员：95折
    } else if (m_points <= 500) {
        return 0.90; // 黄金会员：9折
    } else {
        return 0.85; // 钻石会员：85折
    }
}

void MemberManager::addPoints(const QString &memberId, double finalPrice) {
    if (memberId.isEmpty()) return;
    
    // 会员每消费一元积一分，采用向下取整原则
    m_points += static_cast<int>(finalPrice);
}

int MemberManager::getPoints() const {
    return m_points;
}