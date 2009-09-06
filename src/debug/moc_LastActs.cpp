/****************************************************************************
** Meta object code from reading C++ file 'LastActs.h'
**
** Created: Sun Sep 6 18:02:13 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../LastActs.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LastActs.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_LastActs[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      27,   10,    9,    9, 0x09,
      58,   51,    9,    9, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_LastActs[] = {
    "LastActs\0\0_act,_setCurrent\0"
    "actAdded(Activity,bool)\0_today\0"
    "todayChanged(QDate)\0"
};

const QMetaObject LastActs::staticMetaObject = {
    { &QAbstractTableModel::staticMetaObject, qt_meta_stringdata_LastActs,
      qt_meta_data_LastActs, 0 }
};

const QMetaObject *LastActs::metaObject() const
{
    return &staticMetaObject;
}

void *LastActs::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_LastActs))
        return static_cast<void*>(const_cast< LastActs*>(this));
    return QAbstractTableModel::qt_metacast(_clname);
}

int LastActs::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractTableModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: actAdded((*reinterpret_cast< const Activity(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 1: todayChanged((*reinterpret_cast< const QDate(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
