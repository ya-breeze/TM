
#include "mainwindow.h"

#include <QMessageBox>

#include "exceptions.h"
#include "Saver.h"

TM::TM(QWidget *parent)
    : QMainWindow(parent), p_CurFocus(NULL)
{
	ui.setupUi(this);
	ui.treeView->setModel(&m_Tasks);

	/// Slots
	connect( ui.actionAdd, SIGNAL(triggered(bool)), this, SLOT(slot_AddItem()) );
	connect( ui.actionAddChild, SIGNAL(triggered(bool)), this, SLOT(slot_AddSiblingItem()) );
	connect( ui.actionDel, SIGNAL(triggered(bool)), this, SLOT(slot_DelItem()) );
	connect( ui.treeView->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
			this, SLOT(slot_TaskChanged(const QModelIndex&, const QModelIndex&)) );
	connect( ui.actionSave, SIGNAL(triggered(bool)), this, SLOT(slot_Save()) );
	connect( ui.actionRestore, SIGNAL(triggered(bool)), this, SLOT(slot_Restore()) );

	/// Focuses
//	connect( ui.actionFocusTasks, SIGNAL(triggered(bool)), ui.treeView, SLOT(setFocus()) );
//	connect( ui.actionFocusNotes, SIGNAL(triggered(bool)), ui.Notes, SLOT(setFocus()) );

	/// Shortcuts
	p_ShcFocusTasks		= new QShortcut(QKeySequence("Ctrl+T"), ui.treeView, SLOT(setFocus()));
	p_ShcFocusNotes		= new QShortcut(QKeySequence("Ctrl+N"), this, SLOT(slot_SetFocusNotes()));
	p_ShcFocusAddActivity	= new QShortcut(QKeySequence("Ctrl+A"), this, SLOT(slot_SetFocusAddActivity()));


	p_ShcAddChildTask	= new QShortcut(QKeySequence("Ins"), this, SLOT(slot_AddItem()));
	p_ShcAddSiblingTask	= new QShortcut(QKeySequence("Shift+Ins"), this, SLOT(slot_AddSiblingItem()));
	p_ShcDelTask		= new QShortcut(QKeySequence("Del"), this, SLOT(slot_DelItem()));
}

TM::~TM()
{
}

void TM::slot_AddItem()
{
	if( ui.treeView->hasFocus() )
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
}

void TM::slot_AddSiblingItem()
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

void TM::slot_DelItem()
{
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

void TM::slot_FocusChanged(QWidget *_old, QWidget */*_now*/)
{
	if( _old==ui.Notes )
	{
		QModelIndex idx = ui.treeView->selectionModel()->currentIndex();
		m_Tasks.getItem(idx)->setNotes(ui.Notes->toPlainText());
	}
}

void TM::slot_TaskChanged(const QModelIndex& _new, const QModelIndex& _old)
{
	if( _old.isValid() )
	{
		TaskItem *item = m_Tasks.getItem(_old);
		QString s = ui.Notes->toPlainText();
		if( item->getNotes()!=s )
		{
			item->setNotes(s);
			m_Tasks.setChanged();
		}
	}
	if( _new.isValid() )
		ui.Notes->setText( m_Tasks.getItem(_new)->getNotes() );
	else
		ui.Notes->setText("");
}

void TM::slot_Save()
{
	try
	{
		QModelIndex idx = ui.treeView->selectionModel()->currentIndex();
		slot_TaskChanged(idx, idx);

		Saver saver;
		saver.save(m_Tasks);
	}
	catch(std::exception& ex)
	{
		QMessageBox::critical(this, tr("Can't save"), ex.what());
	}
}

void TM::slot_Restore()
{
	try
	{
		m_Tasks.clear();
		Saver saver;
		saver.restore(m_Tasks);
		ui.treeView->reset();
		ui.treeView->expandAll();
		ui.treeView->resizeColumnToContents(0);
	}
	catch(std::exception& ex)
	{
		QMessageBox::critical(this, tr("Can't restore"), ex.what());
	}
}

void TM::slot_SetFocusNotes()
{
	ui.toolBox->setCurrentIndex(0);
	ui.Notes->setFocus();
}

void TM::slot_SetFocusAddActivity()
{
	ui.toolBox->setCurrentIndex(1);
	ui.rbActivityTask->setFocus();
}

void TM::closeEvent(QCloseEvent *event)
{
	QModelIndex idx = ui.treeView->selectionModel()->currentIndex();
	slot_TaskChanged(idx, idx);

	if( m_Tasks.hasChanged() )
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
