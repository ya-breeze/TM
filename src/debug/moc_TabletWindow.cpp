/****************************************************************************
** Meta object code from reading C++ file 'TabletWindow.h'
**
** Created: Sun Sep 6 18:02:11 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../TabletWindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TabletWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TabletWindow[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x0a,
      35,   13,   13,   13, 0x0a,
      57,   13,   13,   13, 0x0a,
      84,   13,   13,   13, 0x0a,
     102,   13,   13,   13, 0x0a,
     122,   13,   13,   13, 0x0a,
     144,   13,   13,   13, 0x0a,
     159,   13,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_TabletWindow[] = {
    "TabletWindow\0\0slot_SetFocusTasks()\0"
    "slot_SetFocusChrono()\0slot_SetFocusAddActivity()\0"
    "slot_TaskDelete()\0slot_TaskAddChild()\0"
    "slot_TaskAddSibling()\0slot_Restore()\0"
    "slot_Save()\0"
};

const QMetaObject TabletWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_TabletWindow,
      qt_meta_data_TabletWindow, 0 }
};

const QMetaObject *TabletWindow::metaObject() const
{
    return &staticMetaObject;
}

void *TabletWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TabletWindow))
        return static_cast<void*>(const_cast< TabletWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int TabletWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: slot_SetFocusTasks(); break;
        case 1: slot_SetFocusChrono(); break;
        case 2: slot_SetFocusAddActivity(); break;
        case 3: slot_TaskDelete(); break;
        case 4: slot_TaskAddChild(); break;
        case 5: slot_TaskAddSibling(); break;
        case 6: slot_Restore(); break;
        case 7: slot_Save(); break;
        default: ;
        }
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
