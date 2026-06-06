//qt提供的应用程序头文件，引入QApplication类
#include <QApplication>
//引入用户自定义的主窗口类
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    //应用程序对象，有且仅有一个
    //它承载了整个应用程序的底层初始化、事件循环（Event Loop）的分发、
    //系统级别设置（如字体、本地化、双击间隔）以及屏幕与窗口的管理。
    QApplication a(argc, argv);
    //定义了一个窗口对象
    MainWindow w;
    //让窗口显式出来
    w.show();
    //应用程序事件循环
    return a.exec();
}