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

#include "utils.h"


#define FNAME_TASKS	"Tasks"
#define FNAME_ACTS	"Activities."

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

void Saver::saveTask(std::ofstream& _file, const Task& _task)
{
//	DEBUG(_task.getName() << ":'" << escapeString(_task.getNotes()) << "'");
	_file << "BEGIN:VTODO" << std::endl
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

void Saver::recurseSave(std::ofstream& _file, const TaskTree& _tree, const QModelIndex& _idx)
{
	size_t sz = _tree.rowCount(_idx);
	for(size_t i=0;i<sz;++i)
	{
		QModelIndex idx = _tree.index(i, 0, _idx);
		const TaskItem *item = _tree.getItem(idx);
		Q_ASSERT(item);
		saveTask(_file, *item);

		if( _tree.columnCount(idx) )
			recurseSave(_file, _tree, idx);
	}
}

void Saver::save(TaskTree& _tree)
{
	QString fname = getHome() + FNAME_TASKS;
	if( !createDirFromFile(fname) )
		ERROR("Can't create directory for file '" <<fname<<"'")
	std::ofstream file(fname.toUtf8().data(), std::ios::trunc);
	if( !file )
		ERROR("Unable to open file '" << fname << "'");

	recurseSave(file, _tree, QModelIndex());
}

void Saver::restore(TaskTree& _tree, CategoryTree& _cats)
{
	QString fname = getHome() + FNAME_TASKS;
	std::ifstream file(fname.toUtf8().data());
	if( !file )
		ERROR("Unable to open file '" << fname << "'");

	size_t line = 0;
	bool hasStarted = false;
	Task task;
	bool someLines = false;

	while( !file.eof() )
	{
		std::string s;
		std::getline(file, s);
		++line;
		if( s.empty() )
			continue;

		if( s=="BEGIN:VTODO" )
		{
			if( hasStarted )
				ERROR("Begin new ToDo while other is not ended on line " << line);
			hasStarted = true;
			task = Task();
			someLines = false;
		}
		else if( s=="END:VTODO" )
		{
			if( !hasStarted )
				ERROR("End ToDo while nothing begins on line " << line);
			hasStarted = false;
			_tree.addChild(task.getParentId(), task);
			someLines = false;
		}
		else
		{
			if( someLines && s[0]==' ' )
			{
				task.setNotes( task.getNotes() + '\n' + QString::fromUtf8( s.c_str() ).mid(1) );
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
				QString value = lst.join(":");

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
					task.setCreated( QDateTime::fromString(value, Qt::ISODate) );
					someLines = false;
				}
				else if( id.compare( "DateStarted", Qt::CaseInsensitive ) == 0 )
				{
					task.setStarted( QDateTime::fromString(value, Qt::ISODate) );
					someLines = false;
				}
				else if( id.compare( "DateFinished", Qt::CaseInsensitive ) == 0 )
				{
					task.setFinished( QDateTime::fromString(value, Qt::ISODate) );
					someLines = false;
				}
				else if( id.compare( "PlannedTime", Qt::CaseInsensitive ) == 0 )
				{
					task.setPlannedTime( value );
					someLines = false;
				}
				else if( id.compare( "Categories", Qt::CaseInsensitive ) == 0 )
				{
					QStringList lst = value.split(";");
					task.setCategories( lst );
					someLines = false;

					// Проверим, что такие категории есть
					for(int i=0;i<lst.size();++i)
						if( !lst[i].isEmpty() )
							_cats.addCategory( Category(lst[i]) );
				}
			}
		}
	}

	_tree.setChanged(false);
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
