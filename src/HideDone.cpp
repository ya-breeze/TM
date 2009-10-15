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
	bool res = true;
	// Проверим hideDone
	if( !need_HideDone )
	{
		res = true;
	}
	else
	{
		QModelIndex index1 = sourceModel()->index( sourceRow, 1, sourceParent );
		res = !sourceModel()->data( index1 ).toString().contains( "<Done>" );
	}
	if( !res )
		return false;

	// Проверим категории
	if( !m_Categories.empty() )
	{
		res = false;
		TaskTree *tree = dynamic_cast<TaskTree*>(sourceModel());
		Q_ASSERT(tree);

		QModelIndex index = tree->index( sourceRow, 0, sourceParent );
		TaskItem *task = tree->getItem(index);
		Q_ASSERT(task);
		const QStringList& lst = task->getCategories();
		for(int i=0; i<lst.size();++i)
		{
//			DEBUG("item " << task->getName() << " has cat - " << lst[i]);
			if( m_Categories.indexOf(lst[i])!=-1 )
			{
//				DEBUG("Find such cat in filter");
				return true;
			}
		}
	}

	return res;
}
