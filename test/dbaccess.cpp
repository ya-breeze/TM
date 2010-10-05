#include "dbaccess.h"

#include <stdexcept>

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
    return Task::TaskList();
}

void DbAccess::addTask(const Task& _task) {
    QSqlQuery query;
    query.exec("SELECT name, salary FROM employee WHERE salary > 50000");
}
