#ifndef DLGCALENDAR_H
#define DLGCALENDAR_H

#include <QtGui/QDialog>
#include "ui_dlgcalendar.h"

#include <QDateTime>

class DlgCalendar : public QDialog
{
    Q_OBJECT

public:
    DlgCalendar(QWidget *parent = 0);
    ~DlgCalendar();

    /// Если true - пользователь нажал Ok
    bool exec(const QDateTime& _value);
    /// Возвращает выбранное время
    QDateTime	dateTime() const;

public slots:
    void on_btnHourDec_clicked();
    void on_btnHourInc_clicked();

    void on_btnMinDec_clicked();
    void on_btnMinInc_clicked();
    void slot_Clicked(QAbstractButton*);

private:
    Ui::DlgCalendar		ui;
    QDateTime			m_DateTime;
    bool				need_Now;
};

#endif // DLGCALENDAR_H
