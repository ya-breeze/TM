/*
 * Category.cpp
 *
 *  Created on: 14.10.2009
 *      Author: breeze
 */

#include "Category.h"

Category::Category( const QString& _name )
{
	m_Values = _name.split(":");
}

/// Возвращает конечное имя категории - без родителей
const QString& Category::name() const
{
	return m_Values.last();
}

/// Возвращает полное имя категории - с родителями
QString Category::fullName() const
{
	return m_Values.join(":");
}

/// Возвращает глубину родителей. Если родителей нет - возвращает 0
int Category::deep() const
{
	return m_Values.size()-1;
}

/// Возвращает имя родителя указанной глубины
const QString& Category::parentName(int _idx) const
{
	Q_ASSERT( _idx<m_Values.size() );
	return m_Values[_idx];
}

/// Изменяет название категории
void Category::setName(const QString& _name)
{
	m_Values.last() = _name;
}

/// Возвращает строку содержащую указанную глубину категорий
QString Category::partialName(int _deep) const
{
	return QStringList(m_Values.mid(0, _deep)).join(":");
}

/// Возвращает строку, содержащую потомка текущей категории с указанным именем
QString Category::childName(const QString& _name) const
{
	return fullName() + ":" + _name;
}
