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

class CaterotyItem
{
public:
	typedef std::deque<CaterotyItem*> CaterotyItemVec;

	CaterotyItem( CaterotyItem *parent = NULL );
	CaterotyItem( const QString& _name, CaterotyItem *parent = NULL );
	~CaterotyItem();

	void appendChild(CaterotyItem *child);
//	void removeChild(CaterotyItem *child);
//	void removeChild(int);
//	void swapChilds(int _one, int _second);

	CaterotyItem*	child(int row);
	int		childCount() const;
	int		childIndex( CaterotyItem* );

	QVariant	data(int column) const;
	int		row() const;
	CaterotyItem*	parent();
	void		setItemParent(CaterotyItem*);

private:
	CaterotyItemVec		childItems;
	CaterotyItem		*parentItem;
};
typedef shared_ptr<CaterotyItem>	PtrCaterotyItem;




class CategoryTree : public QAbstractItemModel
{
	Q_OBJECT

public:
	CategoryTree( QObject *parent = NULL );
	virtual ~CategoryTree();


	QVariant data( const QModelIndex &index, int role ) const;
	bool setData( const QModelIndex& index, const QVariant& value, int role = Qt::EditRole );

	QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
	QModelIndex index( int row, int column, const QModelIndex &parent = QModelIndex() ) const;
	QModelIndex parent( const QModelIndex &index ) const;
	int rowCount( const QModelIndex &parent = QModelIndex() ) const;
	int columnCount( const QModelIndex &parent = QModelIndex() ) const;

//	QModelIndex addChild( const QModelIndex &index );
//	QModelIndex addChild( const QModelIndex &index, const Task& _task );
//	QModelIndex addChild( const QUuid& _parent, const Task& _task );

//	void delItem( const QModelIndex &index );
//
//	TaskItem* getItem(QModelIndex _idx) const;
//	TaskItem* getItem( const QUuid& _id) const;
//	QModelIndex getItemIndex( const QUuid& _id) const;

	void	clear();
};

#endif /* CATEGORYTREE_H_ */
