/*
 * Task.h
 *
 *  Created on: 21.08.2009
 *      Author: breeze
 */

#ifndef TASK_H_
#define TASK_H_

#include <vector>

#include <QString>
#include <QUuid>
#include <QDateTime>

#include "Category.h"

class Task
{
	typedef std::vector<QUuid> UuidVec;

	////////////////// Properties //////////////////
	QString		str_Name;
	QUuid		m_Id;
	QUuid		m_Parent;
	UuidVec		m_Depends;
	QString		str_Notes;
	QDateTime	m_Created;
	QDateTime	m_Started;
	QDateTime	m_Finished;
	/// Планируемое время выполнения
	QString		m_PlannedTime;
	/// Индекс в родителе - фактически поле для сортировки задач внутри родителя
	int		m_ParentIndex;
	/// Название файла иконки
	QString		str_IconName;

	QDateTime	m_Updated;

	/// Список категорий
	QStringList	m_Categories;
public:
	Task( const QString& _name = "" );

    ///////////////////// Get/Set ///////////////////
	QString getPlannedTime() const
	{
		return m_PlannedTime;
	}

	void setPlannedTime( const QString& m_PlannedTime )
	{
	    m_Updated = QDateTime::currentDateTime();
		this->m_PlannedTime = m_PlannedTime;
	}

	QDateTime getCreated() const
	{
		return m_Created;
	}

	QDateTime getFinished() const
	{
		return m_Finished;
	}

	QDateTime getStarted() const
	{
		return m_Started;
	}

	void setCreated( const QDateTime& m_Created )
	{
		this->m_Created = m_Created;
	    m_Updated = QDateTime::currentDateTime();
	}

	void setFinished( const QDateTime& m_Finished )
	{
		this->m_Finished = m_Finished;
	    m_Updated = QDateTime::currentDateTime();
	}

	void setStarted( const QDateTime& m_Started )
	{
		this->m_Started = m_Started;
	    m_Updated = QDateTime::currentDateTime();
	}

	QString getNotes() const
	{
		return str_Notes;
	}

	void setNotes( QString str_Notes )
	{
		this->str_Notes = str_Notes;
	    m_Updated = QDateTime::currentDateTime();
	}

	UuidVec getDepends() const
	{
		return m_Depends;
	}

	QUuid getId() const
	{
		return m_Id;
	}

	QUuid getParentId() const
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
	    m_Updated = QDateTime::currentDateTime();
	}

	void setParentId( QUuid m_Parent )
	{
		this->m_Parent = m_Parent;
	    m_Updated = QDateTime::currentDateTime();
	}

	void setName( const QString& str_Name )
	{
		this->str_Name = str_Name;
	    m_Updated = QDateTime::currentDateTime();
	}

	int getParentIndex() const {
	    return m_ParentIndex;
	}
	void setParentIndex(int _idx) {
	    m_ParentIndex = _idx;
	    m_Updated = QDateTime::currentDateTime();
	}

	QDateTime getUpdated() const {
		return m_Updated;
	}
	void setUpdated( const QDateTime& _LocalUpdated ) {
		m_Updated = _LocalUpdated;
	}

    const QStringList& getCategories() const
    {
	return m_Categories;
    }

    void setCategories(const QStringList& m_Categories)
    {
	this->m_Categories = m_Categories;
	m_Updated = QDateTime::currentDateTime();
    }

    void addCategory(const QString& _cat)
    {
    	m_Categories << _cat;
    }

    QString getIconName() const
    {
	    return str_IconName;
    }

    void setIconName( QString str_IconName )
    {
	    this->str_IconName = str_IconName;
	m_Updated = QDateTime::currentDateTime();
    }
};

#endif /* TASK_H_ */
