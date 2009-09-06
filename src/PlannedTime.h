/*
 * PlannedTime.h
 *
 *  Created on: 04.09.2009
 *      Author: breeze
 */

#ifndef PLANNEDTIME_H_
#define PLANNEDTIME_H_

#include <QString>
#include <QDateTime>

/// Класс для работы с планируемым временем
///
/// Это время вводит пользователь - нужно уметь его парсить и приводить к нормальному QDateTime
class PlannedTime
{
	QString		str_Value;
	int		m_Days;
	int		m_Hours;
public:
	PlannedTime( const QString& _time );

	bool isNull() const;

protected:
	void parse( const QString& _time );
};

#endif /* PLANNEDTIME_H_ */
