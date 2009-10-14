/*
 * Category.h
 *
 *  Created on: 14.10.2009
 *      Author: breeze
 */

#ifndef CATEGORY_H_
#define CATEGORY_H_

#include <QString>
#include <QStringList>

/** \brief Класс представляющий собой одну категорию
 *
 * Под категорией понимается строка, разделённая двоеточиями. Каждое двоеточие
 * отделяет уровень родителя
 */
class Category
{
public:
	Category( const QString& _name = "empty" );

	/// Возвращает конечное имя категории - без родителей
	const QString&	name() const;
	/// Возвращает полное имя категории - с родителями
	QString			fullName() const;
	/// Возвращает глубину родителей. Если родителей нет - возвращает 0
	int				deep() const;
	/// Возвращает имя родителя указанной глубины
	const QString&	parentName(int _idx) const;
	/// Изменяет название категории
	void			setName(const QString& _name);
	/// Возвращает строку содержащую указанную глубину категорий
	QString			partialName(int _deep) const;
	/// Возвращает строку, содержащую потомка текущей категории с указанным именем
	QString			childName(const QString& _name) const;

protected:
	QStringList	m_Values;
};

#endif /* CATEGORY_H_ */
