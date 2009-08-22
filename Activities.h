/*
 * Activities.h
 *
 *  Created on: 22.08.2009
 *      Author: breeze
 */

#ifndef ACTIVITIES_H_
#define ACTIVITIES_H_

#include <map>
#include "DayActivities.h"
#include "Saver.h"

/// Хранилище хронометража одного дня. Весь день состоит из множества Activity
class Activities// : public ChangableObject
{
	typedef std::map<QDate, DayActivities> ActivitySet;

	ActivitySet	m_Activities;
	Saver::DateSet	m_Days;
	QDate		m_Today;
	Activity	m_CurActivity;

public:
	Activities( const QDate& _date = QDate::currentDate() );

	void		addActivity(const Activity& _act, bool _setCurrent = true);
	const Activity& getCurrentActivity() const;

	size_t		countDays() const;
	const QDate&	getToday() const;
	/// Устанавливает текущий день, а самую последнюю задачу из указанного дня считает текущей
	/// Если _date.isNull(), то в качестве сегодняшнего дня выставляется сегодняшний день
	void		setToday( const QDate& _date = QDate() );
	DayActivities&	getDay(const QDate& _date);

	bool		hasChanged() const;

	void		save();
protected:
};

#endif /* ACTIVITIES_H_ */
