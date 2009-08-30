/*
 * Activities.h
 *
 *  Created on: 22.08.2009
 *      Author: breeze
 */

#ifndef ACTIVITIES_H_
#define ACTIVITIES_H_

#include <map>
#include <QObject>
#include "DayActivities.h"
#include "Saver.h"

/// Хранилище хронометража одного дня. Весь день состоит из множества Activity
class Activities : public QObject
{
	Q_OBJECT

	typedef std::map<QDate, DayActivities> ActivitySet;

	ActivitySet	m_Activities;
	Saver::DateSet	m_Days;
	QDate		m_Today;
	Activity	m_CurActivity;
	bool		has_CurActivity;

public:
	Activities( const QDate& _date = QDate::currentDate() );

	void		addActivity(const Activity& _act, bool _setCurrent = true);

	const Activity& getCurrentActivity() const;
	bool		hasCurActivity() const;
	/// Обновляет активность, например для изменения количества прерываний
	void		updateActivity(const Activity& _act);

	size_t		countDays() const;
	const QDate&	getToday() const;
	DayActivities&	getTodayActs();
	/// Устанавливает текущий день, а самую последнюю задачу из указанного дня считает текущей
	/// Если _date.isNull(), то в качестве сегодняшнего дня выставляется сегодняшний день
	void		setToday( const QDate& _date = QDate() );
	DayActivities&	getDay(const QDate& _date);

	const Saver::DateSet&
			getDays() const;

	bool		hasChanged() const;

	void		save();

signals:
	void 		ActAdded(const Activity& _act, bool _setCurrent);
	void 		todayChanged(const QDate& _today);


protected:
};

#endif /* ACTIVITIES_H_ */
