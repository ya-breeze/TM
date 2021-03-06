/*
 * ConvertorFromSeconds.h
 *
 *  Created on: 25.03.2011
 *      Author: ruilko
 */

#ifndef CONVERTORFROMSECONDS_H_
#define CONVERTORFROMSECONDS_H_

#include "Convertor.h"
#include <QDebug>

struct ConvertorFromSeconds : public Convertor {
    QString convert(int _value) {
        int hours = _value/3600;
        int minutes = (_value-hours*3600)/60;

        return QString("%1:%2").arg(hours).arg((int)minutes, (int)2, (int)10, QChar('0'));
    }
};

#endif /* CONVERTORFROMSECONDS_H_ */
