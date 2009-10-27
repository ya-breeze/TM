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
bool HideDone::filterTask( TaskItem *_item ) const
{
	Q_ASSERT(_item);

	// Что с признаком выполненности?
	if( need_HideDone )
	{
		if( !_item->getFinished().isNull() )
			return false;
	}

	// Категории
	if( !m_Categories.empty() )
	{
		// Содержит ли текущая задача?
		if( !_item->containsCategory(m_Categories) )
			return false;
	}

	// Быстрый фильтр
	if( !str_FastFilter.isEmpty() && !_item->getName().contains(str_FastFilter, Qt::CaseInsensitive) )
		return false;

	return true;
}

/// Возвращает true, если переданная задача или её потомки соответствует заданным условиям
bool HideDone::filterTaskRecursed( TaskItem *_item ) const
{
	Q_ASSERT(_item);

	// Содержит ли текущая задача?
	if( filterTask(_item) )
		return true;

	// Содержат ли потомки?
	for(int i=0; i<_item->childCount(); ++i)
	{
		TaskItem *item = _item->child(i);
		if( filterTaskRecursed(item) )
			return true;
	}

	return false;
}

/// Изменяет используемый быстрый фильтр
void HideDone::setFastFilter( const QString& _value )
{
	str_FastFilter = _value;
	invalidate();
}
