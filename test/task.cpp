#include "task.h"

Task::Task()
    : m_Uuid( QUuid::createUuid() )
{
}

QUuid Task::getUuid() const {
    return m_Uuid;
}
void Task::setUuid(const QUuid& _uuid) {
    m_Uuid = _uuid;
}

QUuid Task::getParentUuid() const {
    return m_ParentUuid;
}
void Task::setParentUuid(const QUuid& _uuid) {
    m_ParentUuid = _uuid;
}

QString Task::getName() const {
    return str_Name;
}

void Task::setName(const QString& _name) {
    str_Name = _name;
}
