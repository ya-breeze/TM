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

class TabletWindow : public QMainWindow
{
	Q_OBJECT

public:
	TabletWindow( QWidget *parent = 0 );

public slots:
	void slot_SetFocusTasks();
	void slot_SetFocusChrono();
	void slot_SetFocusAddActivity();

protected:
	Ui::TabletMain	ui;
};

#endif /* TABLETWINDOW_H_ */
