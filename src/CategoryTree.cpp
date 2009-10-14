/*
 * CategoryTree.cpp
 *
 *  Created on: 18.09.2009
 *      Author: breeze
 */

#include "CategoryTree.h"

CategoryTree::CategoryTree( QObject *parent)
	: QStandardItemModel(parent)
{
}


QModelIndex CategoryTree::addCategory(const Category& _cat)
{
	QStandardItem *parentItem = invisibleRootItem();
	for(int i=0;i<_cat.deep()+1;++i)
		parentItem = addCategory(parentItem, _cat.partialName(i+1));

	return parentItem->index();
}

CaterotyItem::CaterotyItem( const Category& _cat )
	: Category(_cat)
{
	setText( name() );
}

QStandardItem* CategoryTree::addCategory( QStandardItem *_item, const QString& _cat )
{
	for(int i=0;i<_item->rowCount();++i)
	{
		if( _item->child(i,0)->text()==_cat )
		{
			return _item->child(i,0);
		}
	}

	// Такого дитя нет - надо добавить
	QString childName(_cat);
	if( _item->parent() )
	{
		CaterotyItem *catItem = dynamic_cast<CaterotyItem*>(_item);
		Q_ASSERT(catItem);
		childName = catItem->childName(_cat);
	}

	QStandardItem *item = new CaterotyItem( childName );
	_item->appendRow(item);

	return item;
}

QModelIndex CategoryTree::addChild( const QModelIndex &index, const QString& _name )
{
	QStandardItem *item = itemFromIndex(index);
	if( !index.isValid() )
		item = invisibleRootItem();
	Q_ASSERT(item);

	return addCategory(item, _name)->index();
}

/// Сбрасывает выбранные категории
void CategoryTree::clearCheckboxes()
{

}
