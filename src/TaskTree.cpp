/*
 * TaskTree.cpp
 *
 *  Created on: 21.08.2009
 *      Author: breeze
 */

#include "TaskTree.h"

#include "utils.h"

TaskItem::TaskItem( TaskItem *parent )
{
	setItemParent(parent);
}

TaskItem::TaskItem( const Task& _task, TaskItem *parent )
	: Task(_task)
{
	setItemParent(parent);
}

TaskItem::~TaskItem()
{
	for(int i=0;i<(int)childItems.size();++i)
		childItems[i]->setItemParent(parentItem);
	if( parentItem )
		parentItem->removeChild(this);
}

void TaskItem::appendChild(TaskItem *item)
{
	childItems.push_back(item);
}

void TaskItem::removeChild(TaskItem *item)
{
	int index = childIndex(item, false);
	if( index!=-1)
		removeChild(index);
}

void TaskItem::removeChild(int _index)
{
	if( _index>=0 && _index<(int)childItems.size() )
	{
		childItems[_index]->setItemParent(NULL);
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

TaskItem* TaskItem::child(int row, bool _hideDone)
{
	if( row<0 || row>=(int)childItems.size() )
		return NULL;

	if( !_hideDone )
		return childItems[row];

	int res = -1;
	for(int i=0;i<(int)childItems.size();++i)
	{
		if( childItems[i]->getFinished().isNull() )
			++res;
		if( row==res )
			return childItems[i];
	}

	return NULL;
}

int TaskItem::childCount(bool _hideDone) const
{
	if( !_hideDone )
		return childItems.size();

	int res = 0;
	for(int i=0;i<(int)childItems.size();++i)
	{
		if( childItems[i]->getFinished().isNull() )
			++res;
	}

	return res;
}

int TaskItem::childIndex( TaskItem *_item, bool _hideDone )
{
	int res = 0;
	for(int i=0;i<(int)childItems.size();++i)
	{
		if( childItems[i]==_item )
			return res;

		if( !_hideDone )
			++res;
		else if( childItems[i]->getFinished().isNull() )
			++res;
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

int TaskItem::row(bool _hideDone) const
{
	if( parentItem )
	{
		return parentItem->childIndex(const_cast<TaskItem*>(this), _hideDone);
	}

	return 0;
}

void TaskItem::setItemParent(TaskItem *_item)
{
	parentItem = _item;
	if( _item )
		setParentId(_item->getId());
	else
		setParentId(QUuid());
}

TaskItem *TaskItem::parent()
{
	return parentItem;
}






TaskTree::TaskTree( QObject *parent )
	: QAbstractItemModel(parent), ChangableObject(), need_HideDone(false)
{
	rootItem = PtrTaskItem( new TaskItem(NULL) );
	rootItem->setId("{00000000-0000-0000-0000-000000000000}");
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
	{
		switch( index.column() )
		{
			case 0 :
			{
				QString res = item->getName();

				return res;
			}
			break;
			case 1 :
			{
				QString res;
				if( !item->getFinished().isNull() )
					res += "<Done>";
				else if( !item->getStarted().isNull() )
					res += "<Working>";

				return res;
			}
			break;
		}
	}

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

	TaskItem *childItem = parentItem->child(row, need_HideDone);
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

	return createIndex(parentItem->row(need_HideDone), 0, parentItem);
}

int TaskTree::rowCount( const QModelIndex &parent ) const
{
	TaskItem *parentItem;

	if (!parent.isValid())
		parentItem = rootItem.get();
	else
		parentItem = (TaskItem*)parent.internalPointer();

	if( !parentItem )
	{
		return 0;
	}

	int sz = parentItem->childCount(need_HideDone);
	return sz;
}

int TaskTree::columnCount( const QModelIndex& ) const
{
	return 2;
}

void TaskTree::clear()
{
	m_Tasks.clear();

	setChanged();
	reset();
}

QModelIndex TaskTree::addChild( const QModelIndex &_index )
{
	return addChild(_index, Task());
}

QModelIndex TaskTree::addChild( const QModelIndex &_index, const Task& _task )
{
	TaskItem *parent = rootItem.get();
	if( _index.isValid() )
		parent = (TaskItem*)(_index.internalPointer());

	int count = parent->childCount(need_HideDone);
	beginInsertRows(_index, count, count);
	PtrTaskItem item = PtrTaskItem( new TaskItem(_task, parent) );
	parent->appendChild(item.get());
	m_Tasks[item->getId()] = item;
	endInsertRows();

	setChanged();
	return createIndex(item->row(need_HideDone), 0, item.get());
}

QModelIndex TaskTree::addChild( const QUuid& _parent, const Task& _task )
{
	TaskMap::iterator it = m_Tasks.find(_parent);
	if( it==m_Tasks.end() && !_parent.isNull() )
		ERROR("Unknown task with id=" << _parent);

	TaskItem *parent = rootItem.get();
	QModelIndex idx;
	if( !_parent.isNull() )
	{
		parent = it->second.get();
		idx = createIndex(parent->row(need_HideDone), 0, parent);
	}

	return addChild(idx, _task);
}


QModelIndex TaskTree::addSibling( const QModelIndex &_index )
{
	return addSibling(_index, Task());
}

QModelIndex TaskTree::addSibling( const QModelIndex &_index, const Task& _task )
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
		idx = createIndex(parent->row(need_HideDone), 0, parent);
	}

	int count = parent->childCount(need_HideDone);
	beginInsertRows(idx, count, count);
	PtrTaskItem item = PtrTaskItem( new TaskItem(_task, parent) );
	parent->appendChild(item.get());
	m_Tasks[item->getId()] = item;
	endInsertRows();

	setChanged();
	return createIndex(item->row(need_HideDone), 0, item.get());
}

QModelIndex TaskTree::addSibling( const QUuid& _parent, const Task& _task )
{
	TaskMap::iterator it = m_Tasks.find(_parent);
	if( it==m_Tasks.end() )
		ERROR("Unknown task with id=" << _parent);

	TaskItem *parent = it->second.get();
	QModelIndex idx = createIndex(parent->row(need_HideDone), 0, parent);

	return addSibling(idx, _task);
}

void TaskTree::delItem( const QModelIndex &_index )
{
	if( !_index.isValid() )
		return;
	TaskItem *item = (TaskItem*)(_index.internalPointer());
	while( item->childCount(need_HideDone) )
	{
		TaskItem *child = item->child( item->childCount(need_HideDone)-1, need_HideDone );
		delItem( createIndex(child->row(need_HideDone), 0, child) );
	}

	QModelIndex parent = createIndex(item->parent()->row(need_HideDone), 0, item->parent());
	if( item->parent()==rootItem.get() )
		parent = QModelIndex();
	int row = item->row(need_HideDone);
	beginRemoveRows(parent, row, row);
	item->parent()->removeChild(item);
	m_Tasks.erase(item->getId());
	endRemoveRows();

	setChanged();
}

TaskItem* TaskTree::getItem(QModelIndex _idx) const
{
	if (!_idx.isValid())
		return NULL;

	TaskItem *item = (TaskItem*)(_idx.internalPointer());
	return item;
}

QModelIndex TaskTree::getItemIndex( const QUuid& _id) const
{
	DEBUG("Searching for task with id=" << _id);
	TaskMap::const_iterator it=m_Tasks.find(_id);
	if( it!=m_Tasks.end() )
	{
		return createIndex( it->second->row(need_HideDone), 0, it->second.get() );
	}

	return QModelIndex();
}

TaskItem* TaskTree::getItem( const QUuid& _id ) const
{
//	DEBUG("Searching for task with id=" << _id);
	TaskMap::const_iterator it=m_Tasks.find(_id);
	if( it!=m_Tasks.end() )
		return it->second.get();

	return NULL;
}

Qt::ItemFlags TaskTree::flags( const QModelIndex &index ) const
{
	if (!index.isValid())
		return 0;

	return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable
		| Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
}

bool TaskTree::setData( const QModelIndex& _index, const QVariant& _value, int _role )
{
	if( !_index.isValid() || _role!=Qt::EditRole )
		return false;

	TaskItem *item = (TaskItem*)(_index.internalPointer());

	switch( _index.column() )
	{
		case 0 : item->setName( _value.toString() );
	}


	setDataChanged(item);

	return true;
}

void TaskTree::setDataChanged( TaskItem *_item )
{
	int row = _item->row(need_HideDone);
	QModelIndex index1 = createIndex( row, 0, _item );
	QModelIndex index2 = createIndex( row, columnCount(QModelIndex()), _item );

	emit dataChanged(index1, index2);

	setChanged();
}

void TaskTree::setDataChanged( const QModelIndex& _index )
{
	TaskItem *item = getItem(_index);
	if( item )
		setDataChanged(item);
}

void TaskTree::setHideDone(int _value)
{
//	if( _value!=need_HideDone )
//	{
//		need_HideDone = _value;
//		reset();
//	}
}

bool TaskTree::getHideDone() const
{
	return need_HideDone;
}

void TaskTree::moveUp( const QModelIndex& _index )
{
	if( !_index.isValid() )
		return;

	// Если не первый, то можем просто сдвинуть вверх
	if( _index.row() )
	{
//		TaskItem *item = (TaskItem*)(_index.internalPointer());
//		TaskItem *parent = item->parent();
//		parent->swapChilds(_index.row(), _index.row()-1);
	}
	else
	{

	}
}

void TaskTree::moveDown( const QModelIndex& _index )
{
	if( !_index.isValid() )
		return;

}

void TaskTree::moveLeft( const QModelIndex& _index )
{
	if( !_index.isValid() )
		return;

}

void TaskTree::moveRight( const QModelIndex& _index )
{
	if( !_index.isValid() )
		return;

}

/// Меняет местами детей по их индексам
void TaskItem::swapChilds(int _one, int _second)
{
	std::swap( childItems[_one], childItems[_second]);
}
