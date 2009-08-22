/*
 * Activities.h
 *
 *  Created on: 22.08.2009
 *      Author: breeze
 */

#ifndef ACTIVITIES_H_
#define ACTIVITIES_H_

#include <set>
#include "Activity.h"
#include "ChangableObject.h"

/// Хранилище хронометража одного дня. Весь день состоит из множества Activity
class Activities : public ChangableObject
{
	typedef std::set<Activity> ActivitySet;
	ActivitySet	m_Activities;
	bool		has_Changed;
	QDate		m_Today;
	Activity	m_CurActivity;

	ActivitySet::iterator
			it_Begin, it_End;
	size_t		m_DaySize;

public:
	Activities( const QDate& _date = QDate::currentDate() );

	void		addActivity(const Activity& _act, bool _setCurrent = true);
	const Activity& getCurrentActivity() const;

	/// Количество активностей за сегодняшний день
	size_t		count() const;
	const Activity&	getActivity(size_t _idx) const;
	void		delActivity(size_t _idx);
	void		setActivity(size_t _idx, const Activity& _act);
	void		setActivity(const Activity& _act);

	const QDate&	getToday() const;
	/// Устанавливает текущий день, а самую последнюю задачу из указанного дня считает текущей
	/// Если _date.isNull(), то в качестве сегодняшнего дня выставляется самый последний день
	void		setToday( const QDate& _date = QDate() );

protected:
	void		updateDayRanges();
};

#endif /* ACTIVITIES_H_ */
