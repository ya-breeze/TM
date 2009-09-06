/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Sun Sep 6 18:02:16 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TM[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      26,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
       4,    3,    3,    3, 0x0a,
      19,    3,    3,    3, 0x0a,
      41,    3,    3,    3, 0x0a,
      64,   56,    3,    3, 0x0a,
     111,  101,    3,    3, 0x0a,
     153,    3,    3,    3, 0x0a,
     165,    3,    3,    3, 0x0a,
     180,    3,    3,    3, 0x0a,
     201,    3,    3,    3, 0x0a,
     222,    3,    3,    3, 0x0a,
     249,    3,    3,    3, 0x0a,
     275,    3,    3,    3, 0x0a,
     294,    3,    3,    3, 0x0a,
     314,    3,    3,    3, 0x0a,
     337,    3,    3,    3, 0x0a,
     355,    3,    3,    3, 0x0a,
     385,  376,    3,    3, 0x0a,
     419,    3,    3,    3, 0x0a,
     438,    3,    3,    3, 0x0a,
     458,    3,    3,    3, 0x0a,
     478,    3,    3,    3, 0x0a,
     494,    3,    3,    3, 0x0a,
     508,    3,    3,    3, 0x0a,
     524,    3,    3,    3, 0x0a,
     540,    3,    3,    3, 0x0a,
     557,    3,    3,    3, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_TM[] = {
    "TM\0\0slot_AddItem()\0slot_AddSiblingItem()\0"
    "slot_DelItem()\0old,now\0"
    "slot_FocusChanged(QWidget*,QWidget*)\0"
    "_new,_old\0slot_TaskChanged(QModelIndex,QModelIndex)\0"
    "slot_Save()\0slot_Restore()\0"
    "slot_SetFocusTasks()\0slot_SetFocusNotes()\0"
    "slot_SetFocusAddActivity()\0"
    "slot_SetFocusActivities()\0slot_AddActivity()\0"
    "slot_ActivityType()\0slot_CurrentActivity()\0"
    "slot_BtnToTasks()\0slot_BtnUpdateTime()\0"
    "_current\0slot_SelectedLastAct(QModelIndex)\0"
    "slot_SetFinished()\0slot_SetStartTime()\0"
    "slot_AddInterrupt()\0slot_HideDone()\0"
    "slot_MoveUp()\0slot_MoveDown()\0"
    "slot_MoveLeft()\0slot_MoveRight()\0"
    "slot_Sync()\0"
};

const QMetaObject TM::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_TM,
      qt_meta_data_TM, 0 }
};

const QMetaObject *TM::metaObject() const
{
    return &staticMetaObject;
}

void *TM::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TM))
        return static_cast<void*>(const_cast< TM*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int TM::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: slot_AddItem(); break;
        case 1: slot_AddSiblingItem(); break;
        case 2: slot_DelItem(); break;
        case 3: slot_FocusChanged((*reinterpret_cast< QWidget*(*)>(_a[1])),(*reinterpret_cast< QWidget*(*)>(_a[2]))); break;
        case 4: slot_TaskChanged((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< const QModelIndex(*)>(_a[2]))); break;
        case 5: slot_Save(); break;
        case 6: slot_Restore(); break;
        case 7: slot_SetFocusTasks(); break;
        case 8: slot_SetFocusNotes(); break;
        case 9: slot_SetFocusAddActivity(); break;
        case 10: slot_SetFocusActivities(); break;
        case 11: slot_AddActivity(); break;
        case 12: slot_ActivityType(); break;
        case 13: slot_CurrentActivity(); break;
        case 14: slot_BtnToTasks(); break;
        case 15: slot_BtnUpdateTime(); break;
        case 16: slot_SelectedLastAct((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 17: slot_SetFinished(); break;
        case 18: slot_SetStartTime(); break;
        case 19: slot_AddInterrupt(); break;
        case 20: slot_HideDone(); break;
        case 21: slot_MoveUp(); break;
        case 22: slot_MoveDown(); break;
        case 23: slot_MoveLeft(); break;
        case 24: slot_MoveRight(); break;
        case 25: slot_Sync(); break;
        default: ;
        }
        _id -= 26;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
