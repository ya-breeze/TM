#include "dlgcalendar.h"

#include "utils.h"

DlgCalendar::DlgCalendar(QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);

	ui.buttonBox->addButton(DlgCalendar::tr("Now"), QDialogButtonBox::ResetRole);
	connect(ui.buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(slot_Clicked(QAbstractButton*)));
}

DlgCalendar::~DlgCalendar()
{

}

/// Если true - пользователь нажал Ok
bool DlgCalendar::exec(const QDateTime& _value)
{
	need_Now = false;
	m_DateTime = _value;
	if( !m_DateTime.isValid() )
		m_DateTime = QDateTime::currentDateTime();

	ui.calendar->setSelectedDate(m_DateTime.date());
	ui.sbHour->setValue(m_DateTime.time().hour());
	ui.sbMin->setValue(m_DateTime.time().minute());

	DEBUG(m_DateTime.time().hour() << ":" << ui.sbHour->value());
	DEBUG(m_DateTime.time().minute());

	if( QDialog::exec()==QDialog::Rejected )
		return false;

	if( need_Now )
		m_DateTime = QDateTime::currentDateTime();
	else
		m_DateTime = QDateTime( ui.calendar->selectedDate(), QTime(ui.sbHour->value(), ui.sbMin->value()) );

	return true;
}

/// Возвращает выбранное время
QDateTime DlgCalendar::dateTime() const
{
	return m_DateTime;
}

void DlgCalendar::on_btnHourDec_clicked()
{
	ui.sbHour->setValue(ui.sbHour->value()-1);
}
void DlgCalendar::on_btnHourInc_clicked()
{
	ui.sbHour->setValue(ui.sbHour->value()+1);
}

void DlgCalendar::on_btnMinDec_clicked()
{
	ui.sbMin->setValue(ui.sbMin->value()-1);
}
void DlgCalendar::on_btnMinInc_clicked()
{
	ui.sbMin->setValue(ui.sbMin->value()+1);
}

void DlgCalendar::slot_Clicked(QAbstractButton *_btn)
{
	if( _btn->text()==DlgCalendar::tr("Now") )
		need_Now = true;

	accept();
}
