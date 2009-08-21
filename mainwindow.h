#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_mainwindow.h"

#include "TaskTree.h"
#include <QShortcut>

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


private:
	Ui::TMClass	ui;
	TaskTree	m_Tasks;
	QShortcut	*p_ShcAddSiblingTask;
	QShortcut	*p_ShcAddChildTask;
	QShortcut	*p_ShcDelTask;
};

#endif // MAINWINDOW_H
