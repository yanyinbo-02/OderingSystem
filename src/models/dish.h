/// 菜品与菜单模型
#ifndef DISH_H
#define DISH_H

#include <QString>
#include <QList>
#include "../controllers/system_engine.h"

class DishManager {
public:
    static DishManager& instance();
    
    // 【关键适配修正】提供静态桥接函数，直接适配 system_engine.cpp 内部的底层调用
    static QList<DishModel> getFixedDishes() {
        return instance().getDishes();
    }
    
    QList<DishModel> getDishes();
    double getDishPrice(const QString &name);
    QList<CommentModel> getSortedComments(int index);

private:
    DishManager();
    QList<DishModel> m_dishes;
};

#endif // DISH_H