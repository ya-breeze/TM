/*
 * Activities.cpp
 *
 *  Created on: 22.08.2009
 *      Author: breeze
 */

#include "Activities.h"

#include "utils.h"

Activities::Activities( const QDate& _date )
	: m_Today(_date), has_CurActivity(false)
{
	Saver saver;
	m_Days = saver.getActiveDays();
}

bool Activities::hasCurActivity() const
{
	return has_CurActivity;
}

void Activities::addActivity(const Activity& _act, bool _setCurrent)
{
	DayActivities &act = getDay(_act.getStartTime().date());
	act.addActivity(_act);

	if( _setCurrent )
	{
		m_CurActivity = _act;
		m_Today = _act.getStartTime().date();
		has_CurActivity = true;
	}
}

const Activity& Activities::getCurrentActivity() const
{
	return m_CurActivity;
}

size_t Activities::countDays() const
{
	return m_Activities.size();
}

const QDate& Activities::getToday() const
{
	return m_Today;
}

void Activities::setToday( const QDate& _date )
{
	m_Today = _date;
	if( _date.isNull() )
		m_Today = QDate::currentDate();

	DayActivities &day = getDay(m_Today);
	if( day.count() )
	{
		m_CurActivity = day.getActivity( day.count()-1 );
		has_CurActivity = true;
	}
	DEBUG(m_CurActivity.getName());
}

DayActivities& Activities::getDay(const QDate& _date)
{
	ActivitySet::iterator it=m_Activities.find(_date);
	if( it!=m_Activities.end() )
		return it->second;

	Saver saver;
	DayActivities &act = m_Activities[_date];
	if( saver.canRestore(_date) )
		saver.restore(_date, act);

	return act;
}

bool Activities::hasChanged() const
{
	for(ActivitySet::const_iterator it=m_Activities.begin();it!=m_Activities.end(); ++it)
		if( it->second.hasChanged() )
			return true;

	return false;
}

void Activities::save()
{
	Saver saver;
	for(ActivitySet::iterator it=m_Activities.begin();it!=m_Activities.end(); ++it)
	{
		if( it->second.hasChanged() )
		{
			saver.save(it->first, it->second);
			it->second.setChanged(false);
		}
	}
}
