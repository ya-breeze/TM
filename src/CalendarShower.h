/*
 * CalendarShower.h
 *
 *  Created on: 20.12.2009
 *      Author: breeze
 */

#ifndef CALENDARSHOWER_H_
#define CALENDARSHOWER_H_

#include <QObject>
#include <QEvent>

class CalendarShower : public QObject
{
	Q_OBJECT
public:
	CalendarShower( QObject *_parent );

signals:
	void clicked();

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif /* CALENDARSHOWER_H_ */
