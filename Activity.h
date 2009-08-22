/*
 * Activity.h
 *
 *  Created on: 22.08.2009
 *      Author: breeze
 */

#ifndef ACTIVITY_H_
#define ACTIVITY_H_

#include <QDateTime>
#include <QString>
#include <QUuid>

/// Класс для описания одной активности в хронометраже дня
class Activity
{
	/// Время начала
	QDateTime	m_StartTime;
	/// Название
	QString		str_Name;
	/// Id привязанной задачи
	QUuid		m_AssignedTask;
	/// Количество прерываний
	size_t		m_Interrupts;

public:
	Activity(const QDateTime& _dt = QDateTime::currentDateTime());

	bool operator<(const Activity& _a) const;

	////////////////// Get/Set ////////////////////
	QUuid getAssignedTask() const
	{
		return m_AssignedTask;
	}

	size_t getInterrupts() const
	{
		return m_Interrupts;
	}

	QDateTime getStartTime() const
	{
		return m_StartTime;
	}

	QString getName() const
	{
		return str_Name;
	}

	void setAssignedTask( QUuid m_AssignedTask )
	{
		this->m_AssignedTask = m_AssignedTask;
	}

	void setInterrupts( size_t m_Interrupts )
	{
		this->m_Interrupts = m_Interrupts;
	}

	void setStartTime( QDateTime m_StartTime )
	{
		this->m_StartTime = m_StartTime;
	}

	void setName( QString str_Name )
	{
		this->str_Name = str_Name;
	}
};

#endif /* ACTIVITY_H_ */
