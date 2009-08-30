
#include "mainwindow.h"

#include <QMessageBox>

#include "utils.h"
#include "Saver.h"

TM::TM(QWidget *parent)
    : QMainWindow(parent), p_LastActs(new LastActs(&m_Tasks, &m_Activities, this))
{
	ui.setupUi(this);

	// Задачи
	p_ProxyHideDone = new QSortFilterProxyModel(this);
	p_ProxyHideDone->setSourceModel(&m_Tasks);
	p_ProxyHideDone->setDynamicSortFilter(true);
	ui.treeView->setModel(p_ProxyHideDone);

	// Активности
	ui.lvLastActivities->setModel( p_LastActs );

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

	connect( ui.btnToTasks, SIGNAL(clicked()), this, SLOT(slot_BtnToTasks()) );
	connect( ui.btnUpdateTime, SIGNAL(clicked()), this, SLOT(slot_BtnUpdateTime()) );

	connect( ui.lvLastActivities->selectionModel(), SIGNAL(currentChanged ( const QModelIndex &, const QModelIndex &)),
			this, SLOT(slot_SelectedLastAct(const QModelIndex&)) );
	connect( ui.cbHideDone, SIGNAL(stateChanged(int)), &m_Tasks, SLOT(setHideDone(int)) );


	// Shortcuts
	p_ShcFocusTasks		= new QShortcut(QKeySequence("Ctrl+T"), this, SLOT(slot_SetFocusTasks()));
	p_ShcFocusNotes		= new QShortcut(QKeySequence("Ctrl+N"), this, SLOT(slot_SetFocusNotes()));
	p_ShcFocusAddActivity	= new QShortcut(QKeySequence("Ctrl+A"), this, SLOT(slot_SetFocusAddActivity()));
	p_ShcFocusActivities	= new QShortcut(QKeySequence("Ctrl+L"), this, SLOT(slot_SetFocusActivities()));


	p_ShcAddChildTask	= new QShortcut(QKeySequence("Ins"), this, SLOT(slot_AddItem()));
	p_ShcAddSiblingTask	= new QShortcut(QKeySequence("Shift+Ins"), this, SLOT(slot_AddSiblingItem()));
	p_ShcDelTask		= new QShortcut(QKeySequence("Del"), this, SLOT(slot_DelItem()));
	p_ShcSetFinished	= new QShortcut(QKeySequence("Space"), this, SLOT(slot_SetFinished()));

	ui.cbHideDone->setChecked(true);
	ui.teStartTime->setEnabled(false);

	slot_Restore();
	slot_BtnUpdateTime();
}

TM::~TM()
{
}

void TM::slot_SetFinished()
{
	if( ui.treeView->hasFocus() )
	{
		try
		{
			QModelIndex idx = ui.treeView->selectionModel()->currentIndex();
			TaskItem *item = m_Tasks.getItem(idx);
			if( item->getFinished().isNull() )
				item->setFinished(QDateTime::currentDateTime());
			else
				item->setFinished(QDateTime());

			m_Tasks.setDataChanged(idx);
			// Если эту задачу больше показывать не нужно, значит и родитель поменялся
			idx = m_Tasks.parent(idx);
			m_Tasks.setDataChanged(idx);

			updateTaskProperties(*item);

//				res += " <Done>";
//			else if( !item->getStarted().isNull() )
//				res += " <Working>";
		}
		catch(std::exception& ex)
		{
			QMessageBox::critical(this, tr("title"), ex.what());
		}
	}
}

void TM::slot_AddItem()
{
	if( ui.treeView->hasFocus() )
	{
		try
		{
			QModelIndex idx = ui.treeView->selectionModel()->currentIndex();
			QModelIndex srcidx = p_ProxyHideDone->mapToSource(idx);
			QModelIndex srcnewidx = m_Tasks.addChild(srcidx);
			QModelIndex newidx = p_ProxyHideDone->mapFromSource(srcnewidx);

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

/// Изменяет добавляемую активность данными из прошлой активности
void TM::slot_SelectedLastAct(const QModelIndex &_current)
{
	const Activity &act = p_LastActs->getAct(_current);
	if( act.getAssignedTask().isNull() )
	{
		ui.rbActivityOther->setChecked(true);
		ui.leActivityName->setEnabled(true);
	}
	else
	{
		ui.rbActivityTask->setChecked(true);
		ui.leActivityName->setEnabled(false);

		QModelIndex newidx = m_Tasks.getItemIndex(act.getAssignedTask());
		ui.treeView->selectionModel()->setCurrentIndex(newidx, QItemSelectionModel::ClearAndSelect);
	}
	ui.leActivityName->setText(act.getName());

	ui.btnNewActivity->setFocus();
}

void TM::slot_DelItem()
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

void TM::slot_FocusChanged(QWidget *_old, QWidget */*_now*/)
{
	if( _old==ui.Notes )
	{
		QModelIndex idx = ui.treeView->selectionModel()->currentIndex();
		m_Tasks.getItem(idx)->setNotes(ui.Notes->toPlainText());
	}
}

void TM::updateTaskProperties( const Task& _task )
{
	ui.Notes->setText( _task.getNotes() );
	ui.lblCreateTime->setText( _task.getCreated().toString("yyyy.MM.dd H:mm") );
	ui.cbStartedTime->setChecked( !_task.getStarted().isNull() );
	ui.lePlannedTime->setText(_task.getPlannedTime());
	ui.lblRealTimeBrutto->setForegroundRole(QPalette::NoRole);

	bool needHightLight = false;

	if( _task.getStarted().isNull() )
		ui.teStartTime->setDateTime( QDateTime::currentDateTime() );
	else
		ui.teStartTime->setDateTime(_task.getStarted());
	if( _task.getFinished().isNull() )
	{
		needHightLight = true;
		ui.lblFinishTime->setText(tr("<not finished yet>"));
		double hours = (double)_task.getStarted().secsTo(QDateTime::currentDateTime())/3600;
		ui.lblRealTimeBrutto->setText(tr("now is ") + QString::number(hours, 'f', 2) + tr(" hours"));
	}
	else
	{
		ui.lblFinishTime->setText(_task.getFinished().toString("yyyy.MM.dd H:mm") );
		double hours = (double)_task.getStarted().secsTo(_task.getFinished())/3600;

		if( _task.getStarted().isNull() )
		{
			needHightLight = true;
			hours = (double)_task.getCreated().secsTo(_task.getFinished())/3600;
		}
		ui.lblRealTimeBrutto->setText(QString::number(hours, 'f', 2) + tr(" hours"));
	}

	if( needHightLight )
		ui.lblRealTimeBrutto->setForegroundRole(QPalette::Highlight);
}

void TM::slot_TaskChanged(const QModelIndex& _new, const QModelIndex& _old)
{
	if( _old.isValid() )
	{
		QModelIndex idx = p_ProxyHideDone->mapToSource(_old);
		TaskItem *item = m_Tasks.getItem(idx);
		Q_ASSERT(item);

		QString s = ui.Notes->toPlainText();
		if( item->getNotes()!=s )
		{
			item->setNotes(s);
			m_Tasks.setChanged();
		}
		if( ui.cbStartedTime->isChecked() )
			item->setStarted(ui.teStartTime->dateTime());
		item->setPlannedTime(ui.lePlannedTime->text());
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
		Activity act( ui.teActivityStartTime->dateTime() );

		if( ui.rbActivityTask->isChecked() )
		{
			QModelIndex idx = ui.treeView->selectionModel()->currentIndex();
			TaskItem *item = m_Tasks.getItem(idx);
			if( !item )
				ERROR("No one task is specified");
			act.setAssignedTask(item->getId());
			act.setName( item->getName() );
		}
		else
		{
			act.setName(ui.leActivityName->text());
		}
		bool setCurrent = true;
		if( m_Activities.hasCurActivity() && m_Activities.getCurrentActivity().getStartTime()>act.getStartTime() )
			setCurrent = false;
		m_Activities.addActivity(act, setCurrent);

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
	if( !m_Activities.hasCurActivity() )
		return;

	Activity act = m_Activities.getCurrentActivity();
	ui.lblCurrentActivity->setText(act.getName());
	ui.lblActivityStarted->setText( act.getStartTime().toString("yyyy.MM.dd hh:mm") );
	if( act.getAssignedTask().isNull() )
		ui.btnToTasks->setEnabled(false);
	else
	{
		ui.btnToTasks->setEnabled(true);
		TaskItem *item = m_Tasks.getItem(act.getAssignedTask());
		if( !item )
			ERROR("Wrong task in activity");
		ui.lblCurrentActivity->setText(item->getName());
	}
	ui.lblInterrupts->setText( QString::number(act.getInterrupts()) );
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

void TM::slot_BtnToTasks()
{
	slot_SetFocusTasks();
	Activity act = m_Activities.getCurrentActivity();
	ui.treeView->selectionModel()->setCurrentIndex(m_Tasks.getItemIndex(act.getAssignedTask()), QItemSelectionModel::ClearAndSelect);
}

void TM::slot_BtnUpdateTime()
{
	ui.teActivityStartTime->setDateTime( QDateTime::currentDateTime() );
}

void TM::slot_SetStartTime()
{
	QModelIndex idx = ui.treeView->selectionModel()->currentIndex();
	if( !idx.isValid() )
		return;

	m_Tasks.setDataChanged(idx);
	if( ui.cbStartedTime->isChecked() )
	{
		ui.teStartTime->setEnabled(true);
	}
	else
	{
		ui.teStartTime->setEnabled(false);
	}
}

void TM::slot_AddInterrupt()
{
	if( !m_Activities.hasCurActivity() )
		return;

	Activity act = m_Activities.getCurrentActivity();
	act.addInterrupt();
	ui.lblInterrupts->setText( QString::number(act.getInterrupts()) );

	m_Activities.updateActivity(act);
}

void TM::slot_MoveUp()
{
	QModelIndex idx = ui.treeView->selectionModel()->currentIndex();
	if( !idx.isValid() )
		return;

	m_Tasks.moveUp(idx);
}

void TM::slot_MoveDown()
{
	QModelIndex idx = ui.treeView->selectionModel()->currentIndex();
	if( !idx.isValid() )
		return;

	m_Tasks.moveDown(idx);
}

void TM::slot_MoveLeft()
{
	QModelIndex idx = ui.treeView->selectionModel()->currentIndex();
	if( !idx.isValid() )
		return;

	m_Tasks.moveLeft(idx);
}

void TM::slot_MoveRight()
{
	QModelIndex idx = ui.treeView->selectionModel()->currentIndex();
	if( !idx.isValid() )
		return;

	m_Tasks.moveRight(idx);
}
