/****************************************************************************
** Meta object code from reading C++ file 'Activities.h'
**
** Created: Sun Sep 6 18:02:14 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Activities.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Activities.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Activities[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      29,   12,   11,   11, 0x05,
      60,   53,   11,   11, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_Activities[] = {
    "Activities\0\0_act,_setCurrent\0"
    "ActAdded(Activity,bool)\0_today\0"
    "todayChanged(QDate)\0"
};

const QMetaObject Activities::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Activities,
      qt_meta_data_Activities, 0 }
};

const QMetaObject *Activities::metaObject() const
{
    return &staticMetaObject;
}

void *Activities::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Activities))
        return static_cast<void*>(const_cast< Activities*>(this));
    return QObject::qt_metacast(_clname);
}

int Activities::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: ActAdded((*reinterpret_cast< const Activity(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 1: todayChanged((*reinterpret_cast< const QDate(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void Activities::ActAdded(const Activity & _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Activities::todayChanged(const QDate & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
