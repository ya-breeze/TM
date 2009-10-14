/*
 * CategoryTree.h
 *
 *  Created on: 18.09.2009
 *      Author: breeze
 */

#ifndef CATEGORYTREE_H_
#define CATEGORYTREE_H_

#include "smart.h"
#include <QString>
#include <QObject>
#include <deque>
#include <QModelIndex>
#include <QStandardItem>
#include <QStandardItemModel>

#include "Category.h"

class CaterotyItem : public QStandardItem, public Category
{
public:
		CaterotyItem( const Category& _cat );
};

class CategoryTree : public QStandardItemModel
{
	Q_OBJECT

public:
	CategoryTree( QObject *parent = NULL );

////	QModelIndex addChild( const QModelIndex &index );
////	QModelIndex addChild( const QUuid& _parent, const Task& _task );
//
////	void delItem( const QModelIndex &index );
////
////	TaskItem* getItem(QModelIndex _idx) const;
////	TaskItem* getItem( const QUuid& _id) const;
////	QModelIndex getItemIndex( const QUuid& _id) const;

	QModelIndex	addCategory( const Category& _cat );
	QModelIndex addChild( const QModelIndex &index, const QString& _name );

protected:
	QStandardItem*	addCategory( QStandardItem *_item, const QString& _cat );
};

#endif /* CATEGORYTREE_H_ */
