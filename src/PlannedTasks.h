#ifndef PLANNEDTASKS_H
#define PLANNEDTASKS_H

#include <QtGui/QDialog>
#include "ui_PlannedTasks.h"

class PlannedTasks : public QDialog
{
    Q_OBJECT

public:
    PlannedTasks(QWidget *parent = 0);
    ~PlannedTasks();

private:
    Ui::PlannedTasksClass ui;
};

#endif // PLANNEDTASKS_H
