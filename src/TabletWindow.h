/*
 * TabletWindow.h
 *
 *  Created on: 26.08.2009
 *      Author: breeze
 */

#ifndef TABLETWINDOW_H_
#define TABLETWINDOW_H_

#include <QtGui/QMainWindow>
#include "ui_tabletmain.h"

#include "TaskTree.h"
#include "HideDone.h"
#include "CategoryTree.h"
#include "Activities.h"
#include "LastActs.h"
#include "PlannedTaskList.h"

/// Класс главного окна для GUI-таблетки
class TabletWindow : public QMainWindow
{
	Q_OBJECT

	enum Directions
	{
		UP,
		DOWN,
		RIGHT,
		LEFT
	};

public:
	TabletWindow( QWidget *parent = 0 );

public slots:
	void slot_SetFocusTasks();
	void slot_SetFocusAddActivity();
	void on_btnPlanned_clicked();

	void slot_TaskDelete();
	void slot_TaskAddChild();
	void slot_TaskAddSibling();

	void slot_Restore();
	void slot_Save();
//	void slot_Sync();
	void slot_HideNotes();
	void slot_TaskChanged(const QModelIndex& _new, const QModelIndex& _old);
	void slot_TaskProperties();
	/// Вызывает диалог настройки фильтров
	void slot_Filter();
	/// Сбрасывает время начала активности в текущее время
	void slot_ActStartTime();
	/// Добавляет новую активность
	void slot_AddActivity();
	/// Выводит текущую активность
	void slot_CurrentActivity();
	/// Слот вызываемый при изменении типа добавляемой активности - чтобы делать enabled/disabled имя
	void on_rbActivityTask_toggled();
	/// Добавляет к текущей активности ещё одно прерывание
	void on_btnAddInterrupt_clicked();
	/// Вызывается при изменении текста в фильтре быстрой фильтрации
	void slot_FastFilter(const QString& _value);
	/// При изменении времени начала календаря
	void on_deStart_dateChanged( const QDate& _date );
	/// При изменении времени конца календаря
	void on_deEnd_dateChanged( const QDate& _date );
	/// При изменении модели календаря
	void on_PlannedTasks_modelReset();

	/// Сдвигает календарь на сегодняшний день
	void on_btnToday_clicked();
	/// Сдвигает календарь на неделю назад
	void on_btnWeekAgo_clicked();
	/// Сдвигает календарь на неделю вперед
	void on_btnWeekAfter_clicked();

	/// Очищает используемый быстрый фильтр
	void on_btnClearFastFilter_clicked();

	void slot_MoveUp();
	void slot_MoveDown();
	void slot_MoveLeft();
	void slot_MoveRight();

	/// Вызывается при работе с быстрым фильтром - это единственный способ поймать работу с быстрым фильтром
	void slot_ModelReset();

	/// Слот для выбора времени начала активности в специальном диалоге
	void slot_SelectActivityDT();
protected:
	void resizeEvent( QResizeEvent * event );
	void closeEvent(QCloseEvent *event);
	void updateTaskProperties( const Task& _task );

	/// Возвращает индекс задачи, соседней по заданному направлению
	bool getNeighbourIndex(const QModelIndex& _idx, Directions _dir, QModelIndex &_parent, int &_row);
	void moveTask(Directions _dir);

	Ui::TabletMain	ui;
	TaskTree		m_Tasks;
	CategoryTree	m_Cats;
	Activities		m_Activities;
	LastActs		*p_LastActs;
	PlannedTaskList	*p_PlannedTasks;

	HideDone		*p_ProxyHideDone;
};

#endif /* TABLETWINDOW_H_ */
