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

	/// Добавляет категорию
	QModelIndex	addCategory( const Category& _cat );
	/// Добавляет потомка для указанного индекса
	QModelIndex addChild( const QModelIndex &index, const QString& _name );
	/// Сбрасывает выбранные категории для указанного родителя
	void		clearCheckboxes(QStandardItem *_item = NULL);
	/// Выставляет свойство checkable у всех категорий для указанного родителя
	void		setCheckable(bool _value, QStandardItem *_item = NULL);
	/// Выставляет флажок для указанной категории
	void		setChecked( const QString& _name, Qt::CheckState _value = Qt::Checked );
	/// Возвращает список выбранных категорий для указанного родителя
	QStringList	checkedList(QStandardItem *_item = NULL) const;

protected:
	/// Проверяет, что переданная категория-потомок существует. Если не существует и _addOnMiss==true, то явно добавляет потомка.
	/// В качестве _cat должно передаваться название категории на этом уровне, т.е. БЕЗ ":"
	QStandardItem*	checkExists( QStandardItem *_item, const QString& _cat, bool _addOnMiss = true );
	/// Ищет указанную категорию. Если категории нет - возвращает NULL
	QStandardItem*	find( const Category& _cat );
	/// Возвращает список выбранных категорий для указанного родителя
	void checkedList(QStandardItem *_item, QStringList& _lst) const;

	/// В это значение выставляется checkable создаваемых категорий - чтобы в режиме списка не давать check, а в режиме
	/// задачи - давать
	bool			is_Checkable;
};

#endif /* CATEGORYTREE_H_ */
