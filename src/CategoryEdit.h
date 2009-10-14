/*
 * CategoryEdit.h
 *
 *  Created on: 07.10.2009
 *      Author: breeze
 */

#ifndef CATEGORYEDIT_H_
#define CATEGORYEDIT_H_

#include <QDialog>

#include "ui_CatEdit.h"

#include "TaskTree.h"
#include "CategoryTree.h"

class CategoryEdit : public QDialog
{
	Q_OBJECT

public:
	CategoryEdit( QWidget *parent, CategoryTree *_cats);


	/// Редактировать категории переданной задачи
	void edit(Task& _task);

	/// Редактировать список категории
	void edit();

protected slots:
	void slot_AddChild();
	void slot_AddSibling();
	void slot_Delete();

protected:
	/// Отображает список категорий задачи
	void catsFromTask(Task& _task);
	/// Обновляет список категорий задачи
	void catsToTask(Task& _task);

	Ui::DlgCatEdit	ui;
	CategoryTree	*p_Cats;
};

#endif /* CATEGORYEDIT_H_ */
