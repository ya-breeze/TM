/*
 * HideDone.h
 *
 *  Created on: 30.08.2009
 *      Author: breeze
 */

#ifndef HIDEDONE_H_
#define HIDEDONE_H_

#include <QSortFilterProxyModel>

class HideDone : public QSortFilterProxyModel
{
	Q_OBJECT

	bool need_HideDone;
public:
	HideDone( QObject *parent = NULL ) :
		QSortFilterProxyModel( parent ), need_HideDone(true)
	{
	}

	bool getHideDone() const
	{
		return need_HideDone;
	}

	void setHideDone( bool _hideDone )
	{
		this->need_HideDone = _hideDone;
//		invalidate();
		invalidateFilter();
	}
	bool filterAcceptsRow( int sourceRow, const QModelIndex &sourceParent ) const
	{
		if( !need_HideDone )
			return true;

		QModelIndex index1 = sourceModel()->index( sourceRow, 1, sourceParent );
		bool res = !sourceModel()->data( index1 ).toString().contains( "<Done>" );

		return res;
	}
};


#endif /* HIDEDONE_H_ */
