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

class CategoryEdit : public QDialog
{
	Q_OBJECT

public:
	CategoryEdit( QWidget *parent = NULL );
	void edit(Task& _task);

protected:
	Ui::DlgCatEdit	ui;
};

#endif /* CATEGORYEDIT_H_ */
