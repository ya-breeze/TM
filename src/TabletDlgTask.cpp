/*
 * TabletDlgTask.cpp
 *
 *  Created on: 05.10.2009
 *      Author: breeze
 */

#include "TabletDlgTask.h"

#include <QTreeView>
#include <QMessageBox>

#include "CategoryEdit.h"

TabletDlgTask::TabletDlgTask( QWidget *parent )
	: QDialog(parent)
{
	ui.setupUi(this);
}

void TabletDlgTask::edit(TaskItem *_item, CategoryTree *_cats)
{
	Q_ASSERT(_item && _cats);

	m_Task = _item;
	m_Cats = _cats;

	exec();
}

/// Диалог выбора категорий задачи
void TabletDlgTask::slot_Filter()
{
	try
	{
		CategoryEdit dlg(this, m_Cats);
		QStringList cats = dlg.edit(m_Task->getCategories());
		m_Task->setCategories(cats);
	}
	catch(std::exception& ex)
	{
		QMessageBox::critical(this, tr("Error"), ex.what());
	}
}

int TabletDlgTask::exec()
{
	return QDialog::exec();
}
