/****************************************************************************
** Meta object code from reading C++ file 'TimeScale.h'
**
** Created: Sun Sep 6 18:02:09 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../TimeScale.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TimeScale.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TimeScale[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

       0        // eod
};

static const char qt_meta_stringdata_TimeScale[] = {
    "TimeScale\0"
};

const QMetaObject TimeScale::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_TimeScale,
      qt_meta_data_TimeScale, 0 }
};

const QMetaObject *TimeScale::metaObject() const
{
    return &staticMetaObject;
}

void *TimeScale::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TimeScale))
        return static_cast<void*>(const_cast< TimeScale*>(this));
    return QObject::qt_metacast(_clname);
}

int TimeScale::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
