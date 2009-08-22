/*
 * Saver.cpp
 *
 *  Created on: 21.08.2009
 *      Author: breeze
 */

#include "Saver.h"

#include <stdlib.h>

#include "utils.h"


#define FNAME_TASKS	"/home/breeze/.TM/Tasks"
#define FNAME_ACTS	"/home/breeze/.TM/Activities"

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

void Saver::save(const TaskTree& _tree)
{
	if( !createDirFromFile(FNAME_TASKS) )
		ERROR("Can't create directory for file '" <<FNAME_TASKS<<"'")
	std::ofstream file(FNAME_TASKS, std::ios::trunc);
	if( !file )
		ERROR("Unable to open file '" << FNAME_TASKS << "'");
	recurseSave(file, _tree, QModelIndex());
}

void Saver::restore(TaskTree& _tree)
{
	std::ifstream file(FNAME_TASKS);
	if( !file )
		ERROR("Unable to open file '" << FNAME_TASKS << "'");

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

				if( lst[0] == "ID" )
				{
					task.setId( lst[1] );
					someLines = false;
				}
				else if( lst[0].compare( "NAME", Qt::CaseInsensitive ) == 0 )
				{
					task.setName( lst[1] );
					someLines = false;
				}
				else if( lst[0].compare( "Parent", Qt::CaseInsensitive ) == 0 )
				{
					task.setParentId( lst[1] );
					someLines = false;
				}
				else if( lst[0].compare( "Notes", Qt::CaseInsensitive ) == 0 )
				{
					task.setNotes( lst[1] );
					someLines = true;
				}
			}
		}
	}

	_tree.setChanged(false);
}

void Saver::save(const DayActivities& _tree)
{
	QString fname(FNAME_ACTS);
	fname += ".";
	fname += _tree.getToday().toString(Qt::ISODate);

	if( !createDirFromFile(fname.toStdString().c_str()) )
		ERROR("Can't create directory for file '" <<FNAME_TASKS<<"'")
	std::ofstream file(fname.toStdString().c_str(), std::ios::trunc);
	if( !file )
		ERROR("Unable to open file '" << fname << "'");

	size_t sz = _tree.count();
	for(size_t i=0;i<sz;++i)
		saveActivity(file, _tree.getActivity(i));
}

void Saver::restore(DayActivities& _tree)
{

}

void Saver::saveActivity(std::ofstream& _file, const Activity& _act)
{
	_file << "BEGIN:VEVENT" << std::endl
		<< "ID:" << _act.getStartTime().toTime_t() << std::endl
		<< "Name:" << _act.getName().toUtf8().data() << std::endl
//		<< "Notes:" << escapeString(_task.getNotes()).toUtf8().data() << std::endl
		<< "Parent:" << _act.getAssignedTask().toString() << std::endl
		<< "END:VEVENT" << std::endl;
}
