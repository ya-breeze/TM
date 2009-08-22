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

public:
	Activities( const QDate& _date = QDate::currentDate() );

	void		addActivity(const Activity& _act);

	size_t		count() const;
	const Activity&	getActivity(size_t _idx) const;
	void		delActivity(size_t _idx);
	void		setActivity(size_t _idx, const Activity& _act);

	const QDate&	getToday() const;
};

#endif /* ACTIVITIES_H_ */
