#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_mainwindow.h"

#include <QShortcut>
#include <QCloseEvent>

#include "TaskTree.h"
#include "Activities.h"

class TM : public QMainWindow
{
	Q_OBJECT

public:
	TM( QWidget *parent = 0 );
	~TM();

public slots:
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
	void slot_AddActivity();
	/// Слот вызываемый при изменении типа добавляемой активности - чтобы делать enabled/disabled имя
	void slot_ActivityType();
	/// Обновляет информацию о текущей активности
	void slot_CurrentActivity();
	/// Переходит от текущей активности к выполняемой задаче
	void slot_BtnToTasks();

private:
	void closeEvent(QCloseEvent *event);

	Ui::TMClass	ui;
	TaskTree	m_Tasks;
	Activities	m_Activities;

	QShortcut	*p_ShcAddSiblingTask;
	QShortcut	*p_ShcAddChildTask;
	QShortcut	*p_ShcDelTask;
	QShortcut	*p_ShcFocusTasks;
	QShortcut	*p_ShcFocusNotes;
	QShortcut	*p_ShcFocusActivities;
	QShortcut	*p_ShcFocusAddActivity;
};

#endif // MAINWINDOW_H
