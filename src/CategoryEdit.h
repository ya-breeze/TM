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
	void edit(Task& _task);

protected slots:
	void slot_AddChild();
	void slot_AddSibling();
	void slot_Delete();

protected:
	Ui::DlgCatEdit	ui;
	CategoryTree	*p_Cats;
};

#endif /* CATEGORYEDIT_H_ */
