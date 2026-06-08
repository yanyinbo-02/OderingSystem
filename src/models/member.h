// 会员模型
#ifndef MEMBER_H
#define MEMBER_H

#include <QString>

class MemberManager {
public:
    static MemberManager& instance();

    // 获取会员专属折扣系数
    double getDiscount(const QString &memberId);
    
    // 累加会员积分
    void addPoints(const QString &memberId, double finalPrice);

    // 获取当前积分
    int getPoints() const;

private:
    MemberManager();
    int m_points; // 维护单用户的动态积分
};

#endif // MEMBER_H