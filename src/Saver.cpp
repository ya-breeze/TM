/*
 * Saver.cpp
 *
 *  Created on: 21.08.2009
 *      Author: breeze
 */

#include "Saver.h"

#include <stdlib.h>
#include <QFile>
#include <QDir>
#include <QApplication>
#include <QProcessEnvironment>
#include <QVariant>
#include <QSqlError>
#include <QSqlQuery>
#include <QBuffer>

#include "utils.h"
#include "TaskTree.h"


#define FNAME_TASKS	"Tasks"
#define FNAME_ACTS	"Activities."

Saver::Saver()
    : inTransaction(false ) {
}

void Saver::init() {
    QSqlDatabase::removeDatabase("tmDatabase");
    m_Db = QSqlDatabase::addDatabase("QSQLITE", "tmDatabase");
    m_Db.setDatabaseName( QProcessEnvironment::systemEnvironment().value("HOME", "/tmp") + "/.tm.sqlite" );
    if( !m_Db.open() )
	throw std::runtime_error(m_Db.lastError().text().toStdString());

    Transaction<Saver> t(*this);

    QSqlQuery query(m_Db);
    if( !query.exec("CREATE TABLE IF NOT EXISTS Tasks(uuid text primary key, parentUuid text, name text,"
		    "notes text, created integer, localUpdated integer, globalUpdated integer,"
		    "started integer, finished integer, planned text, parentIndex integer, categories text"
		    "iconName text);") )
	throw std::runtime_error(query.lastError().text().toStdString());


    if( !query.exec("CREATE TABLE IF NOT EXISTS Activities(uuid text primary key, taskUuid text, name text,"
		    "startTime integer, localUpdated integer, globalUpdated integer);") )
	throw std::runtime_error(query.lastError().text().toStdString());

    if( !query.exec("CREATE TABLE IF NOT EXISTS Hosts(uuid text primary key, lastUpdated integer);") )
	throw std::runtime_error(query.lastError().text().toStdString());

    if( !query.exec("CREATE TABLE IF NOT EXISTS Icons(uuid text primary key, name text, body blob);") )
	throw std::runtime_error(query.lastError().text().toStdString());

    query.finish();

    t.commit();
}


Saver::~Saver() {
    if(inTransaction) rollback();
    m_Db.close();
}


QString	Saver::getHome() const
{
	char *home = ::getenv("HOME");
	if( home==NULL)
		return "/tmp";

	return QString(home) + "/.TM/";
}

QString Saver::escapeString(const QString& _str)
{
	QString res;

//	bool someLines = false;
//	int idx1 = 0;
//	int idx2 = _str.indexOf('\n');
//	while( idx2!=-1 )
//	{
//		someLines = true;
//		res += _str.mid(idx1, idx2-idx1);
//		res += ' ';
//		idx1 = idx2+1;
//		idx2 = _str.indexOf('\n', idx2+1);
//	}
//	res += _str.mid(idx1, idx2-idx1);

	res = _str;
	res.replace("\n", "\n ");

	return res;
}

void Saver::saveTask(const Task& _task)
{
	QString fname = getHome() + FNAME_TASKS;
	std::ofstream file((fname + "." + _task.getId()).toUtf8().data(), std::ios::trunc);
	if( !file )
		ERROR("Unable to open file '" << fname << "'");

	//	DEBUG(_task.getName() << ":'" << escapeString(_task.getNotes()) << "'");
	file << "BEGIN:VTODO" << std::endl
		<< "ID:" << _task.getId().toString().toStdString() << std::endl
		<< "Name:" << _task.getName().toUtf8().data() << std::endl
		<< "Notes:" << escapeString(_task.getNotes()).toUtf8().data() << std::endl
		<< "Parent:" << _task.getParentId().toString() << std::endl
		<< "DateCreated:" << _task.getCreated().toString(Qt::ISODate) << std::endl
		<< "DateStarted:" << _task.getStarted().toString(Qt::ISODate) << std::endl
		<< "DateFinished:" << _task.getFinished().toString(Qt::ISODate) << std::endl
		<< "PlannedTime:" << _task.getPlannedTime().toUtf8().data() << std::endl
		<< "Categories:" << _task.getCategories().join(";").toUtf8().data() << std::endl
		<< "END:VTODO" << std::endl;
}

void Saver::saveDbTask(const Task& _task)
{
//	DEBUG(_task.getName());
	TimeItem ti;
	ti.start();

	// TODO Вынести prepare в конструктор
	QSqlQuery query(m_Db);
	query.prepare("REPLACE INTO Tasks(uuid, parentUuid, name, notes, created, localUpdated,"
		      "globalUpdated, started, finished, planned, parentIndex, categories, iconName)"
		      "VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
//	DEBUG("SQL prepare time " << ti.update());
	query.addBindValue(_task.getId().toString());
	query.addBindValue(_task.getParentId().toString());
	query.addBindValue(_task.getName());
	query.addBindValue(_task.getNotes());
	query.addBindValue(_task.getCreated().toTime_t());
	query.addBindValue(_task.getLocalUpdated().toTime_t());
	query.addBindValue(_task.getGlobalUpdated().toTime_t());
	query.addBindValue(_task.getStarted().isNull() ? 0 : _task.getStarted().toTime_t());
	query.addBindValue(_task.getFinished().isNull() ? 0 : _task.getFinished().toTime_t());
	query.addBindValue(_task.getPlannedTime());
	query.addBindValue(_task.getParentIndex());
	query.addBindValue(_task.getCategories().join(";"));
	query.addBindValue(_task.getIconName());

//	DEBUG("SQL add binds time " << ti.update());

	if( !query.exec() )
	    throw std::runtime_error(query.lastError().text().toStdString());
//	DEBUG("SQL exec time " << ti.update());
	query.finish();
}

void Saver::saveDbRecurse(TaskTree& _tree, const QModelIndex& _idx) {
    size_t sz = _tree.rowCount(_idx);
    for(size_t i=0;i<sz;++i)
    {
	    QModelIndex idx = _tree.index(i, 0, _idx);
	    const TaskItem *item = _tree.getItem(idx);
	    Q_ASSERT(item);
	    saveDbTask(*item);

	    if( _tree.columnCount(idx) )
		saveDbRecurse(_tree, idx);
    }
}

void Saver::recurseSave(const TaskTree& _tree, const QModelIndex& _idx)
{
	size_t sz = _tree.rowCount(_idx);
	for(size_t i=0;i<sz;++i)
	{
		QModelIndex idx = _tree.index(i, 0, _idx);
		const TaskItem *item = _tree.getItem(idx);
		Q_ASSERT(item);
		saveTask(*item);

		if( _tree.columnCount(idx) )
			recurseSave(_tree, idx);
	}
}

//Saver::TaskList Saver::getTasks(const QModelIndex& _idx) {
//    TaskList result;

//    size_t sz = tree->rowCount(_idx);
//    for(size_t i=0;i<sz;++i)
//    {
//	    QModelIndex idx = tree->index(i, 0, _idx);
//	    const TaskItem *item = tree->getItem(idx);
//	    Q_ASSERT(item);
//	    result.push_back(item);
//    }

//    return result;
//}

void Saver::saveDb(TaskTree& _tree) {
    TimeItem ti;
    ti.start();
    DEBUG("Saving tasks in db...");
    Transaction<Saver> t(*this);

    DEBUG("Deleting...");
    QSqlQuery query(m_Db);
    if( !query.exec("DELETE FROM Tasks;") )
	throw std::runtime_error(query.lastError().text().toStdString());
    if( !query.exec("DELETE FROM Categories;") )
	throw std::runtime_error(query.lastError().text().toStdString());
    query.finish();

    DEBUG("Saving...");
    saveDbRecurse(_tree, QModelIndex());

    DEBUG("Commiting...");
    t.commit();
    DEBUG("Tasks are saved db in " << ti.end());
}

void Saver::save(TaskTree& _tree)
{
	QString fname = getHome() + FNAME_TASKS;
	if( !createDirFromFile(fname) )
		ERROR("Can't create directory for file '" <<fname<<"'");

	// Удалим все файлы задач - мы же их всё равно будем перезаписывать, а если удалим - не
	// нужно будет думать про удалённые задачи
	QStringList files = getTaskList();
	for(int i=0; i<files.size(); ++i)
	{
		if( !QFile::remove(getHome() + files[i]) )
			DEBUG("Can't remove task " << files[i] << " - check permissions");
	}


	// Сохраним
	recurseSave(_tree, QModelIndex());

	saveDb(_tree);
}

Saver::TaskMap Saver::restoreDbTasks() {
    TaskMap tasks;
    TimeItem ti;
    DEBUG("Restoring tasks from db...");

    QSqlQuery query(m_Db);
    query.exec("SELECT uuid, parentUuid, name, notes, created, localUpdated, globalUpdated,"
	       "started, finished, planned, parentIndex, categories, iconName FROM Tasks;");
    while (query.next()) {
	Task task;
	int dt;
	task.setId( QUuid(query.value(0).toString()) );
	task.setParentId( QUuid(query.value(1).toString()) );
	task.setName( query.value(2).toString() );
	task.setNotes( query.value(3).toString() );
	task.setCreated( QDateTime::fromTime_t(query.value(4).toInt()) );
	task.setLocalUpdated( QDateTime::fromTime_t(query.value(5).toInt()) );
	task.setGlobalUpdated( QDateTime::fromTime_t(query.value(6).toInt()) );
	if( (dt = query.value(7).toInt())!=0 )
	    task.setStarted( QDateTime::fromTime_t(dt) );
	if( (dt = query.value(8).toInt())!=0 )
	    task.setFinished( QDateTime::fromTime_t(dt) );
	task.setPlannedTime( query.value(9).toString() );
	task.setParentIndex( query.value(10).toInt() );

	QStringList lst = query.value(11).toString().split( ";" );
	task.setCategories( lst );

	task.setIconName( query.value(12).toString() );

	tasks[ task.getId() ] = task;
//	DEBUG("Restored task " << task.getName());
    }
    query.finish();

    DEBUG("Tasks are restored from db in " << ti.end());

    return tasks;
}

QStringList Saver::restoreDbCategories() {
    QStringList result;
    TimeItem ti;
    DEBUG("Restoring categories from db...");

    QSqlQuery query(m_Db);
    query.exec("SELECT categories from Tasks;");
    while (query.next()) {
	QStringList lst = query.value(0).toString().split( ";" );
	for( int i = 0; i < lst.size(); ++i )
		if( !lst[i].isEmpty() )
			result << lst[i];
    }
    query.finish();

    DEBUG("Categories are restored from db in " << ti.end());

    return result;
}

void Saver::restore(TaskTree& _tree, CategoryTree& _cats)
{

	TaskMap tasks;
	// Читаем из файла или из базы
#if 0
	QStringList files = getTaskList();
	for(int i=0;i<files.size();i++)
	{
//		DEBUG(files[i]);
		QString fname = getHome() + files[i];
		std::ifstream file(fname.toUtf8().data());
		if( file.fail() )
			ERROR("Unable to open file '" << fname << "'");

		Task task;
		int line = 0;
		bool someLines = false;

		while( !file.eof() )
		{
			std::string s;
			std::getline( file, s );
			++line;
			if( s.empty() )
				continue;

			if( someLines && s[0] == ' ' )
			{
				task.setNotes( task.getNotes() + '\n' + QString::fromUtf8( s.c_str() ).mid( 1 ) );
			}
			else
			{
				QStringList lst = QString::fromUtf8( s.c_str() ).split( ":" );
				if( lst.size() == 1 )
				{
					DEBUG("Line without specifier on line " << line);
					continue;
				}
				QString id = lst[0];
				lst.removeFirst();
				QString value = lst.join( ":" );

				if( id == "ID" )
				{
					task.setId( value );
					someLines = false;
				}
				else if( id.compare( "NAME", Qt::CaseInsensitive ) == 0 )
				{
					task.setName( value );
					someLines = false;
				}
				else if( id.compare( "Parent", Qt::CaseInsensitive ) == 0 )
				{
					task.setParentId( value );
					someLines = false;
				}
				else if( id.compare( "Notes", Qt::CaseInsensitive ) == 0 )
				{
					task.setNotes( value );
					someLines = true;
				}
				else if( id.compare( "DateCreated", Qt::CaseInsensitive ) == 0 )
				{
					task.setCreated( QDateTime::fromString( value, Qt::ISODate ) );
					someLines = false;
				}
				else if( id.compare( "DateStarted", Qt::CaseInsensitive ) == 0 )
				{
					task.setStarted( QDateTime::fromString( value, Qt::ISODate ) );
					someLines = false;
				}
				else if( id.compare( "DateFinished", Qt::CaseInsensitive ) == 0 )
				{
					task.setFinished( QDateTime::fromString( value, Qt::ISODate ) );
					someLines = false;
				}
				else if( id.compare( "PlannedTime", Qt::CaseInsensitive ) == 0 )
				{
					task.setPlannedTime( value );
					someLines = false;
				}
				else if( id.compare( "Categories", Qt::CaseInsensitive ) == 0 )
				{
					QStringList lst = value.split( ";" );
					task.setCategories( lst );
					someLines = false;

					// Проверим, что такие категории есть
					for( int i = 0; i < lst.size(); ++i )
						if( !lst[i].isEmpty() )
							_cats.addCategory( Category( lst[i] ) );
				}
			}
		}

		tasks[task.getId()] = task;
	}
#else
	Transaction<Saver> t(*this);
	tasks = restoreDbTasks();
	QStringList cats = restoreDbCategories();
	for(int i=0; i<cats.size(); ++i)
	    _cats.addCategory( cats[i] );
	t.commit();
#endif

	// Добавим задачи в дерево
	while( !tasks.empty() )
		recurseAddTasks(_tree, tasks.begin().value(), tasks);

//	_tree.setChanged( false );
}
void Saver::saveDbActivities(const DayActivities&) {
//    size_t sz = _tree.count();
//    for(size_t i=0;i<sz;++i)
//	    saveActivity(file, _tree.getActivity(i));
}

void Saver::save(const QDate& _date, const DayActivities& _tree)
{
	QString fname = getHome() + FNAME_ACTS;
	fname += _date.toString(Qt::ISODate);

	if( !createDirFromFile(fname) )
		ERROR("Can't create directory for file '" <<fname<<"'");
	std::ofstream file(fname.toUtf8().data(), std::ios::trunc);
	if( !file )
		ERROR("Unable to open file '" << fname << "'");

	size_t sz = _tree.count();
	for(size_t i=0;i<sz;++i)
		saveActivity(file, _tree.getActivity(i));
}

bool Saver::canRestore(const QDate& _date)
{
	QString fname = getHome() + FNAME_ACTS;
	fname += _date.toString(Qt::ISODate);
	return QFile::exists(fname);
}

void Saver::restore(const QDate& _date, DayActivities& _tree)
{
	QString fname = getHome() + FNAME_ACTS;
	fname += _date.toString(Qt::ISODate);

	std::ifstream file(fname.toUtf8().data());
	if( !file )
		ERROR("Unable to open file '" << fname << "'");

	size_t line = 0;
	bool hasStarted = false;
	Activity act;

	while( !file.eof() )
	{
		std::string s;
		std::getline(file, s);
		++line;
		if( s.empty() )
			continue;

		if( s=="BEGIN:VEVENT" )
		{
			if( hasStarted )
				ERROR("Begin new event while other is not ended on line " << line);
			hasStarted = true;
			act = Activity();
		}
		else if( s=="END:VEVENT" )
		{
			if( !hasStarted )
				ERROR("End event while nothing begins on line " << line);
			hasStarted = false;
			_tree.addActivity(act);
		}
		else
		{
			QStringList lst = QString::fromUtf8( s.c_str() ).split( ":" );
			if( lst.size() == 1 )
			{
				DEBUG("Line without specifier on line " << line);
				continue;
			}

			if( lst[0] == "ID" )
			{
				act = Activity( QDateTime::fromTime_t(lst[1].toUInt()));
			}
			else if( lst[0].compare( "NAME", Qt::CaseInsensitive ) == 0 )
			{
				act.setName( lst[1] );
			}
			else if( lst[0].compare( "Parent", Qt::CaseInsensitive ) == 0 )
			{
				act.setAssignedTask( lst[1] );
			}
			else if( lst[0].compare( "Interrupts", Qt::CaseInsensitive ) == 0 )
			{
				act.setInterrupts( lst[1].toUInt() );
			}
		}
	}

	_tree.setChanged(false);
}

void Saver::saveActivity(std::ofstream& _file, const Activity& _act)
{
	_file << "BEGIN:VEVENT" << std::endl
		<< "ID:" << _act.getStartTime().toTime_t() << std::endl
		<< "Name:" << _act.getName().toUtf8().data() << std::endl
//		<< "Notes:" << escapeString(_task.getNotes()).toUtf8().data() << std::endl
		<< "Parent:" << _act.getAssignedTask().toString() << std::endl
		<< "Interrupts:" << _act.getInterrupts() << std::endl
		<< "END:VEVENT" << std::endl;
}

Saver::DateSet Saver::getActiveDays()
{
	DateSet res;
	QString fname = getHome() + FNAME_ACTS;

	if( !createDirFromFile(fname) )
		ERROR("Can't create directory for file '" <<fname<<"'");

	fname = fname.left(fname.lastIndexOf("/"));

	QDir dir(fname);
	dir.setFilter( QDir::Files | QDir::Hidden | QDir::NoSymLinks );
	QStringList filters;
	filters << "Activities.*";
	dir.setNameFilters( filters );

	QFileInfoList list = dir.entryInfoList();
	for( int i = 0; i < list.size(); ++i )
	{
		QFileInfo fileInfo = list.at(i);
		QString date = fileInfo.fileName().mid(11);
//		DEBUG("Day with activities - " << date);
		res.insert( QDate::fromString(date, Qt::ISODate) );
	}

	return res;
}

/// Возвращает список файлов задач
QStringList	Saver::getTaskList()
{
	QStringList res;
	QString fname = getHome() + FNAME_TASKS;

	if( !createDirFromFile(fname) )
		ERROR("Can't create directory for file '" <<fname<<"'");

	fname = fname.left(fname.lastIndexOf("/"));

	QDir dir(fname);
	dir.setFilter( QDir::Files | QDir::Hidden | QDir::NoSymLinks );
	QStringList filters;
	filters << "Tasks.*";
	dir.setNameFilters( filters );

	QFileInfoList list = dir.entryInfoList();
	for( int i = 0; i < list.size(); ++i )
	{
		QFileInfo fileInfo = list.at(i);
		res << fileInfo.fileName();
	}

	return res;
}

/// Добавляет задачи в дерево таким образом, что родительский узел для _task добавляется перед _task.
/// После добавления, _task из _tasks удаляется
void Saver::recurseAddTasks(TaskTree& _tree, Task& _task, TaskMap& _tasks)
{
//    DEBUG("Adding task " << _task.getName());
	// А есть ли родитель в дереве?
	if( _tree.getItem( _task.getParentId() )==NULL )
	{
		// Родителя в дереве нет - нужно рекурсивно добавлять
		TaskMap::iterator it=_tasks.find(_task.getParentId());
		if( it==_tasks.end() )
			ERROR("There is no task with id " << _task.getParentId());
		recurseAddTasks(_tree, it.value(), _tasks);
	}

	// Родитель уже в дереве - смело добавляем потомка
	_tree.addChild( _task.getParentId(), _task );

	// Теперь можно удалить добавленную задачу
	_tasks.remove(_task.getId());
}

void Saver::startTransaction() {
    TRACE;
    if( !m_Db.transaction() )
	throw std::runtime_error(m_Db.lastError().text().toStdString());
    inTransaction = true;
}
void Saver::commit() {
    TRACE;
    if( !m_Db.commit() )
	throw std::runtime_error(m_Db.lastError().text().toStdString());
    inTransaction = false;
}
void Saver::rollback() {
    TRACE;
    if( !m_Db.rollback() )
	throw std::runtime_error(m_Db.lastError().text().toStdString());
    inTransaction = false;
}

void Saver::replaceTask(const Task& _task) {
    TRACE;
    // TODO Проверять что переданная задача новее, чем в хранилище и заменяет её
    saveDbTask(_task);
}

void Saver::removeTask(const Task&) {
    TRACE;
    throw std::runtime_error(__PRETTY_FUNCTION__ + std::string("Not implemented"));
}

void Saver::saveIcon(const QString& _uuid, const QString& _name, const QIcon& _icon) {
    TRACE;
    QByteArray data;
    QDataStream s(&data, QIODevice::ReadWrite);
    s << _icon;

    QSqlQuery query(m_Db);
    if( !query.prepare("REPLACE INTO Icons(uuid, name, body) VALUES (?, ?, ?);") )
	throw std::runtime_error("prepare error " + query.lastError().text().toStdString());
    query.addBindValue(_uuid);
    query.addBindValue(_name);
    query.addBindValue(data);
    if( !query.exec() )
	throw std::runtime_error("exec error " + query.lastError().text().toStdString());
    query.finish();
}

QIcon Saver::restoreIcon(const QString& _name) {
    QIcon result;

//    TimeItem ti;

    QSqlQuery query(m_Db);
    if( !query.prepare("SELECT body from Icons where uuid like ?;") )
	throw std::runtime_error("prepare error " + query.lastError().text().toStdString());
    query.addBindValue(_name);
    if( !query.exec() )
	throw std::runtime_error("exec error " + query.lastError().text().toStdString());
    if( query.first() ) {
	DEBUG("Storage has " << _name << " icon");
	QByteArray data;// = query.value(0).toByteArray();
	QDataStream s(&data, QIODevice::ReadWrite);
	data = query.value(0).toByteArray();
	s >> result;
    }
    query.finish();

//    DEBUG("Icon was restored from db in " << ti.end());

    return result;
}

QStringMap Saver::getIconList() {
    QStringMap res;

    QSqlQuery query(m_Db);
    query.prepare("SELECT uuid, name from Icons;");
    if( !query.exec() )
	throw std::runtime_error(query.lastError().text().toStdString());
    while( query.next() ) {
	QString uuid = query.value(0).toString();
	QString name = query.value(1).toString();
	res[uuid] = name;
    }
    query.finish();
    DEBUG("Will work with " << res.size() << " icons in storage");

    return res;
}
