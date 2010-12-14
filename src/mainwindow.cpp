
#include "mainwindow.h"

#include <QMessageBox>
#include <QProcess>
#include <QPair>
#include <QxtGlobalShortcut>

#include "utils.h"
#include "Saver.h"
#include "dlgcalendar.h"
#include "dlgiconchoose.h"
#include "constants.h"

#include "CategoryEdit.h"

TM::TM(QWidget *parent)
    : QMainWindow(parent), p_LastActs(new LastActs(&m_Tasks, &m_Activities, this)), m_Saver(), p_Server(new Server(m_Saver, this)),
      m_IconCache(m_Saver), m_Tasks(m_IconCache), is_UnknownActivity(false)
{
    m_Saver.init();
	ui.setupUi(this);

	// Задачи
	p_ProxyHideDone = new HideDone(this);
	p_ProxyHideDone->setSourceModel(&m_Tasks);
	p_ProxyHideDone->setDynamicSortFilter(true);
	ui.treeView->setModel(p_ProxyHideDone);

//	p_PlannedTasks = new PlannedTaskList(&m_Tasks, this);

	// Активности
	ui.lvLastActivities->setModel( p_LastActs );

	/// Slots
	connect( ui.actionAdd, SIGNAL(triggered(bool)), this, SLOT(slot_AddItem()) );
	connect( ui.actionAddChild, SIGNAL(triggered(bool)), this, SLOT(slot_AddSiblingItem()) );
	connect( ui.actionDel, SIGNAL(triggered(bool)), this, SLOT(slot_DelItem()) );
	connect( ui.actionDumpActivitiesForDate, SIGNAL(triggered(bool)), this, SLOT(slot_DumpActivitiesForDate() ) );
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
	connect( ui.btn_Icon, SIGNAL(clicked()), this, SLOT(slot_EditTaskIcon()) );

	// Net syncs
	// TODO Нужен сигнал о начале синхронизации, чтобы сохранить текущие правки
	connect( p_Server, SIGNAL(updated()), this, SLOT(slot_Restore()));

	// Shortcuts
	p_ShcFocusTasks		= new QShortcut(QKeySequence("Ctrl+T"), this, SLOT(slot_SetFocusTasks()));
	p_ShcFocusNotes		= new QShortcut(QKeySequence("Ctrl+N"), this, SLOT(slot_SetFocusNotes()));
	p_ShcFocusAddActivity	= new QShortcut(QKeySequence("Ctrl+A"), this, SLOT(slot_SetFocusAddActivity()));
	p_ShcFocusActivities	= new QShortcut(QKeySequence("Ctrl+L"), this, SLOT(slot_SetFocusActivities()));

	new QShortcut(QKeySequence("Ctrl+F"), this, SLOT(slot_SetFFFocus()));
	new QShortcut(QKeySequence("Esc"), ui.leFastFilter, SLOT(clear()));
	// start/stop 'unknown' activity
	new QShortcut(QKeySequence("F11"), this, SLOT(slot_ToggleUnknownActivity()));


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

	// Focus on tasks
	ui.treeView->setFocus();

	// popup on global shortcut
	{
	    QxtGlobalShortcut* shortcut = new QxtGlobalShortcut(this);
	    connect(shortcut, SIGNAL(activated()), this, SLOT(toggleVisibility()));
	    shortcut->setShortcut(QKeySequence("F12"));
	}

	// tray
	p_Tray = new QSystemTrayIcon(this);
	p_Tray->setIcon( QIcon(":/images/MainIcon") );
	p_Tray->show();
	QObject::connect(p_Tray, SIGNAL(activated( QSystemTrayIcon::ActivationReason )), this, SLOT(slotTray( QSystemTrayIcon::ActivationReason )));
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
    TRACE;
	ui.Notes->setText( _task.getNotes() );
	ui.lblCreateTime->setText( _task.getCreated().toString("yyyy.MM.dd H:mm") );
	ui.cbStartedTime->setChecked( !_task.getStarted().isNull() );
	ui.lePlannedTime->setText(_task.getPlannedTime());
	ui.lblRealTimeBrutto->setForegroundRole(QPalette::NoRole);
	ui.btn_Icon->setIcon( m_IconCache.restoreIcon(_task.getIconName()) );

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
	TRACE;
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
//			m_Tasks.setChanged();
		}

		// Время начала
		QDateTime startedCur;
		if( ui.cbStartedTime->isChecked() )
			startedCur = ui.teStartTime->dateTime();
		if( startedCur!=item->getStarted() )
		{
			item->setStarted(startedCur);
//			m_Tasks.setChanged();
		}

		// Планируемое время
		if( ui.lePlannedTime->text()!=item->getPlannedTime() )
		{
			item->setPlannedTime(ui.lePlannedTime->text());
//			m_Tasks.setChanged();
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

		m_Saver.save(m_Tasks);
//		m_Tasks.setChanged(false);
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
		m_Saver.restore(m_Tasks, m_Cats);

		// Нужно добавить задачу для "прочих" активностей
		if( !m_Tasks.getItem(QString(ACTIVITY_EMPTY)) ) {
		    Task task;
		    task.setId(QString(ACTIVITY_EMPTY));
		    task.setParentId(QString(EMPTY_UUID));
		    task.setName( tr("Unknown activity") );

		    m_Tasks.addChild(QString(EMPTY_UUID), task);
		}


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

void TM::slot_SetFFFocus()
{
	ui.leFastFilter->setFocus();
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

void TM::closeEvent(QCloseEvent */*event*/)
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
//		m_Tasks.setChanged();

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

//void TM::slot_Sync()
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

void TM::slot_Categories()
{
	try
	{
		CategoryEdit dlg(this, &m_Cats);

		QModelIndex proxyidx = ui.treeView->selectionModel()->currentIndex();
		QModelIndex idx = p_ProxyHideDone->mapToSource(proxyidx);

		TaskItem *item = m_Tasks.getItem(idx);
		if( !item )
			ERROR("No one task is specified");

		QStringList cats = dlg.edit(item->getCategories());
		item->setCategories(cats);
	}
	catch(std::exception& ex)
	{
		QMessageBox::critical(this, tr("Error"), ex.what());
	}
}

/// Вызывает диалог для правки списка категорий
void TM::slot_TotalCategories()
{
	try
	{
		CategoryEdit dlg(this, &m_Cats);
		dlg.edit();
		ui.treeView->scrollTo(ui.treeView->selectionModel()->currentIndex(), QAbstractItemView::PositionAtCenter);
	}
	catch(std::exception& ex)
	{
		QMessageBox::critical(this, tr("Error"), ex.what());
	}
}

/// Вызывает диалог для редактирования отображаемых категорий
void TM::slot_SelectShowCats()
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

/// Вызывается при изменении текста в фильтре быстрой фильтрации
void TM::slot_FastFilter(const QString& _value)
{
	p_ProxyHideDone->setFastFilter(_value);
	ui.treeView->expandAll();
	ui.treeView->scrollTo(ui.treeView->selectionModel()->currentIndex(), QAbstractItemView::PositionAtCenter);
}

/// Вызывается при начале синхронизации, чтобы заблокировать работу GUI
void TM::slot_StartSynchronization()
{
//    QMessageBox::critical(this, tr("Syncronization"), tr("Syncronization is starting! Please wait."));
    slot_Save();
}

/// Вызывается после окончания синхронизации
void TM::slot_StopSynchronization()
{
//    QMessageBox::critical(this, tr("Syncronization"), tr("Syncronization is finished! Good luck"));
}

// Возвращает строку из пробелов. Длина строки равна глубине вложенности задачи
QString TM::deep2Spaces(Task *task) {
	QString result;

	while( task ) {
	    if( !task->getParentId().isNull() ) {
		task = m_Tasks.getItem( task->getParentId() );
		result += "\t";
	    }
	    else
		task = NULL;
	}

	return result;
}

QString TM::fullName(Task *task) {
	QString result;

	QString fullTaskName;
	while( task ) {
	    if( !fullTaskName.isEmpty() )
		fullTaskName = ":" + fullTaskName;
	    fullTaskName = task->getName() + fullTaskName;

	    if( !task->getParentId().isNull() ) {
		task = m_Tasks.getItem( task->getParentId() );
		//result += "\t";
	    }
	    else
		task = NULL;
	}

	return result + fullTaskName;
}

/// Вызывается для отображения активностей одного дня
void TM::slot_DumpActivitiesForDate() {
    DlgCalendar cld;
    if( cld.exec(QDateTime::currentDateTime())!=QDialog::Accepted )
	return;

    DayActivities acts = m_Activities.getDay( cld.dateTime().date() );
    if( !acts.count() ) {
	DEBUG("There are no any activities");
	return;
    }


    DEBUG("There are " << acts.count() << " activities");
    int total = 0;
    QMap< QPair<QString, QString/*deep*/>, int/*condur*/ > times;
    std::cout << "---------------- " << cld.dateTime().date().toString("yyyy.MM.dd") << " ------------------" << std::endl;
    for(size_t i=0; i<acts.count(); ++i) {
	// Посчитаем длительность. Для последней активности длительность неопределена
	Activity act = acts.getActivity(i);
	int condur = -1;
	if( i!=acts.count()-1 ) {
	    Activity next = acts.getActivity(i+1);
	    condur = act.getStartTime().secsTo( next.getStartTime() );
	}

	// Получим полное название задачи - включая родителей
	Task *task = m_Tasks.getItem(act.getAssignedTask());
	QString fullTaskName;
	while( task ) {
	    if( !fullTaskName.isEmpty() )
		fullTaskName = ":" + fullTaskName;
	    fullTaskName = task->getName() + fullTaskName;

    	    // Длительность родительских задач
    	    QPair<QString, QString> key = qMakePair(fullName(task), deep2Spaces(task));
    	    //QString name = deep2Spaces(task) + task->getName();
    	    //QString name = deep2Spaces(task);// + fullTaskName;
    	    if( !times.contains(key) )
		times[ key ] = 0;
    	    if( condur!=-1 )
		times[key] += condur;

	    if( !task->getParentId().isNull() )
		task = m_Tasks.getItem( task->getParentId() );
	    else
		task = NULL;
	}

	//std::cout << fullTaskName << " - " <<(condur==-1 ? TM::tr("Unknown") : QString::number(condur/60)+" "+TM::tr("min") ) << std::endl;
	total += condur;
    }
    std::cout << "Total: " << sec2str(total) << " min" << std::endl;
    QMapIterator< QPair<QString, QString>, int> i(times);
    while (i.hasNext()) {
	i.next();
	std::cout << "\t" << i.key().second << i.key().first << ": " << sec2str(i.value()) << std::endl;
    }
    std::cout << "----------------------------------" << std::endl;
}

void TM::toggleVisibility() {
    setVisible( ! isVisible() );
}

void TM::slotTray(QSystemTrayIcon::ActivationReason _reason)
{
    if( _reason==QSystemTrayIcon::Trigger )
    {
	toggleVisibility();
    }
    else if( _reason==QSystemTrayIcon::Context )
    {
	DEBUG("Context menu");
//        QMenu menu(this);
//        menu.addAction(p_mw->actionStart);
//        menu.addAction(p_mw->actionStop);
//        menu.addAction(p_mw->actionSettings);
//        menu.addAction(p_mw->actionExit);
//        menu.exec( QCursor::pos() );
    }
}

/// Вызывается для редактирования текущей задачи
void TM::slot_EditTask() {
    TRACE;
}

/// Вызывается для редактирования иконки текущей задачи
void TM::slot_EditTaskIcon() {
    try
    {
	QModelIndex proxyidx = ui.treeView->selectionModel()->currentIndex();
	QModelIndex idx = p_ProxyHideDone->mapToSource(proxyidx);

	TaskItem *item = m_Tasks.getItem(idx);
	if( !item )
		ERROR("No one task is specified");

	DlgIconChoose dlg(m_IconCache);
	QPair<bool, QString> name = dlg.choose(item->getIconName());
	if( name.first ) {
	    item->setIconName(name.second);
	    // Задача обновилась - нужно обновить отображение
	    m_Tasks.setDataChanged(item);
	    updateTaskProperties(*item);
	    TRACE;
	}
    }
    catch(std::exception& _ex)
    {
	    QMessageBox::critical(this, tr("Can't change icon"), _ex.what());
    }
}

/// Включает/выключает активность "прочее"
void TM::slot_ToggleUnknownActivity() {
    TRACE;

    try
    {
	is_UnknownActivity = !is_UnknownActivity;

	Activity act;
	if( is_UnknownActivity ) {
	    // Нужно начать активность "прочее"

	    // Проверим, что мы ещё не начали эту активность
	    DayActivities &acts = m_Activities.getTodayActs();
	    if( acts.count() ) {
		const Activity &lastact = acts.getActivity(acts.count()-1);
		if( lastact.getAssignedTask()==QString(ACTIVITY_EMPTY) ) {
		    DEBUG("'Unknown' activity is already started");
		    return;
		}
	    }


	    TaskItem *item = m_Tasks.getItem( QString(ACTIVITY_EMPTY) );
	    if( !item )
		    ERROR("There are no 'unknown activity' task");
	    act.setAssignedTask(item->getId());
	    act.setName( item->getName() );
	} else {
	    // Нужно завершить активность "прочее"
	    DayActivities &acts = m_Activities.getTodayActs();
	    if( acts.count()>1 ) {
		const Activity &lastact = acts.getActivity(acts.count()-2);
		act.setAssignedTask(lastact.getAssignedTask());
		act.setName(lastact.getName());
	    } else
		return;
	}

	m_Activities.addActivity(act, true);
	ui.tabMain->setCurrentIndex(1);
	slot_CurrentActivity();
    }
    catch(std::exception& _ex)
    {
	    QMessageBox::critical(this, tr("Can't add activity"), _ex.what());
    }
}
