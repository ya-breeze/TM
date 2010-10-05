#include "dbaccess.h"

#include <stdexcept>

#include <QVariant>
#include <QSqlError>
#include <QSqlQuery>
#include <QProcessEnvironment>

DbAccess::DbAccess()
{
    // TODO check errors
    m_Db = QSqlDatabase::addDatabase("QSQLITE");
    m_Db.setDatabaseName( QProcessEnvironment::systemEnvironment().value("HOME", "/tmp") + "/.tm.sqlite" );
    //m_Db.setDatabaseName(":memory:");
    
    if( !m_Db.open() )
        throw std::runtime_error(m_Db.lastError().text().toStdString());

    QSqlQuery query;
    if( !query.exec("CREATE TABLE IF NOT EXISTS Tasks(uuid text, parentUuid text, name text);") )
        throw std::runtime_error(m_Db.lastError().text().toStdString());
}

Task::TaskList DbAccess::getTaskList(const QUuid& _parent) {
    Task::TaskList result;

    QSqlQuery query;
    query.prepare("SELECT uuid, name FROM Tasks WHERE parentUuid=?;");
    query.addBindValue(QVariant(_parent.toString()));
    if( !query.exec() )
        throw std::runtime_error(m_Db.lastError().text().toStdString());
    while (query.next()) {
        QString uuid = query.value(0).toString();
        QString name = query.value(1).toString();
//        qDebug() << uuid << name;
    }

    return result;
}

void DbAccess::addTask(const Task& _task) {
    QSqlQuery query;
    query.prepare("INSERT INTO Tasks(uuid, parentUuid, name) VALUES(?, ?, ?);");
    query.addBindValue(_task.getUuid().toString());
    query.addBindValue(_task.getParentUuid().toString());
    query.addBindValue(_task.getName());

    if( !query.exec() )
        throw std::runtime_error(m_Db.lastError().text().toStdString());
}
