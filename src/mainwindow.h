#ifndef _MAINWINDOW_H
#define _MAINWINDOW_H

#include <QObject>
#include <QtGui/QMainWindow>
#include "ui_mainwindow.h"

#include <QShortcut>
#include <QCloseEvent>

#include "TaskTree.h"
#include "Activities.h"
#include "LastActs.h"
#include "HideDone.h"
#include "CategoryTree.h"
#include "PlannedTaskList.h"

#include "server.h"

class TM : public QMainWindow
{
	Q_OBJECT

public:
	TM( QWidget *parent = 0 );
	~TM();

	enum Directions
	{
		UP,
		DOWN,
		RIGHT,
		LEFT
	};

public slots:
	/// Вызывается при начале синхронизации, чтобы заблокировать работу GUI
	void slot_StartSynchronization();
	/// Вызывается после окончания синхронизации
	void slot_StopSynchronization();
	
	void slot_AddItem();
	void slot_AddSiblingItem();
	void slot_DelItem();
	void slot_FocusChanged(QWidget *old, QWidget *now);
	void slot_TaskChanged(const QModelIndex& _new, const QModelIndex& _old);
	void slot_Save();
	void slot_Restore();
	void slot_SetFocusTasks();
	void slot_SetFocusNotes();
	void slot_SetFocusAddActivity();
	void slot_SetFocusActivities();
	void slot_SetFFFocus();
	void slot_AddActivity();
	/// Слот вызываемый при изменении типа добавляемой активности - чтобы делать enabled/disabled имя
	void slot_ActivityType();
	/// Обновляет информацию о текущей активности
	void slot_CurrentActivity();
	/// Переходит от текущей активности к выполняемой задаче
	void slot_BtnToTasks();
	/// Обновляет время на текущее в окне добавления активности
	void slot_BtnUpdateTime();
	/// Изменяет добавляемую активность данными из прошлой активности
	void slot_SelectedLastAct(const QModelIndex &_current);
	/// Изменяет статус законченности задачи
	void slot_SetFinished();
	/// При изменении чекбокса на начало задачи
	void slot_SetStartTime();
	/// Добавляет к текущей активности ещё одно прерывание
	void slot_AddInterrupt();
	/// Прячет/показывает выполненные задачи
	void slot_HideDone();
	/// Вызывает диалог для редактирования отображаемых категорий
	void slot_SelectShowCats();
	/// Вызывается при изменении текста в фильтре быстрой фильтрации
	void slot_FastFilter(const QString& _value);
	/// Вызывается для отображения активностей одного дня
	void slot_DumpActivitiesForDate();

	void slot_MoveUp();
	void slot_MoveDown();
	void slot_MoveLeft();
	void slot_MoveRight();

//	void slot_Sync();

	/// Вызывает диалог для правки списка категорий
	void slot_TotalCategories();
	/// Вызывает диалог для правки привязанных категорий
	void slot_Categories();

private:
	void updateTaskProperties( const Task& _task );
	void closeEvent(QCloseEvent *event);
	/// Возвращает индекс задачи, соседней по заданному направлению
	bool getNeighbourIndex(const QModelIndex& _idx, Directions _dir, QModelIndex &_parent, int &_row);
	void moveTask(Directions _dir);
	// Возвращает строку из пробелов. Длина строки равна глубине вложенности задачи
	QString deep2Spaces(Task *task);
	// Возвращает полное имя задачи
	QString fullName(Task *task);

	Ui::TMClass	ui;
	TaskTree	m_Tasks;
	CategoryTree m_Cats;
	PlannedTaskList	*p_PlannedTasks;

	Activities	m_Activities;
	LastActs	*p_LastActs;

	QShortcut	*p_ShcAddSiblingTask;
	QShortcut	*p_ShcAddChildTask;
	QShortcut	*p_ShcDelTask;
	QShortcut	*p_ShcSetFinished;

	QShortcut	*p_ShcFocusTasks;
	QShortcut	*p_ShcFocusNotes;
	QShortcut	*p_ShcFocusActivities;
	QShortcut	*p_ShcFocusAddActivity;

	QShortcut	*p_ShcMoveUp;
	QShortcut	*p_ShcMoveDown;
	QShortcut	*p_ShcMoveLeft;
	QShortcut	*p_ShcMoveRight;

	HideDone	*p_ProxyHideDone;
	Saver		m_Saver;
	Server		*p_Server;
};

#endif // MAINWINDOW_H
