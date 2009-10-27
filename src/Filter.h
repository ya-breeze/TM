/*
 * Filter.h
 *
 *  Created on: 27.10.2009
 *      Author: breeze
 */

#ifndef FILTER_H_
#define FILTER_H_

#include <QStringList>

/// \brief Класс для представления фильтра используемого при отображении задач
///
/// Фильтр характеризуется именем
class Filter
{
	bool		need_HideDone;
	QStringList	m_Categories;
	QString		str_Name;
public:
	Filter();

	bool operator<(const Filter& _o) const
	{
		return str_Name < _o.str_Name;
	}

    ///////////////////// Set/Get /////////////////////

	QStringList getCategories() const
    {
        return m_Categories;
    }

    bool getNeedHideDone() const
    {
        return need_HideDone;
    }

    void setCategories(const QStringList& m_Categories)
    {
        this->m_Categories = m_Categories;
    }

    void setNeedHideDone(bool need_HideDone)
    {
        this->need_HideDone = need_HideDone;
    }

    QString getName() const
    {
        return str_Name;
    }

    void setName(const QString& str_Name)
    {
        this->str_Name = str_Name;
    }
};

#endif /* FILTER_H_ */
