/*
 * TaskTree.h
 *
 *  Created on: 21.08.2009
 *      Author: breeze
 */

#ifndef TASKTREE_H_
#define TASKTREE_H_

#include <QAbstractItemModel>
#include <QTreeView>
#include <QAbstractTableModel>
#include <QSortFilterProxyModel>
#include <QList>

#include <tr1/memory>
#include <map>

#include "Task.h"


/// \brief Класс, хранящий данные о одной задаче
class TaskItem : public Task
{
public:
	typedef std::vector<TaskItem*>		TaskItemVec;

	TaskItem( TaskItem *parent = NULL );
	TaskItem( const Task& _task, TaskItem *parent = NULL );
	~TaskItem();

	void appendChild(TaskItem *child);
	void removeChild(TaskItem *child);
	void removeChild(int);

	TaskItem*	child(int row);
	int		childCount() const;
	int		childIndex( TaskItem* );

	QVariant	data(int column) const;
	int		row() const;
	TaskItem*	parent();
	void		setParent(TaskItem*);

private:
	TaskItemVec		childItems;
	TaskItem		*parentItem;
};
typedef std::tr1::shared_ptr<TaskItem>	PtrTaskItem;


class TaskTree : public QAbstractItemModel
{
	Q_OBJECT

	typedef std::map<QUuid, PtrTaskItem> TaskMap;
public:
	TaskTree( QObject *parent = NULL );
	virtual ~TaskTree();

	QVariant data( const QModelIndex &index, int role ) const;
//	Qt::ItemFlags flags( const QModelIndex &index ) const;
	QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
	QModelIndex index( int row, int column, const QModelIndex &parent = QModelIndex() ) const;
	QModelIndex parent( const QModelIndex &index ) const;
	int rowCount( const QModelIndex &parent = QModelIndex() ) const;
	int columnCount( const QModelIndex &parent = QModelIndex() ) const;


	void addChild( const QModelIndex &index );
	void addChild( const QModelIndex &index, const Task& _task );
	void addChild( const QUuid& _parent, const Task& _task );
	void clear();
protected:
	TaskMap		m_Tasks;
	PtrTaskItem rootItem;
};

#endif /* TASKTREE_H_ */
