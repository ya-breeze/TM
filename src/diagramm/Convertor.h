/*
 * Convertor.h
 *
 *  Created on: 25.03.2011
 *      Author: ruilko
 */

#ifndef CONVERTOR_H_
#define CONVERTOR_H_

struct Convertor {
    virtual QString convert(int _value) = 0;
};

#endif /* CONVERTOR_H_ */
