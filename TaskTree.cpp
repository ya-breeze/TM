/*
 * TaskTree.cpp
 *
 *  Created on: 21.08.2009
 *      Author: breeze
 */

#include "TaskTree.h"

#include "exceptions.h"

TaskItem::TaskItem( TaskItem *parent )
{
	setParent(parent);
}

TaskItem::TaskItem( const Task& _task, TaskItem *parent )
	: Task(_task)
{
	setParent(parent);
}

TaskItem::~TaskItem()
{
	for(int i=0;i<(int)childItems.size();++i)
		childItems[i]->setParent(parentItem);
	if( parentItem )
		parentItem->removeChild(this);
}

void TaskItem::appendChild(TaskItem *item)
{
	childItems.push_back(item);
}

void TaskItem::removeChild(TaskItem *item)
{
	int index = childIndex(item);
	removeChild(index);
}

void TaskItem::removeChild(int _index)
{
	if( _index>=0 && _index<(int)childItems.size() )
	{
		childItems[_index]->setParent(NULL);
		TaskItemVec::iterator it=childItems.begin();
		for( int i=0;i<_index;++i )
			++it;
		childItems.erase(it);
		//DEBUG("We have " << childItems.size() << " children in " << process().pid() );
	}
	else
	{
		ERROR("Try to delete with incorrect index " << _index << ", we have " << (int)childItems.size());
	}
}

TaskItem* TaskItem::child(int row)
{
	if( row<0 || row>=(int)childItems.size() )
		return NULL;

	return childItems[row];
}

int TaskItem::childCount() const
{
	return childItems.size();
}

int TaskItem::childIndex( TaskItem *_item )
{
	for(int i=0;i<(int)childItems.size();++i)
	{
		if( childItems[i]==_item )
			return i;
	}

	return -1;
}

QVariant TaskItem::data(int _column) const
{
	switch( _column )
	{
		case 0 : return getName();
	}

	return QVariant();
}

int TaskItem::row() const
{
	if( parentItem )
	{
		return parentItem->childIndex(const_cast<TaskItem*>(this));
	}

	return 0;
}

void TaskItem::setParent(TaskItem *_item)
{
	parentItem = _item;
}

TaskItem *TaskItem::parent()
{
	return parentItem;
}






TaskTree::TaskTree( QObject *parent )
	: QAbstractItemModel(parent)
{
	rootItem = PtrTaskItem( new TaskItem(NULL) );
}

TaskTree::~TaskTree()
{
}

QVariant TaskTree::data( const QModelIndex &index, int role ) const
{
	if (!index.isValid())
		return QVariant();

	TaskItem *item = (TaskItem*)index.internalPointer();
	if( !item )
		return QVariant();

	if( role==Qt::DisplayRole )
		return item->data(index.column());

	return QVariant();
}

QVariant TaskTree::headerData( int section, Qt::Orientation orientation, int role ) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
	{
		return tr("empty");
	}

	return QAbstractItemModel::headerData(section, orientation, role);
}

QModelIndex TaskTree::index( int row, int column, const QModelIndex &parent ) const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	TaskItem *parentItem;

	if (!parent.isValid())
		parentItem = rootItem.get();
	else
		parentItem = (TaskItem*)parent.internalPointer();

	if( !parentItem )
		return QModelIndex();

	TaskItem *childItem = parentItem->child(row);
	if (childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}

QModelIndex TaskTree::parent( const QModelIndex &index ) const
{
	if (!index.isValid())
		return QModelIndex();

	TaskItem *childItem = (TaskItem*)index.internalPointer();
	if (!childItem)
	{
		return QModelIndex();
	}
	TaskItem *parentItem = childItem->parent();

	if (!parentItem || parentItem == rootItem.get())
	{
		return QModelIndex();
	}

	return createIndex(parentItem->row(), 0, parentItem);
}

int TaskTree::rowCount( const QModelIndex &parent ) const
{
	TaskItem *parentItem;

	if (!parent.isValid())
		parentItem = rootItem.get();
	else
		parentItem = (TaskItem*)parent.internalPointer();

	if( !parentItem )
		return 0;

	return parentItem->childCount();
}

int TaskTree::columnCount( const QModelIndex& ) const
{
	return 2;
}

void TaskTree::clear()
{
	m_Tasks.clear();
	reset();
}

void TaskTree::addChild( const QModelIndex &_index )
{
	addChild(_index, Task());
}

void TaskTree::addChild( const QModelIndex &_index, const Task& _task )
{
	TaskItem *parent = rootItem.get();
	if( _index.isValid() )
		parent = (TaskItem*)(_index.internalPointer());

	beginInsertRows(_index, parent->childCount(), parent->childCount());
	PtrTaskItem item = PtrTaskItem( new TaskItem(_task, parent) );
	parent->appendChild(item.get());
	m_Tasks[item->getId()] = item;
	endInsertRows();
}

void TaskTree::addChild( const QUuid& _parent, const Task& _task )
{
	TaskMap::iterator it = m_Tasks.find(_parent);
	if( it==m_Tasks.end() )
		ERROR("Unknown task with id=" << _parent);

	TaskItem *parent = it->second.get();
	QModelIndex idx = createIndex(parent->row(), 0, parent);

	addChild(idx, _task);
}


void TaskTree::addSibling( const QModelIndex &_index )
{
	addSibling(_index, Task());
}

void TaskTree::addSibling( const QModelIndex &_index, const Task& _task )
{
	QModelIndex idx = _index;
	TaskItem *parent = rootItem.get();
	if( _index.isValid() )
		parent = (TaskItem*)(_index.internalPointer());
	parent = parent->parent();
	if( !parent || parent==rootItem.get() )
	{
		idx = QModelIndex();
		parent = rootItem.get();
	}
	else
	{
		idx = createIndex(parent->row(), 0, parent);
	}

	beginInsertRows(idx, parent->childCount(), parent->childCount());
	PtrTaskItem item = PtrTaskItem( new TaskItem(_task, parent) );
	parent->appendChild(item.get());
	m_Tasks[item->getId()] = item;
	endInsertRows();
}

void TaskTree::addSibling( const QUuid& _parent, const Task& _task )
{
	TaskMap::iterator it = m_Tasks.find(_parent);
	if( it==m_Tasks.end() )
		ERROR("Unknown task with id=" << _parent);

	TaskItem *parent = it->second.get();
	QModelIndex idx = createIndex(parent->row(), 0, parent);

	addSibling(idx, _task);
}

void TaskTree::delItem( const QModelIndex &_index )
{
	if( !_index.isValid() )
		return;
	TaskItem *item = (TaskItem*)(_index.internalPointer());
	while( item->childCount() )
	{
		TaskItem *child = item->child( item->childCount()-1 );
		delItem( createIndex(child->row(), 0, child) );
	}

	QModelIndex parent = createIndex(item->parent()->row(), 0, item->parent());
	if( item->parent()==rootItem.get() )
		parent = QModelIndex();
	int row = item->row();
	beginRemoveRows(parent, row, row);
	item->parent()->removeChild(item);
	m_Tasks.erase(item->getId());
	endRemoveRows();
}
