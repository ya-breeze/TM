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
#include <QSqlDatabase>

#include "utils.h"
#include "Task.h"
#include "DayActivities.h"
#include "CategoryTree.h"

template<class T>
class Transaction {
    bool in_Transaction;
    T&	 m_Saver;
public:
    Transaction(T &_saver)
	: m_Saver(_saver) {
	m_Saver.startTransaction();
	in_Transaction = true;
    }
    ~Transaction() {
	if( in_Transaction )
	    m_Saver.rollback();
    }
    void commit() {
	if( in_Transaction )
	    m_Saver.commit();
	in_Transaction = false;
    }
};

class TaskTree;

/// \brief Класс инкапсулирующий формат хранения всех данных программы
///
/// Saver incapsulates all DB operations and saves changelog for all
/// saving data.
class Saver
{
    enum Status {
	ST_UPDATED = 0,
	ST_DELETED = 1
    };

public:
	typedef std::set<QDate> DateSet;
	typedef QMap<QUuid, Task> TaskMap;

	Saver();
	~Saver();

	void init();

	// Заглушка, пока задачи хранятся в TaskTree, а не в Saver
//	TaskTree *tree;
//	void	setTaskTree(TaskTree& _tree) { tree = &_tree; }
//	typedef std::list<const Task*> TaskList;
//	TaskList getTasks(const QModelIndex& _idx = QModelIndex());


	void	save(TaskTree& _tree);
	void	restore(TaskTree& _tree, CategoryTree& _cats);

	QStringMap getIconList();
	void	saveIcon(const QString& _uuid, const QString& _name, const QIcon& _icon);
	QIcon	restoreIcon(const QString& _uuid);

	/// Проверяет, что переданная задача новее, чем в хранилище и заменяет её
	void	replaceTask(const Task& _task);
	void	removeTask(const Task& _task);

	void	save(const QDate& _date, const DayActivities& _tree);
	void	restore(const QDate& _date, DayActivities& _tree);
	bool	canRestore(const QDate& _date);
	DateSet	getActiveDays();
	QString	getHome() const;

	void	startTransaction();
	void	commit();
	void	rollback();

	QString getLocalUuid() { return "dc6a478e-edeb-45ee-a7c7-dc2b258182d8"; };
	size_t getLastUpdated(const QString& /*_uuid*/ ) { return 0; };

	TaskMap		restoreDbTasks();

protected:
	void		saveDb(TaskTree& _tree);
	QStringList	restoreDbCategories();
	void		saveDbActivities(const DayActivities& _tree);
	void		saveDbRecurse(TaskTree& _tree, const QModelIndex& _idx);
	void		saveDbTask(const Task& _task);
	void		recurseSave(const TaskTree& _tree, const QModelIndex& _idx);
	void		saveTask(const Task& _task);
	void		saveActivity(std::ofstream& _file, const Activity& _act);
	QString 	escapeString(const QString& _str);
	/// Возвращает список файлов задач
	QStringList	getTaskList();

	/// Добавляет задачи в дерево таким образом, что родительский узел для _task добавляется перед _task.
	/// После добавления, _task из _tasks удаляется
	void		recurseAddTasks(TaskTree& _tree, Task& _task, TaskMap& _tasks);

	/// Add record into ChangeLog table
	void	addChangeLog(const QString& _uuid, const QString& _type, Status _status);

	bool		inTransaction;

	QSqlDatabase	m_Db;
};

#endif /* SAVER_H_ */
