/*
 * Activities.cpp
 *
 *  Created on: 22.08.2009
 *      Author: breeze
 */

#include "Activities.h"

#include "utils.h"

Activities::Activities( const QDate& _date )
	: ChangableObject(), m_Today(_date)
{
}

void Activities::addActivity(const Activity& _act)
{
	m_Activities.insert(_act);
	setChanged();
}

size_t Activities::count() const
{
	return m_Activities.size();
}

const Activity& Activities::getActivity(size_t _idx) const
{
	Q_ASSERT(_idx<count());

	ActivitySet::const_iterator it=m_Activities.begin();
	for(size_t i=0;i<m_Activities.size();++i,++it)
		if( i==_idx)
			return *it;

	ERROR("Wrong index in getActivity");
}

void Activities::delActivity(size_t _idx)
{
	Q_ASSERT(_idx<count());

	ActivitySet::const_iterator it=m_Activities.begin();
	for(size_t i=0;i<m_Activities.size();++i,++it)
		if( i==_idx)
		{
			m_Activities.erase(it);
			setChanged();
			return;
		}

	ERROR("Wrong index in getActivity");
}

void Activities::setActivity(size_t _idx, const Activity& _act)
{
	delActivity(_idx);
	addActivity(_act);
	setChanged();
}

const QDate& Activities::getToday() const
{
	return m_Today;
}
