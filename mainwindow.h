#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_mainwindow.h"

#include <QShortcut>
#include <QCloseEvent>

#include "TaskTree.h"
#include "DayActivities.h"

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
	void slot_SetFocusNotes();
	void slot_SetFocusAddActivity();
	void slot_AddActivity();


private:
	void closeEvent(QCloseEvent *event);

	Ui::TMClass	ui;
	TaskTree	m_Tasks;
	DayActivities	m_DayActivities;

	QShortcut	*p_ShcAddSiblingTask;
	QShortcut	*p_ShcAddChildTask;
	QShortcut	*p_ShcDelTask;
	QShortcut	*p_ShcFocusTasks;
	QShortcut	*p_ShcFocusNotes;
	QShortcut	*p_ShcFocusAddActivity;
};

#endif // MAINWINDOW_H
