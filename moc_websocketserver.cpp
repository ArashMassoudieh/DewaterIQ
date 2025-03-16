/****************************************************************************
** Meta object code from reading C++ file 'websocketserver.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "websocketserver.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'websocketserver.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
namespace {
struct qt_meta_stringdata_WebSocketServer_t {
    uint offsetsAndSizes[18];
    char stringdata0[16];
    char stringdata1[16];
    char stringdata2[1];
    char stringdata3[22];
    char stringdata4[8];
    char stringdata5[21];
    char stringdata6[20];
    char stringdata7[12];
    char stringdata8[7];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_WebSocketServer_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_WebSocketServer_t qt_meta_stringdata_WebSocketServer = {
    {
        QT_MOC_LITERAL(0, 15),  // "WebSocketServer"
        QT_MOC_LITERAL(16, 15),  // "onNewConnection"
        QT_MOC_LITERAL(32, 0),  // ""
        QT_MOC_LITERAL(33, 21),  // "onTextMessageReceived"
        QT_MOC_LITERAL(55, 7),  // "message"
        QT_MOC_LITERAL(63, 20),  // "onSocketDisconnected"
        QT_MOC_LITERAL(84, 19),  // "sendMessageToClient"
        QT_MOC_LITERAL(104, 11),  // "QWebSocket*"
        QT_MOC_LITERAL(116, 6)   // "client"
    },
    "WebSocketServer",
    "onNewConnection",
    "",
    "onTextMessageReceived",
    "message",
    "onSocketDisconnected",
    "sendMessageToClient",
    "QWebSocket*",
    "client"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_WebSocketServer[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   38,    2, 0x08,    1 /* Private */,
       3,    1,   39,    2, 0x08,    2 /* Private */,
       5,    0,   42,    2, 0x08,    4 /* Private */,
       6,    2,   43,    2, 0x08,    5 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7, QMetaType::QString,    8,    4,

       0        // eod
};

Q_CONSTINIT const QMetaObject WebSocketServer::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_WebSocketServer.offsetsAndSizes,
    qt_meta_data_WebSocketServer,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_WebSocketServer_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<WebSocketServer, std::true_type>,
        // method 'onNewConnection'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onTextMessageReceived'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'onSocketDisconnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'sendMessageToClient'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QWebSocket *, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>
    >,
    nullptr
} };

void WebSocketServer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<WebSocketServer *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onNewConnection(); break;
        case 1: _t->onTextMessageReceived((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->onSocketDisconnected(); break;
        case 3: _t->sendMessageToClient((*reinterpret_cast< std::add_pointer_t<QWebSocket*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QWebSocket* >(); break;
            }
            break;
        }
    }
}

const QMetaObject *WebSocketServer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WebSocketServer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_WebSocketServer.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int WebSocketServer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
