#ifndef TASK_H
#define TASK_H

#include <QUuid>
#include <QList>
#include <QString>

class Task
{
    QUuid   m_Uuid;
    QUuid   m_ParentUuid;
    QString str_Name;

public:
    typedef QList<Task> TaskList;

    Task();

    QUuid   getUuid() const;
    void    setUuid(const QUuid& _uuid);
    QUuid   getParentUuid() const;
    void    setParentUuid(const QUuid& _uuid);

    QString getName() const;
    void    setName(const QString& _name);
};

#endif // TASK_H
