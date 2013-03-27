/*
 * HideDone.h
 *
 *  Created on: 30.08.2009
 *      Author: breeze
 */

#ifndef HIDEDONE_H_
#define HIDEDONE_H_

#include <QSortFilterProxyModel>
#include <QStringList>

#include "TaskTree.h"

/// \brief Фильтрация задач
///
/// Фильтрация обеспечивается пока по двум параметрам - признаку выполненности и списку категорий
/// Плюс есть т.н. "быстрый фильтр", т.е. возможность фильтравать по вхождению подстроки
class HideDone : public QSortFilterProxyModel
{
	Q_OBJECT

	bool		need_HideDone;
	QStringList	m_Categories;
	QString		str_FastFilter;
public:
	enum MatchFilter
	{
		MF_MATCH,
		MF_DOESNT_MATCH,
		MF_UNKNOWN
	};

	HideDone( QObject *parent = NULL );

	bool getHideDone() const;
	void setHideDone( bool _hideDone );
	const QStringList& getCategories() const;
	void setCategories(const QStringList& m_Categories);
	bool filterAcceptsRow( int sourceRow, const QModelIndex &sourceParent ) const;

	/// Изменяет используемый быстрый фильтр
	void setFastFilter( const QString& _value );
protected:
	/// Возвращает true, если переданая задача (без потомков) соответствует заданным условиям
	MatchFilter filterTask( const TaskItem *_item ) const;
	/// Возвращает true, если переданная задача или её потомки соответствует заданным условиям
	bool filterTaskRecursed( const TaskItem *_item ) const;
	/// Проверяет категории на основе даты начала задачи - TM-REQ-047
	bool checkStartDate( const TaskItem *_item ) const;
};


#endif /* HIDEDONE_H_ */
