/*
 * DayActivities.cpp
 *
 *  Created on: 22.08.2009
 *      Author: breeze
 */

#include "DayActivities.h"

#include "utils.h"

DayActivities::DayActivities()
	: ChangableObject()
{
}

void DayActivities::addActivity(const Activity& _act)
{
	m_Activities.insert(_act);
	setChanged();
}

size_t DayActivities::count() const
{
	return m_Activities.size();
}

const Activity& DayActivities::getActivity(size_t _idx) const
{
	Q_ASSERT(_idx<count());

	ActivitySet::const_iterator it=m_Activities.begin();
	for(size_t i=0;i<count();++i,++it)
		if( i==_idx)
			return *it;

	ERROR("Wrong index in getActivity");
}

void DayActivities::delActivity(size_t _idx)
{
	Q_ASSERT(_idx<count());

	ActivitySet::const_iterator it=m_Activities.begin();
	for(size_t i=0;i<count();++i,++it)
		if( i==_idx)
		{
			m_Activities.erase(it);
			setChanged();
			return;
		}

	ERROR("Wrong index in getActivity");
}

void DayActivities::setActivity(size_t _idx, const Activity& _act)
{
	delActivity(_idx);
	addActivity(_act);
}

void DayActivities::setActivity(const Activity& _act)
{
	ActivitySet::iterator it=m_Activities.find(_act);
	if( it!=m_Activities.end() )
	{
DEBUG("Delete old version of Activity");
		m_Activities.erase(it);
	}

DEBUG("Adding version of Activity - " << _act.getInterrupts());
	m_Activities.insert(_act);
	setChanged();
}
