/*
 * TimeScale.cpp
 *
 *  Created on: 04.09.2009
 *      Author: breeze
 */

#include "TimeScale.h"

#include "PlannedTime.h"

TimeScale::TimeScale( TaskTree *_tasks,  QObject *_parent  )
	: QObject(_parent), p_Tasks(_tasks)
{
}

/// Выводит на консольку список задач, которые можно выводить на шкалу времени
void TimeScale::dump(QModelIndex _idx)
{
	TaskItem *item = p_Tasks->getItem(_idx);
	QDateTime started = item->getStarted();
	QDateTime finished = item->getFinished();
	PlannedTime planned( item->getPlannedTime() );

	// Возможны такие случаи
	if( !started.isNull() && !finished.isNull() )
	{
		// Есть даты начала и конца - можем выводить диапазон
	}
	else if( !started.isNull() && finished.isNull() )
	{
		// Есть время начала, но нет времени конца
	}
	else if( started.isNull() && !finished.isNull() )
	{
		// Есть время конца, но нет начала - можем просто галочку поставить
	}
	else if( !planned.isNull() )
	{
		// Нет ни начала, ни конца, но есть запланированное время - тоже можем рисовать
	}

	int sz = p_Tasks->rowCount(_idx);
	for(int i=0;i<sz;++i)
	{
		dump( p_Tasks->index(i, 0, _idx) );
	}
}
