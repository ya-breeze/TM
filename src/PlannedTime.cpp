/*
 * PlannedTime.cpp
 *
 *  Created on: 04.09.2009
 *      Author: breeze
 */

#include "PlannedTime.h"

#include <QRegExp>
#include "utils.h"

PlannedTime::PlannedTime( const QString& _time )
{
	parse(_time);
}

void PlannedTime::parse( const QString& _time )
{
//	QRegExp re("([0-9]+d)?*([0-9]+h)?");
	QRegExp re("([0-9]+d)?.*([0-9]+h)?");
	if( re.indexIn(_time) != -1 )
	{
		m_Days	= re.cap(1).left(re.cap(1).length()-1).toInt();
		m_Hours = re.cap(2).left(re.cap(2).length()-1).toInt();
		DEBUG("Time is ok - " << m_Days << ":" << m_Hours);
	}
	else
	{
		DEBUG("Wrong time - " << _time);
		m_Days	= 0;
		m_Hours = 0;
	}
}

bool PlannedTime::isNull() const
{
	return (m_Days || m_Hours) ? true : false;
}
