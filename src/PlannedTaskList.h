/*
 * PlannedTaskList.h
 *
 *  Created on: 14.11.2009
 *      Author: breeze
 */

#include <map>
#include <QObject>
#include <QAbstractTableModel>
#include <QDate>

#include "TaskTree.h"

#ifndef PLANNEDTASKLIST_H_
#define PLANNEDTASKLIST_H_

/// Модель для задач запланированных на определённое время
class PlannedTaskList : public QAbstractTableModel
{
	Q_OBJECT;

	struct Point
	{
		int x, y;

		Point() : x(-1), y(-1) {};
		Point(int _x, int _y) : x(_x),  y(_y) {};

		bool isValid() const { return (x==-1 || y==-1) ? false : true; };
		bool operator==( const Point& _p ) const { return (x==_p.x && y==_p.y) ? true : false; };
		bool operator!=( const Point& _p ) const { return (x!=_p.x || y!=_p.y) ? true : false; };
		bool operator<( const Point& _p ) const { if(x!=_p.x) return (x<_p.x); return (y<_p.y); };
	};
	typedef std::map<Point, TaskItem*> MapFromGlobal;
public:
	PlannedTaskList( TaskTree *_tasks,  QObject *_parent = NULL );
	virtual ~PlannedTaskList();

	int rowCount( const QModelIndex& _parent ) const;
	int columnCount( const QModelIndex& _parent ) const;
	QVariant data( const QModelIndex& _index, int role ) const;
	QVariant headerData( int section, Qt::Orientation orientation, int role ) const;

	void setStartDate( const QDate& _date);
	QDate startDate() const;
	void setEndDate( const QDate& _date);
	QDate endDate() const;

public slots:
	void on_rowsInserted( const QModelIndex & parent, int start, int end );
	void on_rowsRemoved( const QModelIndex & parent, int start, int end );
	void on_dataChanged( const QModelIndex & topLeft, const QModelIndex & bottomRight );
	void on_modelReset();

protected:
	/// Проверяет и добавляет указанную задачу
	void	addTask( TaskItem *_task );
	/// Для проверки попадания задачи с указанным индексом в диапазон m_Start...m_End
	bool	checkDataRange( TaskItem *_task ) const;
	/// Определяет ячейку для указанного времени
	Point	getPoint( const QDateTime& _dt ) const;
	/// Инкрементит указанную точку на 1 час. Если выходит за границы - возвращает невалидный point
	Point	incPoint( const Point& _p ) const;
	/// Рекурсивно добавляет задачи, начиная с переданной
	void	recurseAdd( const QModelIndex& _idx );

	TaskTree *p_Tasks;
	QDate m_Start;
	QDate m_End;

	MapFromGlobal m_MapFromGlobal;
};

#endif /* PLANNEDTASKLIST_H_ */
