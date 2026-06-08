//菜品与菜单模型
#include "dish.h"
#include <algorithm>

DishManager::DishManager() {
    // 固定初始化菜品，其中 5 个标记为 Top 5 爆款推荐
    m_dishes << DishModel{"招牌红烧肉", "精选主食", 48.0, "肥而不腻，入口即化", true};
    m_dishes << DishModel{"清蒸鲈鱼", "精选主食", 58.0, "鲜美嫩滑，营养丰富", true};
    m_dishes << DishModel{"宫保鸡丁", "精选主食", 28.0, "酸辣适口，经典下饭", true};
    m_dishes << DishModel{"经典波霸奶茶", "即调饮品", 15.0, "浓郁奶香，Q弹珍珠", true};
    m_dishes << DishModel{"抹茶冰淇淋", "精美甜点", 18.0, "纯正抹茶，清甜解腻", true};
    m_dishes << DishModel{"清炒时蔬", "家常热菜", 16.0, "清脆爽口，少油健康", false};
    m_dishes << DishModel{"酸辣土豆丝", "家常热菜", 12.0, "口感爽脆，酸辣开胃", false};
}

DishManager& DishManager::instance() {
    static DishManager inst;
    return inst;
}

QList<DishModel> DishManager::getDishes() {
    return m_dishes;
}

double DishManager::getDishPrice(const QString &name) {
    for (const auto &dish : m_dishes) {
        if (dish.name == name) return dish.price;
    }
    return 0.0;
}

QList<CommentModel> DishManager::getSortedComments(int index) {
    QList<CommentModel> comments;
    QDateTime baseTime = QDateTime::currentDateTime();
    
    // 注入固定的评价数据
    comments << CommentModel{5, "红烧肉绝了，必点！", baseTime.addDays(-2)};
    comments << CommentModel{4, "奶茶味道很好，美中不足是稍微有点甜。", baseTime.addDays(-1)};
    comments << CommentModel{5, "鲈鱼非常新鲜，肉质很嫩，强烈推荐！", baseTime.addSecs(-5 * 3600)};
    comments << CommentModel{2, "今天土豆丝盐放得有点多，希望改进。", baseTime.addSecs(-2 * 3600)};
    comments << CommentModel{5, "抹茶冰淇淋特别浓郁，满分好评！", baseTime.addSecs(-30 * 60)};

    if (index == 0) {
        // 规则 0：按评分从高到低排序
        std::sort(comments.begin(), comments.end(), [](const CommentModel &a, const CommentModel &b) {
            return a.score > b.score;
        });
    } else if (index == 1) {
        // 规则 1：按时间先后（由远及近）排序
        std::sort(comments.begin(), comments.end(), [](const CommentModel &a, const CommentModel &b) {
            return a.time < b.time;
        });
    }
    return comments;
}