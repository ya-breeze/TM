/*
 * ActTree.h
 *
 *  Created on: 22.08.2009
 *      Author: breeze
 */

#ifndef ACTTREE_H_
#define ACTTREE_H_

#include <QAbstractItemModel>
#include "Activities.h"

class ActTree : public QAbstractItemModel
{
	Q_OBJECT

	Activities	*p_Activities;
public:
	ActTree( Activities *_acts, QObject *parent = NULL );

	QVariant data( const QModelIndex &index, int role ) const;
	bool setData( const QModelIndex& index, const QVariant& value, int role = Qt::EditRole );

	Qt::ItemFlags flags( const QModelIndex &index ) const;
	QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
	QModelIndex index( int row, int column, const QModelIndex &parent = QModelIndex() ) const;
	QModelIndex parent( const QModelIndex &index ) const;
	int rowCount( const QModelIndex &parent = QModelIndex() ) const;
	int columnCount( const QModelIndex &parent = QModelIndex() ) const;
protected:
	bool	findAct(const QModelIndex& _idx, QDate&);

protected slots:
	void 	actAdded(const Activity& _act, bool _setCurrent);
	void 	todayChanged(const QDate& _today);
};

#endif /* ACTTREE_H_ */
