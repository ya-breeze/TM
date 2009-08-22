/*
 * Activity.cpp
 *
 *  Created on: 22.08.2009
 *      Author: breeze
 */

#include "Activity.h"

#include <QObject>

Activity::Activity(const QDateTime& _dt)
	: m_StartTime(_dt), str_Name(QObject::tr("Unknown")), m_Interrupts(0)
{
}

bool Activity::operator<(const Activity& _a) const
{
	return (m_StartTime<_a.m_StartTime);
}
