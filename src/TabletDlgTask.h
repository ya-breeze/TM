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
	TabletDlgTask( IconCache& _icons, QWidget *parent = NULL );
	/// Отображает диалог редактирования задачи. При accept возвращает true
	bool edit(TaskItem *_item, CategoryTree *_cats);

protected slots:
	int exec();
	/// Диалог выбора категорий задачи
	void slot_Filter();
	/// Вызывается при смене состояния признака окончания
	void slot_Finish(int);
	/// Выставляет время начала в текущее время
	void slot_UpdateStartTime();
	/// Слот для выбора времени начала в специальном диалоге
	void on_btnStartTime_clicked();

protected:
	Ui::TabletDlgTask	ui;
	TaskItem		*m_Task;
	CategoryTree		*m_Cats;
	IconCache		&m_Icons;
};

#endif /* TABLETDLGTASK_H_ */
