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
	typedef std::set<Activity> ActivitySet;
	ActivitySet	m_Activities;
public:
	DayActivities();
	void		addActivity(const Activity& _act);

	size_t		count() const;
	const Activity&	getActivity(size_t _idx) const;
	void		delActivity(size_t _idx);
	void		setActivity(size_t _idx, const Activity& _act);
	void		setActivity(const Activity& _act);
};

#endif /* DAYACTIVITIES_H_ */
