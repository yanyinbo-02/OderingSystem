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
    QByteArrayData data[15];
    char stringdata0[321];
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
QT_MOC_LITERAL(7, 109, 28), // "on_btnCalculatePrice_clicked"
QT_MOC_LITERAL(8, 138, 25), // "on_btnSubmitOrder_clicked"
QT_MOC_LITERAL(9, 164, 39), // "on_comboCommentSort_currentIn..."
QT_MOC_LITERAL(10, 204, 27), // "on_copyHistoryOrder_clicked"
QT_MOC_LITERAL(11, 232, 14), // "refreshQueueUI"
QT_MOC_LITERAL(12, 247, 26), // "on_btnRefreshQueue_clicked"
QT_MOC_LITERAL(13, 274, 23), // "on_btnJoinQueue_clicked"
QT_MOC_LITERAL(14, 298, 22) // "on_btnCallNext_clicked"

    },
    "MainWindow\0on_sidebarNav_currentRowChanged\0"
    "\0row\0on_tabWidgetQueueType_currentChanged\0"
    "index\0refreshDishMenuUI\0"
    "on_btnCalculatePrice_clicked\0"
    "on_btnSubmitOrder_clicked\0"
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
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   69,    2, 0x08 /* Private */,
       4,    1,   72,    2, 0x08 /* Private */,
       6,    0,   75,    2, 0x08 /* Private */,
       7,    0,   76,    2, 0x08 /* Private */,
       8,    0,   77,    2, 0x08 /* Private */,
       9,    1,   78,    2, 0x08 /* Private */,
      10,    0,   81,    2, 0x08 /* Private */,
      11,    0,   82,    2, 0x08 /* Private */,
      12,    0,   83,    2, 0x08 /* Private */,
      13,    0,   84,    2, 0x08 /* Private */,
      14,    0,   85,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void,
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
        case 3: _t->on_btnCalculatePrice_clicked(); break;
        case 4: _t->on_btnSubmitOrder_clicked(); break;
        case 5: _t->on_comboCommentSort_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->on_copyHistoryOrder_clicked(); break;
        case 7: _t->refreshQueueUI(); break;
        case 8: _t->on_btnRefreshQueue_clicked(); break;
        case 9: _t->on_btnJoinQueue_clicked(); break;
        case 10: _t->on_btnCallNext_clicked(); break;
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
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
