/*
 * DayActivities.h
 *
 *  Created on: 22.08.2009
 *      Author: breeze
 */

#ifndef DAYACTIVITIES_H_
#define DAYACTIVITIES_H_

#include <set>
#include "Activity.h"
#include "ChangableObject.h"

/// Хранилище хронометража одного дня. Весь день состоит из множества Activity
class DayActivities : public ChangableObject
{
	typedef std::set<Activity> Activities;
	Activities	m_Activities;
	bool		has_Changed;
	QDate		m_Today;

public:
	DayActivities( const QDate& _date = QDate::currentDate() );

	void		addActivity(const Activity& _act);

	size_t		count() const;
	const Activity&	getActivity(size_t _idx) const;
	void		delActivity(size_t _idx);
	void		setActivity(size_t _idx, const Activity& _act);

	const QDate&	getToday() const;
};

#endif /* DAYACTIVITIES_H_ */
