/*
 * exceptions.h
 *
 *  Created on: 21.08.2009
 *      Author: breeze
 */

#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

#include <iostream>
#include <stdexcept>
#include <sstream>
#include <QString>

inline std::ostream& operator<<(std::ostream& _os, const QString& _s)
{
	_os << _s.toUtf8().data();
	return _os;
}

#define TRACE {std::cout << __FILE__ << ":" << __LINE__ << std::endl;};
#define DEBUG(TEXT) {std::cout << __FILE__ << ":" << __LINE__ << " " << TEXT << std::endl;};
#define ERROR(TEXT) {std::stringstream s; s << TEXT; throw std::runtime_error(s.str());}



#endif /* EXCEPTIONS_H_ */
