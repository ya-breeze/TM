/*
 * CategoryTree.cpp
 *
 *  Created on: 18.09.2009
 *      Author: breeze
 */

#include "CategoryTree.h"

#include "utils.h"

CategoryTree::CategoryTree( QObject *parent)
	: QStandardItemModel(parent), is_Checkable(false)
{
	QStringList lst;
	lst << tr("Category name");
	setHorizontalHeaderLabels(lst);
}


QModelIndex CategoryTree::addCategory(const Category& _cat)
{
	QStandardItem *parentItem = invisibleRootItem();
	for(int i=0;i<_cat.deep();++i)
		parentItem = checkExists(parentItem, _cat.parentName(i));

	parentItem = checkExists(parentItem, _cat.name());

	return parentItem->index();
}

CaterotyItem::CaterotyItem( const Category& _cat )
	: Category(_cat)
{
	setText( name() );
//	setText( fullName() );
}

QStandardItem* CategoryTree::checkExists( QStandardItem *_item, const QString& _cat, bool _addOnMiss )
{
//	DEBUG("parent - " << _item->text() << " : " << _cat);

	for(int i=0;i<_item->rowCount();++i)
	{
		if( _item->child(i,0)->text()==_cat )
		{
//			DEBUG("Finded");
			return _item->child(i,0);
		}
	}

	// Такого дитя нет
	if( !_addOnMiss )
		return NULL;

	// надо добавить
	QString childName(_cat);
	if( indexFromItem(_item).isValid() )
	{
		CaterotyItem *catItem = dynamic_cast<CaterotyItem*>(_item);
		Q_ASSERT(catItem);
		childName = catItem->childName(_cat);
	}

//	DEBUG("real add " << childName << " for " << _item->text());

	QStandardItem *item = new CaterotyItem( childName );
	item->setCheckable(is_Checkable);
	_item->appendRow(item);

	return item;
}

QModelIndex CategoryTree::addChild( const QModelIndex &index, const QString& _name )
{
	QStandardItem *item = itemFromIndex(index);
	QString name(_name);
	if( !index.isValid() )
	{
		item = invisibleRootItem();
	}
	else
	{
		CaterotyItem *catItem = dynamic_cast<CaterotyItem*>(item);
		Q_ASSERT(catItem);
		name = catItem->childName(_name);
//		DEBUG("addChild - " << name << " for " << _name);
	}
	Q_ASSERT(item);

	return checkExists(item, _name)->index();
}

/// Сбрасывает выбранные категории
void CategoryTree::clearCheckboxes(QStandardItem *_item)
{
	if( !_item )
		_item = invisibleRootItem();
	for(int i=0;i<_item->rowCount();++i)
	{
		QStandardItem *item = _item->child(i, 0);
		item->setCheckState(Qt::Unchecked);
		clearCheckboxes(item);
	}
}

/// Выставляет свойство checkable у всех категорий
void CategoryTree::setCheckable(bool _value, QStandardItem *_item)
{
	is_Checkable = _value;
	if( !_item )
		_item = invisibleRootItem();
	for(int i=0;i<_item->rowCount();++i)
	{
		QStandardItem *item = _item->child(i, 0);
		item->setCheckable(_value);

		setCheckable(_value, item);
	}
}

/// Выставляет флажок для указанной категории
void CategoryTree::setChecked( const QString& _name, Qt::CheckState _value )
{
	Category cat(_name);
	QStandardItem *item = find(cat);
	if( item )
	{
//		DEBUG("We find it!");
		item->setCheckState( _value );
	}
}

QStandardItem* CategoryTree::find( const Category& _cat )
{
//	DEBUG("Looking for " << _cat.fullName());
	QStandardItem *parentItem = invisibleRootItem();
	for(int i=0;i<_cat.deep()+1;++i)
	{
		if( !parentItem )
			return NULL;
		parentItem = checkExists(parentItem, _cat.parentName(i), false );
	}

	return parentItem;
}

/// Возвращает список выбранных категорий для указанного родителя
QStringList	CategoryTree::checkedList(QStandardItem *_item) const
{
	if( !_item )
		_item = invisibleRootItem();

	QStringList lst;
	checkedList(_item, lst);

	return lst;
}

/// Возвращает список выбранных категорий для указанного родителя
void CategoryTree::checkedList(QStandardItem *_item, QStringList& _lst) const
{
	for(int i=0;i<_item->rowCount();++i)
	{
		QStandardItem *item = _item->child(i, 0);
		if( item->checkState()==Qt::Checked )
		{
			CaterotyItem *catItem = dynamic_cast<CaterotyItem*>(item);
			Q_ASSERT(catItem);
			_lst << catItem->fullName();
//			DEBUG("checked - " << catItem->fullName());
		}

		checkedList(item, _lst);
	}
}
