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
#include <QShortcut>
#include <QKeySequence>

#include "Saver.h"

#include "TabletDlgTask.h"
#include "CategoryEdit.h"
#include "CalendarShower.h"
#include "dlgcalendar.h"
#include "utils.h"

TabletWindow::TabletWindow(QWidget *parent)
	: QMainWindow(parent), p_LastActs(new LastActs(&m_Tasks, &m_Activities, this))
{
	p_ProxyHideDone = new HideDone(this);
	p_ProxyHideDone->setSourceModel(&m_Tasks);
	p_ProxyHideDone->setDynamicSortFilter(true);

	p_PlannedTasks = new PlannedTaskList(&m_Tasks, this);
	p_PlannedTasks->setObjectName("PlannedTasks");

	ui.setupUi(this);

	// Подключим календари
	CalendarShower *cs = new CalendarShower(this);
	ui.lblActivityStartTime->installEventFilter(cs);
	connect(cs, SIGNAL(clicked()), this, SLOT(slot_SelectActivityDT()));

	// Задачи
	ui.treeView->setModel(p_ProxyHideDone);
	connect(p_ProxyHideDone, SIGNAL(layoutChanged()), this, SLOT(slot_ModelReset()));

	ui.tblPlannedTasks->setModel(p_PlannedTasks);
	ui.deStart->setDate(p_PlannedTasks->startDate());
	ui.deEnd->setDate(p_PlannedTasks->endDate());

	// Активности
	ui.lvLastActivities->setModel( p_LastActs );
	ui.rbActivityTask->setChecked(false);
	ui.rbActivityTask->setChecked(true);

	connect( ui.treeView->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
			this, SLOT(slot_TaskChanged(const QModelIndex&, const QModelIndex&)) );

	// Shortcuts
	new QShortcut(QKeySequence("Ctrl+Up"), this, SLOT(slot_MoveUp()));
	new QShortcut(QKeySequence("Ctrl+Down"), this, SLOT(slot_MoveDown()));
	new QShortcut(QKeySequence("Ctrl+Left"), this, SLOT(slot_MoveLeft()));
	new QShortcut(QKeySequence("Ctrl+Right"), this, SLOT(slot_MoveRight()));

	slot_Restore();
}

void TabletWindow::slot_SetFocusTasks()
{
	ui.stackedWidget->setCurrentIndex(0);
}

void TabletWindow::on_btnPlanned_clicked()
{
	ui.stackedWidget->setCurrentIndex(2);
	ui.tblPlannedTasks->resizeColumnsToContents();
	ui.tblPlannedTasks->resizeRowsToContents();

	ui.tblPlannedTasks->scrollTo( p_PlannedTasks->index(8, 0, QModelIndex()), QAbstractItemView::PositionAtTop);
}

void TabletWindow::slot_SetFocusAddActivity()
{
	slot_ActStartTime();
	ui.stackedWidget->setCurrentIndex(1);
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

void TabletWindow::slot_Save()
{
	try
	{
		QModelIndex idx = ui.treeView->selectionModel()->currentIndex();
//		slot_TaskChanged(idx, idx);

		Saver saver;
		saver.save(m_Tasks);
//		m_Tasks.setChanged(false);
		if( m_Activities.hasChanged() )
			m_Activities.save();
	}
	catch(std::exception& ex)
	{
		QMessageBox::critical(this, tr("Can't save"), ex.what());
	}
}

//void TabletWindow::slot_Sync()
//{
//	slot_Save();
//	Saver svr;
//	QProcess process(this);
//	process.setWorkingDirectory(svr.getHome());
////	process.setStandardOutputFile("/tmp/tmsync.log");
////	process.setStandardErrorFile("/tmp/tmsync.errors");
//	process.start("./sync", QStringList());
//	if( !process.waitForFinished() )
//	{
//		QMessageBox::critical(this, tr("Sync failed"), tr("Sync failed"));
//	}
//	slot_Restore();
//}

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
//			m_Tasks.setChanged();
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

	ui.rbActivityOther->setChecked(true);
	ui.rbActivityTask->setChecked(_new.isValid());
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
	try
	{
		QModelIndex proxyidx = ui.treeView->selectionModel()->currentIndex();
		QModelIndex idx = p_ProxyHideDone->mapToSource(proxyidx);

		TaskItem *item = m_Tasks.getItem(idx);
		if( !item )
			ERROR("No one task is specified");

		TabletDlgTask dlg(this);
		if( dlg.edit( item, &m_Cats ) )
		{
			// TODO По хорошему, при изменении данных задачи, TaskTree должен посылать сигнал dataChanged()
			// Но пока мы этого не делаем - нужно сбросить фильтр
			p_ProxyHideDone->invalidate();
			p_PlannedTasks->on_modelReset();
		}
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
		ui.treeView->scrollTo(ui.treeView->selectionModel()->currentIndex(), QAbstractItemView::PositionAtCenter);

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

//	if( m_Tasks.hasChanged() || m_Activities.hasChanged() )
//	{
//		int btn = QMessageBox::question(this, tr("Unsaved data"), tr("Save them?"), QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel );
//		if( btn==QMessageBox::Cancel )
//		{
//			event->ignore();
//			return;
//		}
//
//		event->accept();
//		if( btn==QMessageBox::Yes )
			slot_Save();
//	}
//	else
//		QMainWindow::closeEvent(event);
}

/// Сбрасывает время начала активности в текущее время
void TabletWindow::slot_ActStartTime()
{
	ui.lblActivityStartTime->setText( QDateTime::currentDateTime().toString(DT_FORMAT) );
}

/// Добавляет новую активность
void TabletWindow::slot_AddActivity()
{
	try
	{
		QDateTime dt = QDateTime::fromString(ui.lblActivityStartTime->text(), DT_FORMAT);
		Activity act( dt );

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

		slot_CurrentActivity();
	}
	catch(std::exception& _ex)
	{
		QMessageBox::critical(this, tr("Can't add activity"), _ex.what());
	}
}

void TabletWindow::slot_CurrentActivity()
{
	if( !m_Activities.hasCurActivity() )
		return;

	Activity act = m_Activities.getCurrentActivity();
	ui.lblCurrentActivity->setText(act.getName());
	ui.lblActivityStarted->setText( act.getStartTime().toString("yyyy.MM.dd hh:mm") );
	if( !act.getAssignedTask().isNull() )
	{
		TaskItem *item = m_Tasks.getItem(act.getAssignedTask());
		if( !item )
			ERROR("Wrong task in activity");
		ui.lblCurrentActivity->setText(item->getName());
	}
	ui.lblInterrupts->setText( QString::number(act.getInterrupts()) );
}

void TabletWindow::on_rbActivityTask_toggled()
{
	if( ui.rbActivityTask->isChecked() )
	{
		QModelIndex proxyidx = ui.treeView->selectionModel()->currentIndex();
		QModelIndex idx = p_ProxyHideDone->mapToSource(proxyidx);

		TaskItem *item = m_Tasks.getItem(idx);
		if( item )
		{
			ui.leActivityName->setText(item->getName());
		}
		else
		{
			ui.leActivityName->setText("<No task selected>");
		}

		ui.leActivityName->setEnabled(false);
	}
	else
	{
		ui.leActivityName->setEnabled(true);
		ui.leActivityName->setText("");
	}
}

/// Добавляет к текущей активности ещё одно прерывание
void TabletWindow::on_btnAddInterrupt_clicked()
{
	if( !m_Activities.hasCurActivity() )
		return;

	Activity act = m_Activities.getCurrentActivity();
	act.addInterrupt();
	ui.lblInterrupts->setText( QString::number(act.getInterrupts()) );

	m_Activities.updateActivity(act);
}

/// Вызывается при изменении текста в фильтре быстрой фильтрации
void TabletWindow::slot_FastFilter(const QString& _value)
{
	p_ProxyHideDone->setFastFilter(_value);
	ui.treeView->expandAll();
	ui.treeView->scrollTo(ui.treeView->selectionModel()->currentIndex(), QAbstractItemView::PositionAtCenter);
}

/// При изменении времени начала календаря
void TabletWindow::on_deStart_dateChanged( const QDate& _date )
{
	p_PlannedTasks->setStartDate(_date);
	on_PlannedTasks_modelReset();
}

/// При изменении времени конца календаря
void TabletWindow::on_deEnd_dateChanged( const QDate& _date )
{
	p_PlannedTasks->setEndDate(_date);
	on_PlannedTasks_modelReset();
}

/// При изменении модели календаря
void TabletWindow::on_PlannedTasks_modelReset()
{
	ui.tblPlannedTasks->resizeColumnsToContents();
	ui.tblPlannedTasks->resizeRowsToContents();
}

/// Сдвигает календарь на сегодняшний день
void TabletWindow::on_btnToday_clicked()
{
	int days = p_PlannedTasks->columnCount( QModelIndex() );
	ui.deStart->setDate(QDate::currentDate());
	ui.deEnd->setDate(QDate::currentDate().addDays(days-1));
}

/// Сдвигает календарь на неделю назад
void TabletWindow::on_btnWeekAgo_clicked()
{
	int days = p_PlannedTasks->columnCount( QModelIndex() );
	ui.deStart->setDate( ui.deStart->date().addDays(-7) );
	ui.deEnd->setDate(  ui.deStart->date().addDays(days-1)  );
//
//	p_PlannedTasks->setStartDate( p_PlannedTasks->startDate().addDays(-7) );
//	p_PlannedTasks->setEndDate(p_PlannedTasks->startDate().addDays(days));
//
//	on_PlannedTasks_modelReset();
}

/// Сдвигает календарь на неделю вперед
void TabletWindow::on_btnWeekAfter_clicked()
{
	int days = p_PlannedTasks->columnCount( QModelIndex() );
	ui.deStart->setDate( ui.deStart->date().addDays(+7) );
	ui.deEnd->setDate(  ui.deStart->date().addDays(days-1)  );
//	int days = p_PlannedTasks->columnCount( QModelIndex() );
//	p_PlannedTasks->setStartDate( p_PlannedTasks->startDate().addDays(+7) );
//	p_PlannedTasks->setEndDate(p_PlannedTasks->startDate().addDays(days));
//
//	on_PlannedTasks_modelReset();
}

void TabletWindow::resizeEvent( QResizeEvent * )
{
	ui.treeView->scrollTo(ui.treeView->selectionModel()->currentIndex(), QAbstractItemView::PositionAtCenter);
}

void TabletWindow::slot_MoveUp()
{
	moveTask(UP);
}

void TabletWindow::slot_MoveDown()
{
	moveTask(DOWN);
}

void TabletWindow::slot_MoveLeft()
{
	moveTask(LEFT);
}

void TabletWindow::slot_MoveRight()
{
	moveTask(RIGHT);
}

void TabletWindow::moveTask(Directions _dir)
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
//		m_Tasks.setChanged();

		ui.treeView->selectionModel()->setCurrentIndex(p_ProxyHideDone->mapFromSource(idx), QItemSelectionModel::ClearAndSelect);
	}
}


bool TabletWindow::getNeighbourIndex(const QModelIndex& _idx, Directions _dir, QModelIndex &_parent, int &_row)
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

/// Очищает используемый быстрый фильтр
void TabletWindow::on_btnClearFastFilter_clicked()
{
	ui.leFastFIlter->setText("");
}

void TabletWindow::slot_ModelReset()
{
	QModelIndex idx = ui.treeView->selectionModel()->currentIndex();
	slot_TaskChanged(idx, QModelIndex());
}

/// Слот для выбора времени начала активности в специальном диалоге
void TabletWindow::slot_SelectActivityDT()
{
	DlgCalendar cal(this);
	QDateTime dt = QDateTime::fromString(ui.lblActivityStartTime->text(), DT_FORMAT);
	if( cal.exec(dt) )
	{
		ui.lblActivityStartTime->setText( cal.dateTime().toString(DT_FORMAT));
	}
}
