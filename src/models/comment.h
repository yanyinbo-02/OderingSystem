//评论模型
#ifndef COMMENT_H
#define COMMENT_H

#include <QString>
#include <QList>
#include "../controllers/system_engine.h"   // 复用 CommentModel 定义，与全系统数据结构保持统一

// 评论管理器：单例模式，全局唯一的评价数据源
// 职责：1. 提供硬编码的展示性基础评论（保留原有5条文案）
//      2. 持久化存储用户在“下单后评论”环节主动提交的真实评价
//      3. 提供统一的排序检索接口供 SystemEngine 调用
class CommentManager {
public:
    static CommentManager& instance();

    // 获取排序后的评论列表（硬编码 + 动态评论融合）
    // index: 0-按评分从高到低排序，1-按留言时间先后排序
    QList<CommentModel> getSortedComments(int index);

    // 新增：用户提交一条新评价
    // dishOrOrderTag：评价对象标识（菜品名 或 订单号，用于关联展示）
    // memberId：评价人会员号（散客可为空）
    // score：评分 1-5，越界会被自动截断
    // content：评论正文
    // 返回值：始终为 true（预留未来失败场景，如非法订单号校验等扩展空间）
    bool addComment(const QString &dishOrOrderTag, const QString &memberId, int score, const QString &content);

    // 获取指定会员的历史评价（按时间倒序）
    QList<CommentModel> getMemberHistory(const QString &memberId) const;

private:
    CommentManager();

    CommentManager(const CommentManager&) = delete;
    CommentManager& operator=(const CommentManager&) = delete;

    // 用户下单后动态提交的真实评价，与硬编码展示数据分离存放，避免污染演示数据
    QList<CommentModel> m_dynamicComments;
};

#endif // COMMENT_H