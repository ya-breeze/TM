#ifndef DLGICONCHOOSE_H
#define DLGICONCHOOSE_H

#include <QDialog>
#include <QPair>

#include "iconcache.h"
namespace Ui {
    class DlgIconChoose;
}

class DlgIconChoose : public QDialog
{
    Q_OBJECT

public:
    explicit DlgIconChoose(IconCache &_icons, QWidget *parent = 0);
    ~DlgIconChoose();

    /// Вызывает диалог для выбора иконки. Если нажат Cancel - возвращает пустую строку
    QPair<bool, QString> choose(const QString& _current);

protected slots:
    void slot_AddIcon();

private:
    Ui::DlgIconChoose *ui;
    IconCache	&m_Icons;
};

#endif // DLGICONCHOOSE_H
