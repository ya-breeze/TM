/*
 * LastActs.cpp
 *
 *  Created on: 22.08.2009
 *      Author: breeze
 */

#include "LastActs.h"

#include "utils.h"

LastActs::LastActs( TaskTree* _tasks, Activities *_acts, QObject *parent )
	: QAbstractTableModel(parent), p_Activities(_acts), p_Tasks(_tasks)
{
	connect(p_Activities, SIGNAL(ActAdded(const Activity&, bool)), this, SLOT(actAdded(const Activity&, bool)));
	connect(p_Activities, SIGNAL(todayChanged(const QDate&)), this, SLOT(todayChanged(const QDate&)));
}

int LastActs::rowCount( const QModelIndex &_parent ) const
{
	DayActivities &acts = p_Activities->getTodayActs();
//	DEBUG("Today activities - " << acts.count());
	if( !_parent.isValid() )
		return (acts.count()>5 ? 5 : acts.count());

	return 0;
}

int LastActs::columnCount( const QModelIndex &/*_parent*/ ) const
{
	return 2;
}

QVariant LastActs::data( const QModelIndex &_index, int _role ) const
{
	if( _role!=Qt::DisplayRole )
		return QVariant();

	DayActivities &acts = p_Activities->getTodayActs();
	size_t index = acts.count() - _index.row() - 1;
	const Activity &act = acts.getActivity(index);
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
				TaskItem *item = p_Tasks->getItem(act.getAssignedTask());
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
	if( _act.getStartTime().date()==p_Activities->getToday() || _setCurrent )
		reset();
}

void LastActs::todayChanged(const QDate&)
{
	reset();
}
