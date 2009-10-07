/*
 * CategoryEdit.cpp
 *
 *  Created on: 07.10.2009
 *      Author: breeze
 */

#include "CategoryEdit.h"

CategoryEdit::CategoryEdit( QWidget *parent )
	: QDialog(parent)
{
	ui.setupUi(this);
}

void CategoryEdit::edit(Task& _task)
{
	exec();
}
