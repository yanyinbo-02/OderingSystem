/* Minimal ui_mainwindow.h to satisfy compile-time references.
   This provides the widgets used by MainWindow without changing logic.
   Generated manually to complement the .ui which currently lacks these widgets. */

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTabWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    QTableWidget *tableWidgetMenu;
    QTableWidget *tableWidgetQueue;
    QListWidget *listWidgetHistory;
    QListWidget *listWidgetComments;
    QLineEdit *lineEditMemberId;
    QLabel *lblFinalPrice;
    QLabel *lblQueueCount;
    QTabWidget *tabWidgetQueueType;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        MainWindow->setCentralWidget(centralwidget);

        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        MainWindow->setMenuBar(menubar);

        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        // Minimal widgets used by MainWindow logic
        tableWidgetMenu = new QTableWidget(centralwidget);
        tableWidgetMenu->setObjectName(QString::fromUtf8("tableWidgetMenu"));

        tableWidgetQueue = new QTableWidget(centralwidget);
        tableWidgetQueue->setObjectName(QString::fromUtf8("tableWidgetQueue"));

        listWidgetHistory = new QListWidget(centralwidget);
        listWidgetHistory->setObjectName(QString::fromUtf8("listWidgetHistory"));

        listWidgetComments = new QListWidget(centralwidget);
        listWidgetComments->setObjectName(QString::fromUtf8("listWidgetComments"));

        lineEditMemberId = new QLineEdit(centralwidget);
        lineEditMemberId->setObjectName(QString::fromUtf8("lineEditMemberId"));

        lblFinalPrice = new QLabel(centralwidget);
        lblFinalPrice->setObjectName(QString::fromUtf8("lblFinalPrice"));

        lblQueueCount = new QLabel(centralwidget);
        lblQueueCount->setObjectName(QString::fromUtf8("lblQueueCount"));

        tabWidgetQueueType = new QTabWidget(centralwidget);
        tabWidgetQueueType->setObjectName(QString::fromUtf8("tabWidgetQueueType"));

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    }

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
    }
};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
}

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
