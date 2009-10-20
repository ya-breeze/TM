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
class HideDone : public QSortFilterProxyModel
{
	Q_OBJECT

	bool		need_HideDone;
	QStringList	m_Categories;
public:
	HideDone( QObject *parent = NULL );

	bool getHideDone() const;
	void setHideDone( bool _hideDone );
    const QStringList& getCategories() const;
    void setCategories(const QStringList& m_Categories);
	bool filterAcceptsRow( int sourceRow, const QModelIndex &sourceParent ) const;

protected:
	/// Возвращает true, если у переданного узла, или его потомков есть среди категорий текущие
	bool filterCategories( TaskItem *_item ) const;
};


#endif /* HIDEDONE_H_ */
