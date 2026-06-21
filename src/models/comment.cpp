//评论模型
#include "comment.h"
#include <algorithm>

//单例构造，与 DishManager / MemberManager / OrderManager / QueueManager 风格保持一致
CommentManager::CommentManager() {}

CommentManager& CommentManager::instance() {
    static CommentManager inst;
    return inst;
}

//获取排序后的评论（保留原有硬编码的五条评论，并融合用户下单后主动提交的动态评价）
QList<CommentModel> CommentManager::getSortedComments(int index) {
    QList<CommentModel> comments;
    QDateTime now = QDateTime::currentDateTime();

    // 1. 保留原有代码中的硬编码五条评论（演示性数据，targetTag/memberId 留空表示系统预置）
    comments.append(CommentModel{5, "招牌红烧肉肥而不腻", now.addDays(-2), "", ""});
    comments.append(CommentModel{4, "服务态度很好。", now.addDays(-1), "", ""});
    comments.append(CommentModel{5, "每次来必点宫保鸡丁", now.addSecs(-5 * 3600), "", ""});
    comments.append(CommentModel{3, "排队的人稍微有点多。", now.addSecs(-2 * 3600), "", ""});
    comments.append(CommentModel{2, "鱼肉稍微有点腥", now.addSecs(-30 * 60), "", ""});

    // 2. 融合用户下单后主动提交的真实动态评价
    comments.append(m_dynamicComments);

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

//用户在下单后主动评分评论，写入动态评价列表
bool CommentManager::addComment(const QString &dishOrOrderTag, const QString &memberId, int score, const QString &content) {
    // 评分合法性边界校验：强制截断到 1~5 区间，防止脏数据进入展示层
    if (score < 1) score = 1;
    if (score > 5) score = 5;

    CommentModel newComment;
    newComment.score = score;
    newComment.content = content;
    newComment.time = QDateTime::currentDateTime();
    newComment.targetTag = dishOrOrderTag;
    newComment.memberId = memberId;

    m_dynamicComments.append(newComment);
    return true;
}

//获取指定会员的历史评价（按时间倒序，最新的评价排在最前）
QList<CommentModel> CommentManager::getMemberHistory(const QString &memberId) const {
    QList<CommentModel> history;
    for (const auto &comment : m_dynamicComments) {
        if (comment.memberId == memberId) {
            history.append(comment);
        }
    }
    std::sort(history.begin(), history.end(), [](const CommentModel &a, const CommentModel &b) {
        return a.time > b.time;
    });
    return history;
}