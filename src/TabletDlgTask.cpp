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
#include "dlgcalendar.h"
#include "utils.h"

TabletDlgTask::TabletDlgTask( IconCache& _icons, QWidget *parent )
	: QDialog(parent), m_Icons(_icons)
{
	ui.setupUi(this);
}

bool TabletDlgTask::edit(TaskItem *_item, CategoryTree *_cats)
{
	Q_ASSERT(_item && _cats);

	m_Task = _item;
	m_Cats = _cats;

	// Покажем свойства задачи
	if( !m_Task->getStarted().isNull() )
	{
		ui.cbStartedTime->setChecked(true);
		ui.btnStartTime->setText(m_Task->getStarted().toString(DT_FORMAT));
	}
	else
	{
		slot_UpdateStartTime();
	}
	if( !m_Task->getFinished().isNull() )
	{
		ui.cbFinishTime->setChecked(true);
		ui.lblFinishTime->setText(m_Task->getFinished().toString("yyyy.MM.dd H:mm"));
	}
	else
	{
		ui.lblFinishTime->setText(tr("<not finished yet>"));
	}
	ui.lePlannedTime->setText(m_Task->getPlannedTime());

	// Картинка
	ui.btn_Icon->setIcon( m_Icons.restoreIcon(m_Task->getIconName()) );

	if( exec()==QDialog::Rejected )
		return false;

	// Теперь изменим текущую задачу
	if( ui.cbStartedTime->isChecked() )
	{
		QDateTime dt = QDateTime::fromString(ui.btnStartTime->text(), DT_FORMAT);
		m_Task->setStarted(dt);
	}
	else
		m_Task->setStarted( QDateTime() );
	if( ui.cbFinishTime->isChecked() )
		m_Task->setFinished( QDateTime::fromString(ui.lblFinishTime->text(), "yyyy.MM.dd H:mm") );
	else
		m_Task->setFinished( QDateTime() );
	m_Task->setPlannedTime(ui.lePlannedTime->text());

	// TODO Пошлём сигнал об изменении данных


	return true;
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

/// Вызывается при смене состояния признака окончания
void TabletDlgTask::slot_Finish(int _state)
{
	if( _state==Qt::Checked )
	{
		ui.lblFinishTime->setText( QDateTime::currentDateTime().toString("yyyy.MM.dd H:mm") );
	}
	else
	{
		ui.lblFinishTime->setText(tr("<not finished yet>"));
	}
}
/// Выставляет время начала в текущее время
void TabletDlgTask::slot_UpdateStartTime()
{
	ui.btnStartTime->setText( QDateTime::currentDateTime().toString(DT_FORMAT) );
}

/// Слот для выбора времени начала в специальном диалоге
void TabletDlgTask::on_btnStartTime_clicked()
{
	DlgCalendar cal(this);
	QDateTime dt = QDateTime::fromString(ui.btnStartTime->text(), DT_FORMAT);
	if( cal.exec(dt) )
	{
		ui.btnStartTime->setText( cal.dateTime().toString(DT_FORMAT));
	}
}
