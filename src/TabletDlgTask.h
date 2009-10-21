/*
 * TabletDlgTask.h
 *
 *  Created on: 05.10.2009
 *      Author: breeze
 */

#ifndef TABLETDLGTASK_H_
#define TABLETDLGTASK_H_

#include <QDialog>

#include "ui_TabletDlgTask.h"
#include "TaskTree.h"
#include "CategoryTree.h"

class TabletDlgTask : public QDialog
{
	Q_OBJECT

public:
	TabletDlgTask( QWidget *parent = NULL );
	void edit(TaskItem *_item, CategoryTree *_cats);

protected slots:
	int exec();
	/// Диалог выбора категорий задачи
	void slot_Filter();

protected:
	Ui::TabletDlgTask	ui;
	TaskItem			*m_Task;
	CategoryTree		*m_Cats;
};

#endif /* TABLETDLGTASK_H_ */
