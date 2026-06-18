//菜品与菜单模型
#include "dish.h"
#include <algorithm>

DishManager::DishManager() {
    // 固定初始化菜品，其中 5 个标记为 Top 5 爆款推荐
    //构造函数通过 Qt 的流操作符 << 向核心数据容器 QList<DishModel> m_dishes 中批量注入预置的菜单数据
    m_dishes << DishModel{"招牌红烧肉", "精选主食", 48.0, "肥而不腻，入口即化", true};
    m_dishes << DishModel{"清蒸鲈鱼", "精选主食", 58.0, "鲜美嫩滑，营养丰富", true};
    m_dishes << DishModel{"宫保鸡丁", "精选主食", 28.0, "酸辣适口，经典下饭", true};
    m_dishes << DishModel{"经典波霸奶茶", "即调饮品", 15.0, "浓郁奶香，Q弹珍珠", true};
    m_dishes << DishModel{"抹茶冰淇淋", "精美甜点", 18.0, "纯正抹茶，清甜解腻", true};
    m_dishes << DishModel{"清炒时蔬", "家常热菜", 16.0, "清脆爽口，少油健康", false};
    m_dishes << DishModel{"酸辣土豆丝", "家常热菜", 12.0, "口感爽脆，酸辣开胃", false};
}


//单例设计模式（Singleton Pattern） 的标准 C++ 实现。
//利用函数内部的静态局部变量 static DishManager inst;，
//确保在整个应用程序的生命周期内，该管理器有且仅有一个实例被初始化。
//这避免了菜单在多处被重复加载带来的内存开销，并为整个系统提供了一个高内聚、全局唯一的电子菜单数据源
DishManager& DishManager::instance() {
    static DishManager inst;
    return inst;
}

//数据提供接口（Getter）。将内部私有的菜品列表封装后完整拷贝返回给控制层
QList<DishModel> DishManager::getDishes() {
    return m_dishes;
}

//通过 if (dish.name == name) 线性比对字符串，匹配成功则立刻将对应的标准原价 dish.price 返回；
//若遍历结束仍未找到该菜品，则返回安全兜底边界值 0.0
double DishManager::getDishPrice(const QString &name) {
    for (const auto &dish : m_dishes) {
        if (dish.name == name) return dish.price;
    }
    return 0.0;
}


//动态构建评价看板数据集
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