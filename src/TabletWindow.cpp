/*
 * TabletWindow.cpp
 *
 *  Created on: 26.08.2009
 *      Author: breeze
 */

#include "TabletWindow.h"

#include <QMessageBox>
#include <QProcess>
#include <QCloseEvent>

#include "Saver.h"

#include "TabletDlgTask.h"
#include "CategoryEdit.h"
#include "utils.h"

TabletWindow::TabletWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	// Задачи
	p_ProxyHideDone = new HideDone(this);
	p_ProxyHideDone->setSourceModel(&m_Tasks);
	p_ProxyHideDone->setDynamicSortFilter(true);
	ui.treeView->setModel(p_ProxyHideDone);

	connect( ui.treeView->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
			this, SLOT(slot_TaskChanged(const QModelIndex&, const QModelIndex&)) );

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
		idx = p_ProxyHideDone->mapToSource(idx);
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
		idx = p_ProxyHideDone->mapToSource(idx);
		QModelIndex newidx = m_Tasks.addChild(idx);
		newidx = p_ProxyHideDone->mapFromSource(newidx);
		ui.treeView->expand(idx);
		ui.treeView->scrollTo(newidx, QAbstractItemView::PositionAtCenter);
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
		idx = p_ProxyHideDone->mapToSource(idx);
		QModelIndex newidx = m_Tasks.addSibling(idx);
		newidx = p_ProxyHideDone->mapFromSource(newidx);
		ui.treeView->scrollTo(newidx, QAbstractItemView::PositionAtCenter);
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
		saver.restore(m_Tasks, m_Cats);
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

void TabletWindow::slot_HideNotes()
{
	ui.Notes->setVisible( !ui.Notes->isVisible() );
}

void TabletWindow::slot_TaskChanged(const QModelIndex& _new, const QModelIndex& _old)
{
	if( _old.isValid() )
	{
		QModelIndex idx = p_ProxyHideDone->mapToSource(_old);
		TaskItem *item = m_Tasks.getItem(idx);
		Q_ASSERT(item);

		// Заметки
		QString s = ui.Notes->toPlainText();
		if( item->getNotes()!=s )
		{
			item->setNotes(s);
			m_Tasks.setChanged();
		}

//		// Время начала
//		QDateTime startedCur;
//		if( ui.cbStartedTime->isChecked() )
//			startedCur = ui.teStartTime->dateTime();
//		if( startedCur!=item->getStarted() )
//		{
//			item->setStarted(startedCur);
//			m_Tasks.setChanged();
//		}
//
//		// Планируемое время
//		if( ui.lePlannedTime->text()!=item->getPlannedTime() )
//		{
//			item->setPlannedTime(ui.lePlannedTime->text());
//			m_Tasks.setChanged();
//		}
	}
	if( _new.isValid() )
	{
		QModelIndex idx = p_ProxyHideDone->mapToSource(_new);
		TaskItem *item = m_Tasks.getItem(idx);
		Q_ASSERT(item);

		updateTaskProperties(*item);
	}
	else
	{
		ui.Notes->setText("");
	}
}

void TabletWindow::updateTaskProperties( const Task& _task )
{
	ui.Notes->setText( _task.getNotes() );
//	ui.lblCreateTime->setText( _task.getCreated().toString("yyyy.MM.dd H:mm") );
//	ui.cbStartedTime->setChecked( !_task.getStarted().isNull() );
//	ui.lePlannedTime->setText(_task.getPlannedTime());
//	ui.lblRealTimeBrutto->setForegroundRole(QPalette::NoRole);
//
//	bool needHightLight = false;
//
//	if( _task.getStarted().isNull() )
//		ui.teStartTime->setDateTime( QDateTime::currentDateTime() );
//	else
//		ui.teStartTime->setDateTime(_task.getStarted());
//	if( _task.getFinished().isNull() )
//	{
//		needHightLight = true;
//		ui.lblFinishTime->setText(tr("<not finished yet>"));
//		double hours = (double)_task.getStarted().secsTo(QDateTime::currentDateTime())/3600;
//		ui.lblRealTimeBrutto->setText(tr("now is ") + QString::number(hours, 'f', 2) + tr(" hours"));
//	}
//	else
//	{
//		ui.lblFinishTime->setText(_task.getFinished().toString("yyyy.MM.dd H:mm") );
//		double hours = (double)_task.getStarted().secsTo(_task.getFinished())/3600;
//
//		if( _task.getStarted().isNull() )
//		{
//			needHightLight = true;
//			hours = (double)_task.getCreated().secsTo(_task.getFinished())/3600;
//		}
//		ui.lblRealTimeBrutto->setText(QString::number(hours, 'f', 2) + tr(" hours"));
//	}
//
//	if( needHightLight )
//		ui.lblRealTimeBrutto->setForegroundRole(QPalette::Highlight);
}

void TabletWindow::slot_TaskProperties()
{
	TRACE;
	try
	{
		QModelIndex proxyidx = ui.treeView->selectionModel()->currentIndex();
		QModelIndex idx = p_ProxyHideDone->mapToSource(proxyidx);

		TaskItem *item = m_Tasks.getItem(idx);
		if( !item )
			ERROR("No one task is specified");

		TabletDlgTask dlg(this);
		dlg.edit( item, &m_Cats );
	}
	catch(std::exception& ex)
	{
		QMessageBox::critical(this, tr("Error"), ex.what());
	}
}

/// Вызывает диалог настройки фильтров
void TabletWindow::slot_Filter()
{
	try
	{
		CategoryEdit dlg(this, &m_Cats);

		QStringList cats = dlg.edit( p_ProxyHideDone->getCategories() );
		p_ProxyHideDone->setCategories(cats);
		ui.treeView->expandAll();
		ui.treeView->resizeColumnToContents(0);

		DEBUG("Filter cats has " << cats.size());
	}
	catch(std::exception& ex)
	{
		QMessageBox::critical(this, tr("Error"), ex.what());
	}
}

void TabletWindow::closeEvent(QCloseEvent *event)
{
	QModelIndex idx = ui.treeView->selectionModel()->currentIndex();
	slot_TaskChanged(idx, idx);

	if( m_Tasks.hasChanged() || m_Activities.hasChanged() )
	{
		int btn = QMessageBox::question(this, tr("Unsaved data"), tr("Save them?"), QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel );
		if( btn==QMessageBox::Cancel )
		{
			event->ignore();
			return;
		}

		event->accept();
		if( btn==QMessageBox::Yes )
			slot_Save();
	}
	else
		QMainWindow::closeEvent(event);
}
