/*
 * CalendarShower.cpp
 *
 *  Created on: 20.12.2009
 *      Author: breeze
 */

#include "CalendarShower.h"

#include "utils.h"

CalendarShower::CalendarShower( QObject *_parent )
	: QObject(_parent)
{
}

bool CalendarShower::eventFilter( QObject *obj, QEvent *event )
{
//	DEBUG(event->type());
	if( event->type() == QEvent::MouseButtonPress )
	{
		emit clicked();
		return true;
	}

	return QObject::eventFilter( obj, event );
}
