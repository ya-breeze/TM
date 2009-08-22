/*
 * ActTree.cpp
 *
 *  Created on: 22.08.2009
 *      Author: breeze
 */

#include "ActTree.h"


ActTree::ActTree( Activities *_acts, QObject *parent )
	: QAbstractItemModel(parent), p_Activities(_acts)
{
	connect(p_Activities, SIGNAL(ActAdded(const Activity&, bool)), this, SLOT(ActAdded(const Activity&, bool)));
	connect(p_Activities, SIGNAL(todayChanged(const QDate&)), this, SLOT(todayChanged(const QDate&)));
}

QVariant ActTree::data( const QModelIndex &index, int role ) const
{
	return QVariant();
}

bool ActTree::setData( const QModelIndex& index, const QVariant& value, int role )
{
	return false;
}

Qt::ItemFlags ActTree::flags( const QModelIndex &index ) const
{
	if (!index.isValid())
		return 0;

	return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QVariant ActTree::headerData( int section, Qt::Orientation orientation, int role ) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
	{
		return tr("empty");
	}

	return QAbstractItemModel::headerData(section, orientation, role);
}

QModelIndex ActTree::index( int row, int column, const QModelIndex &parent ) const
{

}

QModelIndex ActTree::parent( const QModelIndex &index ) const
{

}

int ActTree::rowCount( const QModelIndex &parent ) const
{

}

int ActTree::columnCount( const QModelIndex &parent ) const
{
	return 2;
}

void ActTree::actAdded(const Activity& _act, bool _setCurrent)
{

}

void ActTree::todayChanged(const QDate& _today)
{

}
