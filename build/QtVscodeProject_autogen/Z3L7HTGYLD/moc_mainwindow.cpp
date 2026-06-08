/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/views/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[21];
    char stringdata0[456];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 31), // "on_sidebarNav_currentRowChanged"
QT_MOC_LITERAL(2, 43, 0), // ""
QT_MOC_LITERAL(3, 44, 3), // "row"
QT_MOC_LITERAL(4, 48, 36), // "on_tabWidgetQueueType_current..."
QT_MOC_LITERAL(5, 85, 5), // "index"
QT_MOC_LITERAL(6, 91, 17), // "refreshDishMenuUI"
QT_MOC_LITERAL(7, 109, 23), // "on_btnAddToCart_clicked"
QT_MOC_LITERAL(8, 133, 36), // "on_tableWidgetMenu_cellDouble..."
QT_MOC_LITERAL(9, 170, 6), // "column"
QT_MOC_LITERAL(10, 177, 36), // "on_tableWidgetCart_cellDouble..."
QT_MOC_LITERAL(11, 214, 31), // "on_lineEditMemberId_textChanged"
QT_MOC_LITERAL(12, 246, 4), // "text"
QT_MOC_LITERAL(13, 251, 21), // "recalculateCartPrices"
QT_MOC_LITERAL(14, 273, 25), // "on_btnSubmitOrder_clicked"
QT_MOC_LITERAL(15, 299, 39), // "on_comboCommentSort_currentIn..."
QT_MOC_LITERAL(16, 339, 27), // "on_copyHistoryOrder_clicked"
QT_MOC_LITERAL(17, 367, 14), // "refreshQueueUI"
QT_MOC_LITERAL(18, 382, 26), // "on_btnRefreshQueue_clicked"
QT_MOC_LITERAL(19, 409, 23), // "on_btnJoinQueue_clicked"
QT_MOC_LITERAL(20, 433, 22) // "on_btnCallNext_clicked"

    },
    "MainWindow\0on_sidebarNav_currentRowChanged\0"
    "\0row\0on_tabWidgetQueueType_currentChanged\0"
    "index\0refreshDishMenuUI\0on_btnAddToCart_clicked\0"
    "on_tableWidgetMenu_cellDoubleClicked\0"
    "column\0on_tableWidgetCart_cellDoubleClicked\0"
    "on_lineEditMemberId_textChanged\0text\0"
    "recalculateCartPrices\0on_btnSubmitOrder_clicked\0"
    "on_comboCommentSort_currentIndexChanged\0"
    "on_copyHistoryOrder_clicked\0refreshQueueUI\0"
    "on_btnRefreshQueue_clicked\0"
    "on_btnJoinQueue_clicked\0on_btnCallNext_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   89,    2, 0x08 /* Private */,
       4,    1,   92,    2, 0x08 /* Private */,
       6,    0,   95,    2, 0x08 /* Private */,
       7,    0,   96,    2, 0x08 /* Private */,
       8,    2,   97,    2, 0x08 /* Private */,
      10,    2,  102,    2, 0x08 /* Private */,
      11,    1,  107,    2, 0x08 /* Private */,
      13,    0,  110,    2, 0x08 /* Private */,
      14,    0,  111,    2, 0x08 /* Private */,
      15,    1,  112,    2, 0x08 /* Private */,
      16,    0,  115,    2, 0x08 /* Private */,
      17,    0,  116,    2, 0x08 /* Private */,
      18,    0,  117,    2, 0x08 /* Private */,
      19,    0,  118,    2, 0x08 /* Private */,
      20,    0,  119,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    3,    9,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    3,    9,
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_sidebarNav_currentRowChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->on_tabWidgetQueueType_currentChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->refreshDishMenuUI(); break;
        case 3: _t->on_btnAddToCart_clicked(); break;
        case 4: _t->on_tableWidgetMenu_cellDoubleClicked((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->on_tableWidgetCart_cellDoubleClicked((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 6: _t->on_lineEditMemberId_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->recalculateCartPrices(); break;
        case 8: _t->on_btnSubmitOrder_clicked(); break;
        case 9: _t->on_comboCommentSort_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->on_copyHistoryOrder_clicked(); break;
        case 11: _t->refreshQueueUI(); break;
        case 12: _t->on_btnRefreshQueue_clicked(); break;
        case 13: _t->on_btnJoinQueue_clicked(); break;
        case 14: _t->on_btnCallNext_clicked(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 15;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
