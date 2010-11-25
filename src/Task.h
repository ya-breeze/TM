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

	QDateTime	m_LocalUpdated;
	QDateTime	m_GlobalUpdated;

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
	}

	void setFinished( const QDateTime& m_Finished )
	{
		this->m_Finished = m_Finished;
	}

	void setStarted( const QDateTime& m_Started )
	{
		this->m_Started = m_Started;
	}

	QString getNotes() const
	{
		return str_Notes;
	}

	void setNotes( QString str_Notes )
	{
		this->str_Notes = str_Notes;
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
	}

	void setParentId( QUuid m_Parent )
	{
		this->m_Parent = m_Parent;
	}

	void setName( const QString& str_Name )
	{
		this->str_Name = str_Name;
	}

	int getParentIndex() const {
	    return m_ParentIndex;
	}
	void setParentIndex(int _idx) {
	    m_ParentIndex = _idx;
	}

	QDateTime getLocalUpdated() const {
		return m_LocalUpdated;
	}
	void setLocalUpdated( const QDateTime& _LocalUpdated ) {
		m_LocalUpdated = _LocalUpdated;
	}

	QDateTime getGlobalUpdated() const {
		return m_GlobalUpdated;
	}
	void setGlobalUpdated( const QDateTime& _GlobalUpdated ) {
		m_GlobalUpdated = _GlobalUpdated;
	}
    const QStringList& getCategories() const
    {
	return m_Categories;
    }

    void setCategories(const QStringList& m_Categories)
    {
	this->m_Categories = m_Categories;
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
    }
};

#endif /* TASK_H_ */
