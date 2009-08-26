
#ifndef __SMART_H_
#define __SMART_H_

#ifdef TABLET
#include <boost/shared_ptr.hpp>
using boost::shared_ptr;
#else
#include <tr1/memory>
using std::tr1::shared_ptr;
#endif

#endif
