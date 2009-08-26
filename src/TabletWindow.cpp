/*
 * TabletWindow.cpp
 *
 *  Created on: 26.08.2009
 *      Author: breeze
 */

#include "TabletWindow.h"

TabletWindow::TabletWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

void TabletWindow::slot_SetFocusTasks()
{
	ui.stackedWidget->setCurrentIndex(0);
}

void TabletWindow::slot_SetFocusChrono()
{
	ui.stackedWidget->setCurrentIndex(1);
}

void TabletWindow::slot_SetFocusAddActivity()
{
	ui.stackedWidget->setCurrentIndex(2);
}
