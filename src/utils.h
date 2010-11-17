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
#include <QMap>
#include <sys/time.h>
#include <errno.h>


//#include <map>

#define DT_FORMAT	"yyyy.MM.dd hh:mm"


typedef QMap<QString, QString> QStringMap;

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


/// Класс для подсчёта времени выполнения участков кода. Результат в миллисекундах
class TimeItem {
    timeval m_Start;
    timeval m_End;

public:
    TimeItem() {
	start();
    }

    void start() {
	if( ::gettimeofday(&m_Start, NULL) )
	    throw std::runtime_error(std::string("Can't get localtime - ") + ::strerror(errno));
    }

    long end() {
	if( ::gettimeofday(&m_End, NULL) )
	    throw std::runtime_error(std::string("Can't get localtime - ") + ::strerror(errno));

//	DEBUG("Start " << m_Start.tv_sec << "." << m_Start.tv_usec);
//	DEBUG("End   " << m_End.tv_sec << "." << m_End.tv_usec);
	long result = (m_End.tv_sec - m_Start.tv_sec)*1000 + (m_End.tv_usec - m_Start.tv_usec)/1000;
	return result;
    }

    long update() {
	long result = end();
	m_Start = m_End;
	return result;
    }
};

#endif /* UTILS_H_ */
