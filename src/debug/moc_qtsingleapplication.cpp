/****************************************************************************
** Meta object code from reading C++ file 'qtsingleapplication.h'
**
** Created: Sun Sep 6 18:02:12 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../qtsingleapp/qtsingleapplication.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qtsingleapplication.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtSingleApplication[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      29,   21,   20,   20, 0x05,

 // slots: signature, parameters, type, tag, flags
      75,   59,   54,   20, 0x0a,
     100,   21,   54,   20, 0x2a,
     121,   20,   20,   20, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QtSingleApplication[] = {
    "QtSingleApplication\0\0message\0"
    "messageReceived(QString)\0bool\0"
    "message,timeout\0sendMessage(QString,int)\0"
    "sendMessage(QString)\0activateWindow()\0"
};

const QMetaObject QtSingleApplication::staticMetaObject = {
    { &QApplication::staticMetaObject, qt_meta_stringdata_QtSingleApplication,
      qt_meta_data_QtSingleApplication, 0 }
};

const QMetaObject *QtSingleApplication::metaObject() const
{
    return &staticMetaObject;
}

void *QtSingleApplication::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtSingleApplication))
        return static_cast<void*>(const_cast< QtSingleApplication*>(this));
    return QApplication::qt_metacast(_clname);
}

int QtSingleApplication::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QApplication::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: messageReceived((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: { bool _r = sendMessage((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 2: { bool _r = sendMessage((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 3: activateWindow(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void QtSingleApplication::messageReceived(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
