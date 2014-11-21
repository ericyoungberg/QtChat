/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "gui/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[18];
    char stringdata[234];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10),
QT_MOC_LITERAL(1, 11, 11),
QT_MOC_LITERAL(2, 23, 0),
QT_MOC_LITERAL(3, 24, 15),
QT_MOC_LITERAL(4, 40, 5),
QT_MOC_LITERAL(5, 46, 5),
QT_MOC_LITERAL(6, 52, 14),
QT_MOC_LITERAL(7, 67, 11),
QT_MOC_LITERAL(8, 79, 17),
QT_MOC_LITERAL(9, 97, 11),
QT_MOC_LITERAL(10, 109, 21),
QT_MOC_LITERAL(11, 131, 17),
QT_MOC_LITERAL(12, 149, 16),
QT_MOC_LITERAL(13, 166, 13),
QT_MOC_LITERAL(14, 180, 17),
QT_MOC_LITERAL(15, 198, 7),
QT_MOC_LITERAL(16, 206, 12),
QT_MOC_LITERAL(17, 219, 14)
    },
    "MainWindow\0queueRouter\0\0receivedMessage\0"
    "char*\0route\0showAddContact\0sendMessage\0"
    "addUserToContacts\0userAddress\0"
    "removeSelectedContact\0startConversation\0"
    "QListWidgetItem*\0deleteHistory\0"
    "closeConversation\0quitApp\0openSettings\0"
    "setEnvironment"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   79,    2, 0x06 /* Public */,
       3,    2,   84,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    2,   89,    2, 0x0a /* Public */,
       6,    0,   94,    2, 0x08 /* Private */,
       7,    0,   95,    2, 0x08 /* Private */,
       8,    1,   96,    2, 0x08 /* Private */,
      10,    0,   99,    2, 0x08 /* Private */,
      11,    1,  100,    2, 0x08 /* Private */,
      13,    0,  103,    2, 0x08 /* Private */,
      14,    0,  104,    2, 0x08 /* Private */,
      15,    0,  105,    2, 0x08 /* Private */,
      16,    0,  106,    2, 0x08 /* Private */,
      17,    0,  107,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    2,    2,
    QMetaType::Void, 0x80000000 | 4, 0x80000000 | 4,    2,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    2,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 12,    2,
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
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->queueRouter((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->receivedMessage((*reinterpret_cast< char*(*)>(_a[1])),(*reinterpret_cast< char*(*)>(_a[2]))); break;
        case 2: _t->route((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 3: _t->showAddContact(); break;
        case 4: _t->sendMessage(); break;
        case 5: _t->addUserToContacts((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->removeSelectedContact(); break;
        case 7: _t->startConversation((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 8: _t->deleteHistory(); break;
        case 9: _t->closeConversation(); break;
        case 10: _t->quitApp(); break;
        case 11: _t->openSettings(); break;
        case 12: _t->setEnvironment(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MainWindow::*_t)(QString , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::queueRouter)) {
                *result = 0;
            }
        }
        {
            typedef void (MainWindow::*_t)(char * , char * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::receivedMessage)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, 0, 0}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::queueRouter(QString _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MainWindow::receivedMessage(char * _t1, char * _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
