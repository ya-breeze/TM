/*
 * utils.h
 *
 *  Created on: 28.02.2009
 *      Author: breeze
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <iostream>
#include <QString>
#include <QObject>
#include <stdexcept>
#include <sstream>
#include <QDateTime>


//#include <map>


inline std::ostream& operator<<(std::ostream& _os, const QString& _s)
{
	_os << _s.toUtf8().data();
	return _os;
}

#ifdef NDEBUG
#define TRACE
#define DEBUG(x)
#else
#define TRACE		{std::cout << __FILE__ << ":" << __LINE__ << std::endl;}
#define DEBUG(x)	{std::cout << __FILE__ << ":" << __LINE__ << " - " << x << std::endl;}
#endif

#define ERROR(TEXT) {std::stringstream s; s << TEXT; throw std::runtime_error(s.str());}


#define CHECK_YES(x) (x.length()>=2 && x[1]=='n')

/// Создаёт указанный каталог по имени файла, включая вышележащие каталоги. Возвращает true если удалось
bool createDirFromFile( const char* );
/// Создаёт указанный каталог по имени файла, включая вышележащие каталоги. Возвращает true если удалось
bool createDirFromFile( const QString& );

/// \brief Формирует строку для отображения времени начала задачи
///
/// Выводит дату _date относительно переданной даты _now в более простом для человека виде.
/// Например: "завтра", "послезавтра", "на неделе" и т.д.
QString prettyDate( const QDateTime& _now, const QDateTime& _date);

#endif /* UTILS_H_ */
