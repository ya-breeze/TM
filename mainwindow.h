#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_mainwindow.h"

#include "TaskTree.h"
#include <QShortcut>
#include <QCloseEvent>

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


private:
	void closeEvent(QCloseEvent *event);

	Ui::TMClass	ui;
	TaskTree	m_Tasks;
	QShortcut	*p_ShcAddSiblingTask;
	QShortcut	*p_ShcAddChildTask;
	QShortcut	*p_ShcDelTask;
	QShortcut	*p_ShcFocusTasks;
	QShortcut	*p_ShcFocusNotes;
	QShortcut	*p_ShcFocusAddActivity;

	QWidget		*p_CurFocus;
};

#endif // MAINWINDOW_H
