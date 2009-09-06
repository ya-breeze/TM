/*
 * TimeScale.h
 *
 *  Created on: 04.09.2009
 *      Author: breeze
 */

#ifndef TIMESCALE_H_
#define TIMESCALE_H_

#include <QObject>

#include "TaskTree.h"

class TimeScale : public QObject
{
	Q_OBJECT

	TaskTree	*p_Tasks;

public:
	TimeScale( TaskTree *_tasks, QObject * parent = NULL );
	/// Выводит на консольку список задач, которые можно выводить на шкалу времени,
	/// начиная с переданного родителя
	void dump(QModelIndex _idx);
};

#endif /* TIMESCALE_H_ */
