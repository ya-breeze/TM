/*
 * LastActs.h
 *
 *  Created on: 22.08.2009
 *      Author: breeze
 */

#ifndef LASTACTS_H_
#define LASTACTS_H_

#include <QAbstractTableModel>

#include "Activities.h"
#include "TaskTree.h"

class LastActs : public QAbstractTableModel
{
	Q_OBJECT

	Activities		&p_Activities;
	const TaskTree	&p_Tasks;
	DayActivities	m_Acts;
public:
	LastActs( const TaskTree& _tasks, Activities &_acts, QObject *parent = NULL );
	int rowCount ( const QModelIndex &_parent ) const;
	int columnCount ( const QModelIndex &_parent ) const;
	QVariant data( const QModelIndex &_index, int _role ) const;
	const Activity& getAct( const QModelIndex &_index ) const;

public slots:
	void 	todayChanged(const QDate& _today);

protected slots:
	void 	actAdded(const Activity& _act, bool _setCurrent);
};

#endif /* LASTACTS_H_ */
