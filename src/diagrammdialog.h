#ifndef DIAGRAMMDIALOG_H
#define DIAGRAMMDIALOG_H

#include <QDialog>

#include "Activities.h"
#include "TaskTree.h"
#include "diagramm/diagramm.h"
#include "LastActs.h"

namespace Ui {
    class DiagrammDialog;
}

class DiagrammDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DiagrammDialog(Activities &_acts, const TaskTree& _tasks, QWidget *parent = 0);
    ~DiagrammDialog();

protected slots:
    void onDateSelected();

private:
    Ui::DiagrammDialog *ui;
    Diagramm    *p_Diagramm;
    Activities &m_Acts;
    const TaskTree& m_Tasks;
    LastActs	*p_LastActs;
};

#endif // DIAGRAMMDIALOG_H
