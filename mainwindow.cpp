
#include "mainwindow.h"

#include <QMessageBox>

#include "utils.h"
#include "Saver.h"

TM::TM(QWidget *parent)
    : QMainWindow(parent)
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

	connect( ui.btnNewActivity, SIGNAL(clicked()), this, SLOT(slot_AddActivity()) );

	connect( ui.rbActivityTask, SIGNAL(toggled(bool)), this, SLOT(slot_ActivityType()) );
	/// Focuses
//	connect( ui.actionFocusTasks, SIGNAL(triggered(bool)), ui.treeView, SLOT(setFocus()) );
//	connect( ui.actionFocusNotes, SIGNAL(triggered(bool)), ui.Notes, SLOT(setFocus()) );

	/// Shortcuts
	p_ShcFocusTasks		= new QShortcut(QKeySequence("Ctrl+T"), this, SLOT(slot_SetFocusTasks()));
	p_ShcFocusNotes		= new QShortcut(QKeySequence("Ctrl+N"), this, SLOT(slot_SetFocusNotes()));
	p_ShcFocusAddActivity	= new QShortcut(QKeySequence("Ctrl+A"), this, SLOT(slot_SetFocusAddActivity()));
	p_ShcFocusActivities	= new QShortcut(QKeySequence("Ctrl+L"), this, SLOT(slot_SetFocusActivities()));


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
//		saver.save(m_Activities);
		m_Tasks.setChanged(false);
		if( m_Activities.hasChanged() )
			m_Activities.save();
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
//		saver.restore(m_Activities);
		m_Activities.setToday();
		ui.treeView->reset();
		ui.treeView->expandAll();
		ui.treeView->resizeColumnToContents(0);

		slot_CurrentActivity();
	}
	catch(std::exception& ex)
	{
		QMessageBox::critical(this, tr("Can't restore"), ex.what());
	}
}

void TM::slot_SetFocusTasks()
{
	ui.tabMain->setCurrentIndex(0);
	ui.treeView->setFocus();
}

void TM::slot_SetFocusActivities()
{
	ui.tabMain->setCurrentIndex(1);
//	ui.treeView->setFocus();
}

void TM::slot_SetFocusNotes()
{
	ui.toolBox->setCurrentIndex(0);
	ui.Notes->setFocus();
}

void TM::slot_SetFocusAddActivity()
{
	ui.toolBox->setCurrentIndex(1);
	ui.teActivityStartTime->setDateTime( QDateTime::currentDateTime() );
	ui.rbActivityTask->setChecked(true);
	slot_ActivityType();
	ui.rbActivityTask->setFocus();
}

void TM::closeEvent(QCloseEvent *event)
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

void TM::slot_AddActivity()
{
	try
	{
		Activity act( QDateTime(m_Activities.getToday(), ui.teActivityStartTime->dateTime().time()) );

		if( ui.rbActivityTask->isChecked() )
		{
			QModelIndex idx = ui.treeView->selectionModel()->currentIndex();
			TaskItem *item = m_Tasks.getItem(idx);
			if( !item )
				ERROR("No one task is specified");
			act.setAssignedTask( item->getId());
		}
		else
		{
			act.setName(ui.leActivityName->text());
		}
		m_Activities.addActivity(act);

		ui.tabMain->setCurrentIndex(1);
		slot_CurrentActivity();
	}
	catch(std::exception& _ex)
	{
		QMessageBox::critical(this, tr("Can't add activity"), _ex.what());
	}
}

void TM::slot_CurrentActivity()
{
	Activity act = m_Activities.getCurrentActivity();
	ui.lblCurrentActivity->setText(act.getName());
	ui.lblActivityStarted->setText( act.getStartTime().toString("yyyy.MM.dd hh:mm") );
	if( act.getAssignedTask().isNull() )
		ui.btnToTasks->setEnabled(false);
	else
	{
		TaskItem *item = m_Tasks.getItem(act.getAssignedTask());
		if( !item )
			ERROR("Wrong task in activity");
		ui.lblCurrentActivity->setText(item->getName());
	}
}

void TM::slot_ActivityType()
{
	if( ui.rbActivityTask->isChecked() )
	{
		ui.leActivityName->setEnabled(false);
	}
	else
	{
		ui.leActivityName->setEnabled(true);
	}
}
