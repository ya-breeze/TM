/*
 * CategoryEdit.cpp
 *
 *  Created on: 07.10.2009
 *      Author: breeze
 */

#include "CategoryEdit.h"

#include <QInputDialog>

#include "utils.h"

CategoryEdit::CategoryEdit( QWidget *parent, CategoryTree *_cats )
	: QDialog(parent), p_Cats(_cats)
{
	ui.setupUi(this);
	ui.treeView->setModel(p_Cats);
	ui.treeView->expandAll();
}

void CategoryEdit::edit(Task& _task)
{
	DEBUG(_task.getName());
	p_Cats->setCheckable(true);
	p_Cats->clearCheckboxes();

	catsFromTask(_task);
	if( exec()==QDialog::Rejected )
		return;

	catsToTask(_task);
}

void CategoryEdit::slot_AddChild()
{
	bool ok;
	QString text = QInputDialog::getText(this, tr("Input category name"),
										 tr("Category name:"), QLineEdit::Normal,
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

/// Редактировать список категории
void CategoryEdit::edit()
{
	p_Cats->clearCheckboxes();
	p_Cats->setCheckable(false);
	exec();
}

/// Отображает список категорий задачи
void CategoryEdit::catsFromTask(Task& _task)
{
	QStringList lst = _task.getCategories();
	for(int i=0;i<lst.size();++i)
		p_Cats->setChecked(lst[i], Qt::Checked);
}
/// Обновляет список категорий задачи
void CategoryEdit::catsToTask(Task& _task)
{
	QStringList lst = p_Cats->checkedList();
	_task.setCategories(lst);
}
