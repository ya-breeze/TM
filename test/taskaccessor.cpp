#include "taskaccessor.h"

TaskAccessor::TaskAccessor()
{
}

Task::TaskList TaskAccessor::getTaskList(const QUuid& _parent) {
    return Task::TaskList();
}

void TaskAccessor::addTask(const Task& _task) {
    m_TaskCache.remove(_task.getUuid());
    m_Db.addTask(_task);
}
