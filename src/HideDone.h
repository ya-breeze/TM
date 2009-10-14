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

class HideDone : public QSortFilterProxyModel
{
	Q_OBJECT

	bool		need_HideDone;
	QStringList	m_Categories;
public:
	HideDone( QObject *parent = NULL ) :
		QSortFilterProxyModel( parent ), need_HideDone(true)
	{
		m_Categories << "1:2:3";
	}

	bool getHideDone() const
	{
		return need_HideDone;
	}

	void setHideDone( bool _hideDone )
	{
		this->need_HideDone = _hideDone;
		invalidateFilter();
	}

    const QStringList& getCategories() const
    {
        return m_Categories;
    }

    void setCategories(const QStringList& m_Categories)
    {
        this->m_Categories = m_Categories;
    }

	bool filterAcceptsRow( int sourceRow, const QModelIndex &sourceParent ) const
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
				if( m_Categories.indexOf(lst[i])!=-1 )
				{
					res = true;
					break;
				}
			}
		}

		return res;
	}
};


#endif /* HIDEDONE_H_ */
