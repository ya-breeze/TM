#ifndef DBACCESS_H
#define DBACCESS_H

#include <QSqlDatabase>

#include "task.h"

class DbAccess
{
    QSqlDatabase m_Db;
public:
    DbAccess();
    Task::TaskList  getTaskList(const QUuid& _parent);
    void            addTask(const Task& _task);
};

#endif // DBACCESS_H
