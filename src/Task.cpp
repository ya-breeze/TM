/*
 * Task.cpp
 *
 *  Created on: 21.08.2009
 *      Author: breeze
 */

#include "Task.h"

#include <QObject>


Task::Task(const QString& _name)
	: m_Created( QDateTime::currentDateTime() ), m_ParentIndex(0),
	  m_LocalUpdated( m_Created ), m_GlobalUpdated(m_LocalUpdated)
{
	str_Name = _name;
	if( str_Name.isEmpty() )
		str_Name = QObject::tr("Unnamed");
	m_Id = QUuid::createUuid();
}
