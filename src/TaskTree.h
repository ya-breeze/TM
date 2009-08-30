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

#include <map>

#include "smart.h"
#include "Task.h"
#include "ChangableObject.h"


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
	void swapChilds(int _one, int _second);

	TaskItem*	child(int row);
	int		childCount() const;
	int		childIndex( TaskItem* );

	QVariant	data(int column) const;
	int		row() const;
	TaskItem*	parent();
	void		setItemParent(TaskItem*);

private:
	TaskItemVec		childItems;
	TaskItem		*parentItem;
};
typedef shared_ptr<TaskItem>	PtrTaskItem;


class TaskTree : public QAbstractItemModel, public ChangableObject
{
	Q_OBJECT

	typedef std::map<QUuid, PtrTaskItem> TaskMap;
public:
	TaskTree( QObject *parent = NULL );
	virtual ~TaskTree();

	QVariant data( const QModelIndex &index, int role ) const;
	bool setData( const QModelIndex& index, const QVariant& value, int role = Qt::EditRole );

	Qt::ItemFlags flags( const QModelIndex &index ) const;
	QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
	QModelIndex index( int row, int column, const QModelIndex &parent = QModelIndex() ) const;
	QModelIndex parent( const QModelIndex &index ) const;
	int rowCount( const QModelIndex &parent = QModelIndex() ) const;
	int columnCount( const QModelIndex &parent = QModelIndex() ) const;


	QModelIndex addChild( const QModelIndex &index );
	QModelIndex addChild( const QModelIndex &index, const Task& _task );
	QModelIndex addChild( const QUuid& _parent, const Task& _task );

	QModelIndex addSibling( const QModelIndex &index );
	QModelIndex addSibling( const QModelIndex &index, const Task& _task );
	QModelIndex addSibling( const QUuid& _parent, const Task& _task );

	void delItem( const QModelIndex &index );

	TaskItem* getItem(QModelIndex _idx) const;
	TaskItem* getItem( const QUuid& _id) const;
	QModelIndex getItemIndex( const QUuid& _id) const;

	void	clear();

	/// Посылает сигнал о изменении данных в указанном индексе
	void	setDataChanged( const QModelIndex& _index );
	void	setDataChanged( TaskItem *_item );

	void	moveUp( const QModelIndex& _index );
	void	moveDown( const QModelIndex& _index );
	void	moveLeft( const QModelIndex& _index );
	void	moveRight( const QModelIndex& _index );

public slots:
protected:
	TaskMap		m_Tasks;
	PtrTaskItem	rootItem;
};

#endif /* TASKTREE_H_ */
