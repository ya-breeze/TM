/*
 * HideDone.cpp
 *
 *  Created on: 15.10.2009
 *      Author: breeze
 */

#include "HideDone.h"

#include "utils.h"

HideDone::HideDone( QObject *parent ) :
	QSortFilterProxyModel( parent ), need_HideDone(true)
{
}

bool HideDone::getHideDone() const
{
	return need_HideDone;
}

void HideDone::setHideDone( bool _hideDone )
{
	this->need_HideDone = _hideDone;
	invalidateFilter();
}

const QStringList& HideDone::getCategories() const
{
	return m_Categories;
}

void HideDone::setCategories(const QStringList& m_Categories)
{
	this->m_Categories = m_Categories;
	invalidateFilter();
}

bool HideDone::filterAcceptsRow( int sourceRow, const QModelIndex &sourceParent ) const
{
//    TRACE;
	TaskTree *tree = dynamic_cast<TaskTree*>(sourceModel());
	Q_ASSERT(tree);

	QModelIndex index = tree->index( sourceRow, 0, sourceParent );
	TaskItem *task = tree->getItem(index);
	Q_ASSERT(task);

	// Что с признаком выполненности?
	// Тут мы явно проверяем этот признак, хотя он и проверяется в filterTask. Дело в том,
	// что если родитель завершён, то дальше проверять вообще не нужно, а иначе если
	// хотя бы один потомок не завершён, то родитель будет показан, хотя это не требуется
	if( need_HideDone )
	{
		if( !task->getFinished().isNull() )
			return false;
	}

	return filterTaskRecursed(task);
}

/// Возвращает true, если переданая задача (без потомков) соответствует заданным условиям
HideDone::MatchFilter HideDone::filterTask( TaskItem *_item ) const
{
//    TRACE;
	Q_ASSERT(_item);

	// Что с признаком выполненности?
	if( need_HideDone )
	{
		// Если тут стоит признак завершённости, то дальше потомков смотреть не нужно, а
		// следовательно, нужно наверх сказать, об этом
		if( !_item->getFinished().isNull() )
			return MF_DOESNT_MATCH;
	}

	// Категории
	if( !m_Categories.empty() )
	{
		// Содержит ли текущая задача? Если не содержит, то может содержать потомок
		if( !_item->containsCategory(m_Categories) && !checkStartDate(_item) )
			return MF_UNKNOWN;
	}

	// Быстрый фильтр. Если не совпадает, то может совпадать потомок
	if( !str_FastFilter.isEmpty() && !_item->getName().contains(str_FastFilter, Qt::CaseInsensitive) )
		return MF_UNKNOWN;

	return MF_MATCH;
}

/// Возвращает true, если переданная задача или её потомки соответствует заданным условиям
bool HideDone::filterTaskRecursed( TaskItem *_item ) const
{
	Q_ASSERT(_item);
//	DEBUG(__LINE__ << " " << _item->getName());

	// Содержит ли текущая задача?
	if( filterTask(_item)==MF_MATCH )
	{
//		DEBUG(_item->getName() << " this true");
		return true;
	}
	else if( filterTask(_item)==MF_DOESNT_MATCH )
	{
//		DEBUG(_item->getName() << " this doesnt match");
		return false;
	}


	// Содержат ли потомки?
	for(int i=0; i<_item->childCount(); ++i)
	{
		TaskItem *item = _item->child(i);
		if( filterTaskRecursed(item) )
		{
//			DEBUG(item->getName() << " child  true");
			return true;
		}
	}

	return false;
}

/// Изменяет используемый быстрый фильтр
void HideDone::setFastFilter( const QString& _value )
{
	str_FastFilter = _value;
	invalidate();
}

/// Проверяет категории на основе даты начала задачи - TM-REQ-047
bool HideDone::checkStartDate( TaskItem *_item ) const
{
	if( !_item->getStarted().isValid() )
		return false;

	QDateTime now = QDateTime::currentDateTime();

	// Время:День
	if( now.date()>=_item->getStarted().date() )
	{
		if( m_Categories.contains(QString::fromUtf8("Время:День"), Qt::CaseInsensitive) )
				return true;
	}

	// Время:Неделя
	if( now.date().year()==_item->getStarted().date().year() && now.date().weekNumber()==_item->getStarted().date().weekNumber() )
	{
		if( m_Categories.contains(QString::fromUtf8("Время:Неделя"), Qt::CaseInsensitive) )
				return true;
	}

	return false;
}
