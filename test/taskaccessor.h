#ifndef TASKACCESSOR_H
#define TASKACCESSOR_H

#include <QMap>

#include "task.h"
#include "dbaccess.h"

class TaskAccessor
{
    typedef QMap<QUuid, Task>   TaskMap;
    TaskMap     m_TaskCache;
    DbAccess    m_Db;

public:
    TaskAccessor();

    Task::TaskList  getTaskList(const QUuid& _parent);
    void            addTask(const Task& _task);
};

#endif // TASKACCESSOR_H
