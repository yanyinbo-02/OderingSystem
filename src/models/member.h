// 会员模型
// 会员模型
#ifndef MEMBER_H
#define MEMBER_H

#include <QString>
#include <QMap> // 引入 QMap 容器

class MemberManager {
public:
    static MemberManager& instance();

    // 获取会员专属折扣系数
    double getDiscount(const QString &memberId);
    
    // 累加会员积分
    void addPoints(const QString &memberId, double finalPrice);

    // 【改进】获取指定会员的当前积分，支持多卡号区分
    int getPoints(const QString &memberId) const;

private:
    MemberManager();
    
    // 【重构】从单个整型变量改为映射表：Key 为会员号，Value 为对应积分
    QMap<QString, int> m_memberPoints; 
};

#endif // MEMBER_H