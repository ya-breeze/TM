/*
 * PlannedTaskList.cpp
 *
 *  Created on: 14.11.2009
 *      Author: breeze
 */

#include "PlannedTaskList.h"

#include "utils.h"

PlannedTaskList::PlannedTaskList( TaskTree *_tasks,  QObject *_parent )
	: QAbstractTableModel(_parent), p_Tasks(_tasks)
{
	// TODO Auto-generated constructor stub

	m_Start = QDate::currentDate();
	if( m_Start.dayOfWeek()!=1 )
		m_Start = m_Start.addDays(1 - m_Start.dayOfWeek());
	m_End = m_Start.addDays(9);

	connect(p_Tasks, SIGNAL(rowsInserted( const QModelIndex &, int , int )),
			this, SLOT(on_rowsInserted( const QModelIndex &, int , int )));
	connect(p_Tasks, SIGNAL(rowsRemoved( const QModelIndex &, int , int )),
			this, SLOT(on_rowsRemoved( const QModelIndex &, int , int )));
	connect(p_Tasks, SIGNAL(dataChanged( const QModelIndex &, const QModelIndex & )),
			this, SLOT(on_dataChanged( const QModelIndex &, const QModelIndex & )));
	connect(p_Tasks, SIGNAL(modelReset()),
			this, SLOT(on_modelReset()));

	// Получим данные
	on_modelReset();
}


PlannedTaskList::~PlannedTaskList()
{
	// TODO Auto-generated destructor stub
}

int PlannedTaskList::rowCount( const QModelIndex& _parent ) const
{
	return 24;
}

int PlannedTaskList::columnCount( const QModelIndex& ) const
{
	return m_Start.daysTo(m_End)+1;
}

QVariant PlannedTaskList::data( const QModelIndex& _idx, int role ) const
{
	if( _idx.row()<0 || _idx.row()>rowCount(QModelIndex()) )
		return QVariant();
	if( _idx.column()<0 || _idx.column()>columnCount(QModelIndex()) )
		return QVariant();

	// Заполнена ли эта ячейка?
	MapFromGlobal::const_iterator it = m_MapFromGlobal.find( Point(_idx.column(), _idx.row() ));
	if( it!=m_MapFromGlobal.end() )
	{
		// Текст
		if( role==Qt::DisplayRole )
		{
			return it->second->getName();
//			return p_Tasks->data(idx, Qt::DisplayRole);
		}

		// Заливка
		if( role==Qt::BackgroundRole )
		{
			return QBrush(Qt::yellow);
		}
	}

	// Раскрасим неиспользуемые ячейки
	if( role == Qt::BackgroundRole )
	{
		// Выделим сегодняшний день
		if( m_Start.addDays( _idx.column() ) == QDate::currentDate() )
		{
			return QBrush( QColor( "#c1ffc1" ) );
		}
		// Выделим выходные
		else
		{
			int dayOfWeek = m_Start.addDays( _idx.column() ).dayOfWeek();
			if( dayOfWeek == 6 || dayOfWeek == 7 )
			{
				return QBrush( QColor( "#ffdab9" ) );
			}
		}
	}


	return QVariant();
}

QVariant PlannedTaskList::headerData( int section, Qt::Orientation orientation, int role ) const
{
	if( role==Qt::DisplayRole )
	{
		if( orientation==Qt::Horizontal )
		{
			return m_Start.addDays(section).toString("d (ddd)");
		}
		else
		{
			return section;
		}
	}

	return QVariant();
}

/// Проверяет и добавляет указанную задачу
void PlannedTaskList::addTask( TaskItem *_task )
{
	//DEBUG("Checking task " << item->getName());
	if( checkDataRange(_task) )
	{
		//DEBUG("Task " << _task->getName() << " is ok");
		Point start = getPoint( _task->getStarted() );
		Point finish = getPoint( _task->getPlannedFinish() );
		//DEBUG(start.isValid() << ":" << finish.isValid() << " - " << (start==finish) );

		for( Point p = start; p != finish && p.isValid(); p = incPoint( p ) )
		{
			//DEBUG("curr - " << p.x << ":" << p.y);
			m_MapFromGlobal[p] = _task;
		}
	}
}

void PlannedTaskList::on_rowsInserted( const QModelIndex &_parent, int _start, int _end )
{
//	DEBUG(__PRETTY_FUNCTION__ << _start << ":" << _end);
	for( int i=_start; i<=_end; ++i )
	{
		QModelIndex idx = p_Tasks->index(i, 0, _parent);
		TaskItem *item = p_Tasks->getItem(idx);
		addTask(item);
	}
}

/// Инкрементит указанную точку на 1 час. Если выходит за границы - возвращает невалидный point
PlannedTaskList::Point PlannedTaskList::incPoint( const Point& _p ) const
{
	Point res(_p);
	++res.y;
	if( res.y>=rowCount(QModelIndex()))
	{
		++res.x;
		res.y = 0;
	}

	if( res.x>=columnCount(QModelIndex()))
		return Point();

	return res;
}

/// Определяет ячейку для указанного времени
PlannedTaskList::Point PlannedTaskList::getPoint( const QDateTime& _dt ) const
{
	// Не выходит ли за нижнюю границу?
	if( _dt.date()<m_Start )
		return Point(0, 0);

	// Не выходит ли за верхнюю границу?
	if( _dt.date()>m_End )
		return Point(columnCount(QModelIndex()), rowCount(QModelIndex()));

	// Точно знаем, что время в наших границах

	// Определим столбец - день
	int column = m_Start.daysTo( _dt.date() );

	// Определим строку - час
	int row = QTime(0, 0).secsTo( _dt.time() ) / 3600;

	return Point( column, row );
}

void PlannedTaskList::on_rowsRemoved( const QModelIndex& _parent, int _start, int _end )
{
	DEBUG(__PRETTY_FUNCTION__);
}

void PlannedTaskList::on_dataChanged( const QModelIndex& _topLeft, const QModelIndex& _bottomRight )
{
	DEBUG(__PRETTY_FUNCTION__);
	for( int i=_topLeft.row(); i<=_bottomRight.row(); ++i )
	{
		QModelIndex idx = p_Tasks->index(i, 0, _topLeft.parent());
		TaskItem *item = p_Tasks->getItem(idx);
		addTask(item);
	}
}

void PlannedTaskList::setStartDate( const QDate& _date)
{
	m_Start = _date;
	on_modelReset();
}

QDate PlannedTaskList::startDate() const
{
	return m_Start;
}

void PlannedTaskList::setEndDate( const QDate& _date)
{
	m_End = _date;
	on_modelReset();
}

QDate PlannedTaskList::endDate() const
{
	return m_End;
}

/// Для проверки попадания задачи с указанным индексом в диапазон m_Start...m_End
bool PlannedTaskList::checkDataRange( TaskItem *_task ) const
{
	if( _task->getStarted().date()<m_Start )
		return false;

	QDateTime finish = _task->getPlannedFinish();
	if( finish.date()>m_End )
		return false;

	return true;
}

void PlannedTaskList::on_modelReset()
{
	m_MapFromGlobal.clear();

	recurseAdd( QModelIndex() );

	reset();
}

void PlannedTaskList::recurseAdd( const QModelIndex& _idx )
{
	size_t sz = p_Tasks->rowCount(_idx);
	for(size_t i=0;i<sz;++i)
	{
		QModelIndex idx = p_Tasks->index(i, 0, _idx);
		TaskItem *item = p_Tasks->getItem(idx);
		Q_ASSERT(item);
		addTask(item);

		if( p_Tasks->columnCount(idx) )
			recurseAdd(idx);
	}
}
