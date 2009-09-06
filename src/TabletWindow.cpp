/*
 * TabletWindow.cpp
 *
 *  Created on: 26.08.2009
 *      Author: breeze
 */

#include "TabletWindow.h"

#include <QMessageBox>
#include <QProcess>

#include "Saver.h"

TabletWindow::TabletWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.treeView->setModel(&m_Tasks);

	slot_Restore();
}

void TabletWindow::slot_SetFocusTasks()
{
	ui.stackedWidget->setCurrentIndex(0);
}

void TabletWindow::slot_SetFocusChrono()
{
	ui.stackedWidget->setCurrentIndex(1);
}

void TabletWindow::slot_SetFocusAddActivity()
{
	ui.stackedWidget->setCurrentIndex(2);
}

void TabletWindow::slot_TaskDelete()
{
	int btn = QMessageBox::question(this, tr("Task delete"), tr("Do you really want to delete task?"), QMessageBox::Yes, QMessageBox::No );
	if( btn==QMessageBox::No )
		return;

	try
	{
		QModelIndex idx = ui.treeView->selectionModel()->currentIndex();
		m_Tasks.delItem(idx);
		ui.treeView->resizeColumnToContents(0);
	}
	catch(std::exception& ex)
	{
		QMessageBox::critical(this, tr("title"), ex.what());
	}
}

void TabletWindow::slot_TaskAddChild()
{
	try
	{
		QModelIndex idx = ui.treeView->selectionModel()->currentIndex();
		QModelIndex newidx = m_Tasks.addChild(idx);
		ui.treeView->expand(idx);
		ui.treeView->edit(newidx);
		ui.treeView->selectionModel()->setCurrentIndex(newidx, QItemSelectionModel::ClearAndSelect);
		ui.treeView->resizeColumnToContents(0);
	}
	catch(std::exception& ex)
	{
		QMessageBox::critical(this, tr("title"), ex.what());
	}
}

void TabletWindow::slot_TaskAddSibling()
{
	try
	{
		QModelIndex idx = ui.treeView->selectionModel()->currentIndex();
		QModelIndex newidx = m_Tasks.addSibling(idx);
		ui.treeView->edit(newidx);
		ui.treeView->selectionModel()->setCurrentIndex(newidx, QItemSelectionModel::ClearAndSelect);
		ui.treeView->resizeColumnToContents(0);
	}
	catch(std::exception& ex)
	{
		QMessageBox::critical(this, tr("title"), ex.what());
	}
}

void TabletWindow::slot_Restore()
{
	try
	{
		m_Tasks.clear();
		Saver saver;
		saver.restore(m_Tasks);
//		m_Activities.setToday();
		ui.treeView->reset();
		ui.treeView->expandAll();
		ui.treeView->resizeColumnToContents(0);

//		slot_CurrentActivity();
	}
	catch(std::exception& ex)
	{
		QMessageBox::critical(this, tr("Can't restore"), ex.what());
	}
}

void TabletWindow::slot_Save()
{
	try
	{
		QModelIndex idx = ui.treeView->selectionModel()->currentIndex();
//		slot_TaskChanged(idx, idx);

		Saver saver;
		saver.save(m_Tasks);
		m_Tasks.setChanged(false);
//		if( m_Activities.hasChanged() )
//			m_Activities.save();
	}
	catch(std::exception& ex)
	{
		QMessageBox::critical(this, tr("Can't save"), ex.what());
	}
}

void TabletWindow::slot_Sync()
{
	slot_Save();
	Saver svr;
	QProcess process(this);
	process.setWorkingDirectory(svr.getHome());
//	process.setStandardOutputFile("/tmp/tmsync.log");
//	process.setStandardErrorFile("/tmp/tmsync.errors");
	process.start("./sync", QStringList());
	if( !process.waitForFinished() )
	{
		QMessageBox::critical(this, tr("Sync failed"), tr("Sync failed"));
	}
	slot_Restore();
}
