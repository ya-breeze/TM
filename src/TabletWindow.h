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

/// Класс главного окна для GUI-таблетки
class TabletWindow : public QMainWindow
{
	Q_OBJECT

public:
	TabletWindow( QWidget *parent = 0 );

public slots:
	void slot_SetFocusTasks();
	void slot_SetFocusChrono();
	void slot_SetFocusAddActivity();

	void slot_TaskDelete();
	void slot_TaskAddChild();
	void slot_TaskAddSibling();

	void slot_Restore();
	void slot_Save();
	void slot_Sync();
	void slot_HideNotes();
	void slot_TaskChanged(const QModelIndex& _new, const QModelIndex& _old);
	void slot_TaskProperties();
	/// Вызывает диалог настройки фильтров
	void slot_Filter();

protected:
	void closeEvent(QCloseEvent *event);
	void updateTaskProperties( const Task& _task );

	Ui::TabletMain	ui;
	TaskTree		m_Tasks;
	CategoryTree	m_Cats;
	Activities		m_Activities;

	HideDone		*p_ProxyHideDone;
};

#endif /* TABLETWINDOW_H_ */
