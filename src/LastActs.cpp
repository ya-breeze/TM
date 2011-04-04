/*
 * LastActs.cpp
 *
 *  Created on: 22.08.2009
 *      Author: breeze
 */

#include "LastActs.h"

#include "utils.h"

LastActs::LastActs( const TaskTree &_tasks, Activities &_acts, QObject *parent )
	: QAbstractTableModel(parent), p_Activities(_acts), p_Tasks(_tasks)
{
	connect(&p_Activities, SIGNAL(ActAdded(const Activity&, bool)), this, SLOT(actAdded(const Activity&, bool)));
	connect(&p_Activities, SIGNAL(todayChanged(const QDate&)), this, SLOT(todayChanged(const QDate&)));

	todayChanged(_acts.getToday());
}

int LastActs::rowCount( const QModelIndex &_parent ) const
{
//	DEBUG("Today activities - " << acts.count());
	if( !_parent.isValid() )
		return m_Acts.count();

	return 0;
}

int LastActs::columnCount( const QModelIndex &/*_parent*/ ) const
{
	return 2;
}

const Activity& LastActs::getAct( const QModelIndex &_index ) const
{
	size_t index = m_Acts.count() - _index.row() - 1;
	const Activity &act = m_Acts.getActivity(index);

	return act;
}

QVariant LastActs::data( const QModelIndex &_index, int _role ) const
{
	if( _role!=Qt::DisplayRole )
		return QVariant();

	const Activity &act = getAct(_index);
	switch( _index.column() )
	{
		case 0 :
		{
			QString res = act.getStartTime().toString("yyyy.MM.dd hh:mm");
			res += " ";

			if( act.getAssignedTask().isNull() )
				res += act.getName();
			else
			{
				TaskItem *item = p_Tasks.getItem(act.getAssignedTask());
				if( !item )
					res += "<Wrong task in activity>";
				else
					res += item->getName();
			}

			return  res;
		}
	}

	return QVariant();
}


void LastActs::actAdded(const Activity& _act, bool _setCurrent)
{
	if( _act.getStartTime().date()==p_Activities.getToday() || _setCurrent )
		todayChanged(p_Activities.getToday());
}

void LastActs::todayChanged(const QDate& _date)
{
	m_Acts = p_Activities.getDay(_date);
	reset();
}
