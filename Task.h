/*
 * Task.h
 *
 *  Created on: 21.08.2009
 *      Author: breeze
 */

#ifndef TASK_H_
#define TASK_H_

#include <QString>
#include <QUuid>
#include <vector>

class Task
{
	typedef std::vector<QUuid> UuidVec;

	////////////////// Properties //////////////////
	QString		str_Name;
	QUuid		m_Id;
	QUuid		m_Parent;
	UuidVec		m_Depends;
public:
	Task(const QString& _name = "");

	///////////////////// Get/Set ///////////////////

	UuidVec getDepends() const
	{
		return m_Depends;
	}

	QUuid getId() const
	{
		return m_Id;
	}

	QUuid getParent() const
	{
		return m_Parent;
	}

	QString getName() const
	{
		return str_Name;
	}

	void setId( QUuid m_Id )
	{
		this->m_Id = m_Id;
	}

	void setParent( QUuid m_Parent )
	{
		this->m_Parent = m_Parent;
	}

	void setName( const QString& str_Name )
	{
		this->str_Name = str_Name;
	}

};

#endif /* TASK_H_ */
