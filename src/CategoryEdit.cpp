/*
 * CategoryEdit.cpp
 *
 *  Created on: 07.10.2009
 *      Author: breeze
 */

#include "CategoryEdit.h"

#include <QInputDialog>

CategoryEdit::CategoryEdit( QWidget *parent, CategoryTree *_cats )
	: QDialog(parent), p_Cats(_cats)
{
	ui.setupUi(this);
	ui.treeView->setModel(p_Cats);
}

void CategoryEdit::edit(Task& _task)
{
	exec();
}

void CategoryEdit::slot_AddChild()
{
	bool ok;
	QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
										 tr("User name:"), QLineEdit::Normal,
										 tr("Empty"), &ok);
	if (ok && !text.isEmpty())
	{
		QModelIndex idx = ui.treeView->selectionModel()->currentIndex();
		idx = p_Cats->addChild(idx, text);
		ui.treeView->selectionModel()->setCurrentIndex(idx, QItemSelectionModel::ClearAndSelect);
		ui.treeView->resizeColumnToContents(0);
	}
}

void CategoryEdit::slot_AddSibling()
{
	bool ok;
	QString text = QInputDialog::getText(this, tr("Input category name"),
										 tr("Category name:"), QLineEdit::Normal,
										 tr("Empty"), &ok);
	if (ok && !text.isEmpty())
	{
		QModelIndex idx = ui.treeView->selectionModel()->currentIndex();
		idx = idx.parent();
		idx = p_Cats->addChild(idx, text);
		ui.treeView->selectionModel()->setCurrentIndex(idx, QItemSelectionModel::ClearAndSelect);
		ui.treeView->resizeColumnToContents(0);
	}
}

void CategoryEdit::slot_Delete()
{
	// TODO Тут надо достаточно хитро работать
}
