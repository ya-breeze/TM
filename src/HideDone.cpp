/*
 * HideDone.cpp
 *
 *  Created on: 15.10.2009
 *      Author: breeze
 */

#include "HideDone.h"

#include "utils.h"

HideDone::HideDone( QObject *parent ) :
	QSortFilterProxyModel( parent ), need_HideDone(true)
{
}

bool HideDone::getHideDone() const
{
	return need_HideDone;
}

void HideDone::setHideDone( bool _hideDone )
{
	this->need_HideDone = _hideDone;
	invalidateFilter();
}

const QStringList& HideDone::getCategories() const
{
	return m_Categories;
}

void HideDone::setCategories(const QStringList& m_Categories)
{
	this->m_Categories = m_Categories;
	invalidateFilter();
}

bool HideDone::filterAcceptsRow( int sourceRow, const QModelIndex &sourceParent ) const
{
	// Проверим hideDone
	if( need_HideDone )
	{
		QModelIndex index1 = sourceModel()->index( sourceRow, 1, sourceParent );
		if( sourceModel()->data( index1 ).toString().contains( "<Done>" ) )
			return false;
	}

	// Проверим категории
	if( !m_Categories.empty() )
	{
		TaskTree *tree = dynamic_cast<TaskTree*>(sourceModel());
		Q_ASSERT(tree);

		QModelIndex index = tree->index( sourceRow, 0, sourceParent );
		TaskItem *task = tree->getItem(index);
		Q_ASSERT(task);
		// TODO Тут должно быть чуть хитрее. Например, если у нас есть потомок уже выполненный с установленной категорией,
		// то мы его всё равно зачтём, а не должны. Видимо нужно идти рекурсивно и спрашивать этой же самой функцией, т.е.
		// с учётом всех фильтров (в данный момент - признак окончания)
		if( !filterCategories(task) )
			return false;
	}

	return true;
}

/// Возвращает true, если у переданного узла, или его потомков есть среди категорий текущие
bool HideDone::filterCategories( TaskItem *_item ) const
{
	Q_ASSERT(_item);

	// Содержит ли текущая задача?
	if( _item->containsCategory(m_Categories) )
		return true;

	// Содержат ли потомки?
	for(int i=0; i<_item->childCount(); ++i)
	{
		TaskItem *item = _item->child(i);
		if( filterCategories(item) )
			return true;
	}

	return false;
}
