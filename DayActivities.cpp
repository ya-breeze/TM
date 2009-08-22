/*
 * DayActivities.cpp
 *
 *  Created on: 22.08.2009
 *      Author: breeze
 */

#include "DayActivities.h"

#include "utils.h"

DayActivities::DayActivities( const QDate& _date )
	: ChangableObject(), m_Today(_date)
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

	Activities::const_iterator it=m_Activities.begin();
	for(size_t i=0;i<m_Activities.size();++i,++it)
		if( i==_idx)
			return *it;

	ERROR("Wrong index in getActivity");
}

void DayActivities::delActivity(size_t _idx)
{
	Q_ASSERT(_idx<count());

	Activities::const_iterator it=m_Activities.begin();
	for(size_t i=0;i<m_Activities.size();++i,++it)
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
	setChanged();
}

const QDate& DayActivities::getToday() const
{
	return m_Today;
}
