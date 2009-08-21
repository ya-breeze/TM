#include "mainwindow.h"

#include "exceptions.h"
#include <QMessageBox>

TM::TM(QWidget *parent)
    : QMainWindow(parent), p_CurFocus(NULL)
{
	ui.setupUi(this);

//	Task task("First"), task2("Second");
//	task2.setParent(task.getId());
//
//	m_Tasks.addChild(QModelIndex(), task);
//	m_Tasks.addChild(task.getId(), task2);
	ui.treeView->setModel(&m_Tasks);

	/// Slots
	connect( ui.actionAdd, SIGNAL(triggered(bool)), this, SLOT(slot_AddItem()) );
	connect( ui.actionAddChild, SIGNAL(triggered(bool)), this, SLOT(slot_AddSiblingItem()) );
	connect( ui.actionDel, SIGNAL(triggered(bool)), this, SLOT(slot_DelItem()) );
	connect( ui.treeView->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
			this, SLOT(slot_TaskChanged(const QModelIndex&, const QModelIndex&)) );

	/// Focuses
	connect( ui.actionFocusTasks, SIGNAL(triggered(bool)), ui.treeView, SLOT(setFocus()) );
	connect( ui.actionFocusNotes, SIGNAL(triggered(bool)), ui.Notes, SLOT(setFocus()) );

	/// Shortcuts
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
		DEBUG(ui.Notes->toPlainText());
		DEBUG(m_Tasks.getItem(idx)->getNotes());
	}
}

void TM::slot_TaskChanged(const QModelIndex& _new, const QModelIndex& _old)
{
	if( _old.isValid() )
		m_Tasks.getItem(_old)->setNotes(ui.Notes->toPlainText());
	if( _new.isValid() )
		ui.Notes->setText( m_Tasks.getItem(_new)->getNotes() );
}
