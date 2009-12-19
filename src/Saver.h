/*
 * Saver.h
 *
 *  Created on: 21.08.2009
 *      Author: breeze
 */

#ifndef SAVER_H_
#define SAVER_H_

#include <fstream>
#include <QModelIndex>
#include <QDate>

#include "TaskTree.h"
#include "DayActivities.h"
#include "CategoryTree.h"

/// Класс инкапсулирующий формат хранения всех данных программы
class Saver
{
public:
	typedef std::set<QDate> DateSet;
	typedef std::map<QUuid, Task> TaskMap;

	void	save(TaskTree& _tree);
	void	restore(TaskTree& _tree, CategoryTree& _cats);

	void	save(const QDate& _date, const DayActivities& _tree);
	void	restore(const QDate& _date, DayActivities& _tree);
	bool	canRestore(const QDate& _date);
	DateSet	getActiveDays();
	QString	getHome() const;

protected:
	void		recurseSave(const TaskTree& _tree, const QModelIndex& _idx);
	void		saveTask(const Task& _task);
	void		saveActivity(std::ofstream& _file, const Activity& _act);
	QString 	escapeString(const QString& _str);
	/// Возвращает список файлов задач
	QStringList	getTaskList();

	/// Добавляет задачи в дерево таким образом, что родительский узел для _task добавляется перед _task.
	/// После добавления, _task из _tasks удаляется
	void		recurseAddTasks(TaskTree& _tree, Task& _task, TaskMap& _tasks);
};

#endif /* SAVER_H_ */
