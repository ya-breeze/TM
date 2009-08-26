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

protected:
	Ui::TabletMain	ui;
	TaskTree	m_Tasks;
};

#endif /* TABLETWINDOW_H_ */
