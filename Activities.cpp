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

void Activities::addActivity(const Activity& _act, bool _setCurrent)
{
	m_Activities.insert(_act);
	if( _setCurrent )
	{
		m_CurActivity = _act;
		setToday(_act.getStartTime().date());
	}
	else
		updateDayRanges();
}

const Activity& Activities::getCurrentActivity() const
{
	return m_CurActivity;
}

size_t Activities::count() const
{
	return m_DaySize;
}

const Activity& Activities::getActivity(size_t _idx) const
{
	Q_ASSERT(_idx<count());

	ActivitySet::const_iterator it=it_Begin;
	for(size_t i=0;i<count();++i,++it)
		if( i==_idx)
			return *it;

	ERROR("Wrong index in getActivity");
}

void Activities::delActivity(size_t _idx)
{
	Q_ASSERT(_idx<count());

	ActivitySet::const_iterator it=it_Begin;
	for(size_t i=0;i<count();++i,++it)
		if( i==_idx)
		{
			m_Activities.erase(it);
			updateDayRanges();
			return;
		}

	ERROR("Wrong index in getActivity");
}

void Activities::setActivity(size_t _idx, const Activity& _act)
{
	delActivity(_idx);
	addActivity(_act);
}

void Activities::setActivity(const Activity& _act)
{
	ActivitySet::iterator it=m_Activities.find(_act);
	if( it!=m_Activities.end() )
		m_Activities.erase(it);

	m_Activities.insert(_act);
}

const QDate& Activities::getToday() const
{
	return m_Today;
}

void Activities::setToday( const QDate& _date )
{
	if( _date.isNull() && !m_Activities.empty() )
	{
		// Самая последняя запись
		ActivitySet::reverse_iterator it=m_Activities.rbegin();
		m_Today = it->getStartTime().date();
		m_CurActivity = *it;
	}
	else
		m_Today = _date;

	updateDayRanges();
}

void Activities::updateDayRanges()
{
	QDateTime tm(m_Today);
	Activity act(tm), act2(tm.addDays(1));
	it_Begin = m_Activities.find(act);
	it_End = m_Activities.find(act2);

	m_DaySize = 0;
	for(ActivitySet::const_iterator it=it_Begin;it!=it_End;++it)
		++m_DaySize;

	setChanged();
}
