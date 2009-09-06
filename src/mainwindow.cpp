
#include "mainwindow.h"

#include <QMessageBox>
#include <QProcess>

#include "utils.h"
#include "Saver.h"

TM::TM(QWidget *parent)
    : QMainWindow(parent), p_LastActs(new LastActs(&m_Tasks, &m_Activities, this))
{
	ui.setupUi(this);

	// Задачи
	p_ProxyHideDone = new HideDone(this);
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
	connect( ui.cbHideDone, SIGNAL(stateChanged(int)), this, SLOT(slot_HideDone()) );


	// Shortcuts
	p_ShcFocusTasks		= new QShortcut(QKeySequence("Ctrl+T"), this, SLOT(slot_SetFocusTasks()));
	p_ShcFocusNotes		= new QShortcut(QKeySequence("Ctrl+N"), this, SLOT(slot_SetFocusNotes()));
	p_ShcFocusAddActivity	= new QShortcut(QKeySequence("Ctrl+A"), this, SLOT(slot_SetFocusAddActivity()));
	p_ShcFocusActivities	= new QShortcut(QKeySequence("Ctrl+L"), this, SLOT(slot_SetFocusActivities()));


	p_ShcAddChildTask	= new QShortcut(QKeySequence("Ins"), this, SLOT(slot_AddItem()));
	p_ShcAddSiblingTask	= new QShortcut(QKeySequence("Shift+Ins"), this, SLOT(slot_AddSiblingItem()));
	p_ShcDelTask		= new QShortcut(QKeySequence("Del"), this, SLOT(slot_DelItem()));
	p_ShcSetFinished	= new QShortcut(QKeySequence("Space"), this, SLOT(slot_SetFinished()));

	p_ShcMoveUp		= new QShortcut(QKeySequence("Ctrl+Up"), this, SLOT(slot_MoveUp()));
	p_ShcMoveDown		= new QShortcut(QKeySequence("Ctrl+Down"), this, SLOT(slot_MoveDown()));
	p_ShcMoveLeft		= new QShortcut(QKeySequence("Ctrl+Left"), this, SLOT(slot_MoveLeft()));
	p_ShcMoveRight		= new QShortcut(QKeySequence("Ctrl+Right"), this, SLOT(slot_MoveRight()));

	if( p_ProxyHideDone->getHideDone() )
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
			QModelIndex proxyidx = ui.treeView->selectionModel()->currentIndex();
			QModelIndex idx = p_ProxyHideDone->mapToSource(proxyidx);

			TaskItem *item = m_Tasks.getItem(idx);
			if( item->getFinished().isNull() )
				item->setFinished(QDateTime::currentDateTime());
			else
				item->setFinished(QDateTime());

			m_Tasks.setDataChanged(idx);
			// Слать сигнал о изменении родителя НЕ нужно
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
		QModelIndex proxyidx = ui.treeView->selectionModel()->currentIndex();
		QModelIndex idx = p_ProxyHideDone->mapToSource(proxyidx);

		QModelIndex newidx = m_Tasks.addSibling(idx);
		QModelIndex proxynewidx = p_ProxyHideDone->mapFromSource(newidx);

		ui.treeView->edit(proxynewidx);
		ui.treeView->selectionModel()->setCurrentIndex(proxynewidx, QItemSelectionModel::ClearAndSelect);
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
		QModelIndex proxyidx = p_ProxyHideDone->mapFromSource(newidx);
		ui.treeView->selectionModel()->setCurrentIndex(proxyidx, QItemSelectionModel::ClearAndSelect);
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
		idx = p_ProxyHideDone->mapToSource(idx);
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
		QModelIndex proxyidx = ui.treeView->selectionModel()->currentIndex();
		QModelIndex idx = p_ProxyHideDone->mapToSource(proxyidx);

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

		// Заметки
		QString s = ui.Notes->toPlainText();
		if( item->getNotes()!=s )
		{
			item->setNotes(s);
			m_Tasks.setChanged();
		}

		// Время начала
		QDateTime startedCur;
		if( ui.cbStartedTime->isChecked() )
			startedCur = ui.teStartTime->dateTime();
		if( startedCur!=item->getStarted() )
		{
			item->setStarted(startedCur);
			m_Tasks.setChanged();
		}

		// Планируемое время
		if( ui.lePlannedTime->text()!=item->getPlannedTime() )
		{
			item->setPlannedTime(ui.lePlannedTime->text());
			m_Tasks.setChanged();
		}
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
		Activity act( ui.teActivityStartTime->dateTime() );

		if( ui.rbActivityTask->isChecked() )
		{
			QModelIndex proxyidx = ui.treeView->selectionModel()->currentIndex();
			QModelIndex idx = p_ProxyHideDone->mapToSource(proxyidx);

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
	QModelIndex idx = m_Tasks.getItemIndex(act.getAssignedTask());
	QModelIndex proxy = p_ProxyHideDone->mapFromSource(idx);
	ui.treeView->selectionModel()->setCurrentIndex(proxy, QItemSelectionModel::ClearAndSelect);
}

void TM::slot_BtnUpdateTime()
{
	ui.teActivityStartTime->setDateTime( QDateTime::currentDateTime() );
}

void TM::slot_SetStartTime()
{
	QModelIndex proxyidx = ui.treeView->selectionModel()->currentIndex();
	if( !proxyidx.isValid() )
		return;

	QModelIndex idx = p_ProxyHideDone->mapToSource(proxyidx);

	if( ui.cbStartedTime->isChecked() )
	{
		ui.teStartTime->setEnabled(true);
	}
	else
	{
		ui.teStartTime->setEnabled(false);
	}

	// Сохраняем изменения при смене текущей задачи, поэтому тут задачу менять не нужно
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

void TM::moveTask(Directions _dir)
{
	QModelIndex proxyidx = ui.treeView->selectionModel()->currentIndex();
	if( !proxyidx.isValid() )
		return;

	QModelIndex parent;
	int row, realrow;
	if( getNeighbourIndex(proxyidx, _dir, parent, row) )
	{
		// Нужно преобразовать row из прокси в m_Tasks
		QModelIndex idx = p_ProxyHideDone->index(row, 0, parent);
		QModelIndex realparent = p_ProxyHideDone->mapToSource(parent);
		if( idx.isValid() )
		{
			// Уже есть такой элемент, нужно просто получить его row
			realrow = p_ProxyHideDone->mapToSource(idx).row();
		}
		else
		{
			// Такого элемента нет, значит добавляем в конец
			realrow = m_Tasks.rowCount(realparent);
		}

		idx = m_Tasks.moveTask(p_ProxyHideDone->mapToSource(proxyidx), realparent, realrow);
		m_Tasks.setChanged();

		ui.treeView->selectionModel()->setCurrentIndex(p_ProxyHideDone->mapFromSource(idx), QItemSelectionModel::ClearAndSelect);
	}

}

void TM::slot_MoveUp()
{
	moveTask(UP);
}

void TM::slot_MoveDown()
{
	moveTask(DOWN);
}

void TM::slot_MoveLeft()
{
	moveTask(LEFT);
}

void TM::slot_MoveRight()
{
	moveTask(RIGHT);
}

void TM::slot_HideDone()
{
	p_ProxyHideDone->setHideDone(ui.cbHideDone->isChecked());
}


bool TM::getNeighbourIndex(const QModelIndex& _idx, Directions _dir, QModelIndex &_parent, int &_row)
{
	if( !_idx.isValid() )
		return false;

	bool res = false;
	QModelIndex parent = p_ProxyHideDone->parent(_idx);
	switch( _dir )
	{
		case UP :
		{
			if( _idx.row() )
			{
				_parent = parent;
				_row = _idx.row()-1;
				res = true;
			}
		}
		break;

		case DOWN :
		{
			if( _idx.row()!=p_ProxyHideDone->rowCount(parent)-1 )
			{
				_parent = parent;
				_row = _idx.row()+2;
				res = true;
			}
		}
		break;

		case LEFT :
		{
			if( parent.isValid() )
			{
				res = true;
				QModelIndex grandparent = p_ProxyHideDone->parent(parent);
				_parent = grandparent;
				_row = parent.row()+1;
			}
		}
		break;

		case RIGHT :
		{
			if( _idx.row() )
			{
				res = true;
				_parent = p_ProxyHideDone->index(_idx.row()-1, 0, parent);
				_row = p_ProxyHideDone->rowCount(_parent);
			}
		}
		break;

		default:
			ERROR("Unknown direction");
	}

	return res;
}

void TM::slot_Sync()
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
