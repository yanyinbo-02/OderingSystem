/// 菜品与菜单模型
#ifndef DISH_H
#define DISH_H

#include <QString>
#include <QList>
#include "../controllers/system_engine.h"

class DishManager {
public:
    static DishManager& instance();
    
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