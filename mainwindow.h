#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_mainwindow.h"

#include "TaskTree.h"

class TM : public QMainWindow
{
	Q_OBJECT

public:
	TM( QWidget *parent = 0 );
	~TM();

public slots:
	void slot_AddItem();
	void slot_AddChildItem();
	void slot_DelItem();


private:
	Ui::TMClass	ui;
	TaskTree	m_Tasks;
};

#endif // MAINWINDOW_H
