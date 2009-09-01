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
	item->setItemParent(this);
}

void TaskItem::insertChild(int _index, TaskItem *_child)
{
	Q_ASSERT(_index>=0);
//	DEBUG("index - " << _index << " for " << _child->getName() << ". Child count - " << childCount());

	if( _index>=childCount() )
	{
		appendChild(_child);
		return;
	}

	int index = 0;
	for(TaskItemVec::iterator it=childItems.begin();it!=childItems.end();++it)
	{
		if( _index==index )
		{
			childItems.insert(it, _child);
			_child->setItemParent(this);
			return;
		}
		++index;
	}

	ERROR("Wrong index in insertChild");
}

void TaskItem::removeChild(TaskItem *item)
{
	int index = childIndex(item);
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
	: QAbstractItemModel(parent), ChangableObject()
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
	{
		return 0;
	}

	int sz = parentItem->childCount();
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

	int count = parent->childCount();
	beginInsertRows(_index, count, count);
	PtrTaskItem item = PtrTaskItem( new TaskItem(_task, parent) );
	parent->appendChild(item.get());
	m_Tasks[item->getId()] = item;
	endInsertRows();

	setChanged();
	return createIndex(item->row(), 0, item.get());
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
		idx = createIndex(parent->row(), 0, parent);
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
		idx = createIndex(parent->row(), 0, parent);
	}

	int count = parent->childCount();
	beginInsertRows(idx, count, count);
	PtrTaskItem item = PtrTaskItem( new TaskItem(_task, parent) );
	parent->appendChild(item.get());
	m_Tasks[item->getId()] = item;
	endInsertRows();

	setChanged();
	return createIndex(item->row(), 0, item.get());
}

QModelIndex TaskTree::addSibling( const QUuid& _parent, const Task& _task )
{
	TaskMap::iterator it = m_Tasks.find(_parent);
	if( it==m_Tasks.end() )
		ERROR("Unknown task with id=" << _parent);

	TaskItem *parent = it->second.get();
	QModelIndex idx = createIndex(parent->row(), 0, parent);

	return addSibling(idx, _task);
}

void TaskTree::delItem( const QModelIndex &_index )
{
	if( !_index.isValid() )
		return;
	TaskItem *item = (TaskItem*)(_index.internalPointer());
	Q_ASSERT(item);

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
		return createIndex( it->second->row(), 0, it->second.get() );
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

//void TaskTree::setDataRecurseChanged( const QModelIndex& _index )
//{
//	QModelIndex parentidx = parent(_index);
//
//	beginRemoveRows(parentidx, _index.row(), _index.row());
//	endInsertRows();
//
//	beginInsertRows(parentidx, _index.row(), _index.row());
//	endInsertRows();
//
//	setChanged();
//}

void TaskTree::setDataChanged( TaskItem *_item )
{
	int row = _item->row();
	QModelIndex index1 = createIndex( row, 0, _item );
	QModelIndex index2 = createIndex( row, columnCount(QModelIndex()), _item );

	emit dataChanged(index1, index2);

	// childs
	for(int i=0;i<_item->childCount();++i)
	{
		setDataChanged(_item->child(i));
	}

	setChanged();
}

void TaskTree::setDataChanged( const QModelIndex& _index )
{
	TaskItem *item = getItem(_index);
	if( item )
		setDataChanged(item);
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

bool TaskTree::dropMimeData( const QMimeData */*data*/, Qt::DropAction /*action*/, int row, int column, const QModelIndex & parent )
{
	DEBUG("row " << row << ", column " << column << ", parent " << parent.isValid() );

	return false;
}

//void TaskTree::swapTasks( const QModelIndex& _one, const QModelIndex& _two )
//{
//	QModelIndex parent1 = parent(_one);
//	QModelIndex parent2 = parent(_two);
//
//	if( parent1==parent2 )
//	{
//		TaskItem *parent = (TaskItem*)(parent1.internalPointer());
//		parent->swapChilds(_one.row(), _two.row());
//		setDataChanged(_one);
//		setDataChanged(_two);
//	}
//}

void TaskTree::moveTask( const QModelIndex& _task, const QModelIndex& _parent, int _row)
{
	if( !_task.isValid() )
		ERROR("Try to move task with invalid index");

	QModelIndex parentCurr = parent(_task);
	TaskItem *task = (TaskItem*)(_task.internalPointer());
	TaskItem *parent = (TaskItem*)(parentCurr.internalPointer());
	DEBUG("Moving '" << task->getName() << "' to '" << parent->getName() << "':"<< _row <<" childs - " << parent->childCount());

	int offset = 0;
	if( parentCurr==_parent )
	{
		if( _task.row()==_row )
			return;
		if( _task.row()<_row )
			offset = 2;

		// В какое место вставляем?
		if( _row>=parent->childCount() )
		{
TRACE;
			// Вставляем в конец - тут всё просто, т.к. никого не придётся двигать

			// Оторвём от текущёго родителя
			beginRemoveRows(parentCurr, _task.row(), _task.row());
			parent->removeChild(_task.row());
			endRemoveRows();

			// Вставим к новому родителю
			beginInsertRows(parentCurr, _row-offset, _row-offset);
			parent->appendChild(task);
			endInsertRows();
		}
		else
		{
TRACE;
			// Вставляем в середину - придётся замещать
			// кого замещаем?
			TaskItem *replaced = parent->child(_row);

			// Оторвём от текущёго родителя того, кого двигаем
			beginRemoveRows(parentCurr, _task.row(), _task.row());
			DEBUG("Remove '" << task->getName() << "' from '" << parent->getName() << "':"<< _task.row());
			parent->removeChild(_task.row());
			endRemoveRows();

			// Оторвём от родителя того, кого замещаем
			beginRemoveRows(parentCurr, replaced->row(), replaced->row());
			DEBUG("Remove '" << replaced->getName() << "' from '" << parent->getName() << "':"<< replaced->row());
			parent->removeChild(replaced);
			endRemoveRows();

			// Теперь вставляем обоих
			beginInsertRows(parentCurr, _row-offset, _row-offset+1);
			DEBUG("Total childs " << parent->childCount() << ", inserting to " << _row-1-offset << ":" << _row-offset);
			parent->insertChild(_row-offset, task);
			parent->insertChild(_row-offset+1, replaced);
			endInsertRows();
		}
//
//
//		parent->swapChilds(_task.row(), _row);
//		setDataChanged(_task);
//		setDataChanged( index(_row, 0, parentCurr) );
	}

}
