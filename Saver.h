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

#include "TaskTree.h"
#include "DayActivities.h"

class Saver
{
public:
	void save(const TaskTree& _tree);
	void restore(TaskTree& _tree);

	void save(const QDate& _date, const DayActivities& _tree);
	void restore(const QDate& _date, DayActivities& _tree);
	bool canRestore(const QDate& _date);
protected:
	void recurseSave(std::ofstream& _file, const TaskTree& _tree, const QModelIndex& _idx);
	void saveTask(std::ofstream& _file, const Task& _task);
	void saveActivity(std::ofstream& _file, const Activity& _act);
	QString escapeString(const QString& _str);
};

#endif /* SAVER_H_ */
