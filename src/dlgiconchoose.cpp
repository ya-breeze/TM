#include "dlgiconchoose.h"
#include "ui_dlgiconchoose.h"

#include <QFileDialog>
#include <QInputDialog>
#include <QLineEdit>
#include <QFileInfo>

#include "utils.h"

DlgIconChoose::DlgIconChoose(IconCache &_icons, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgIconChoose),
    m_Icons(_icons)
{
    ui->setupUi(this);

    connect(ui->btn_AddIcon, SIGNAL(clicked()), this, SLOT(slot_AddIcon()));
}

DlgIconChoose::~DlgIconChoose()
{
    delete ui;
}

/// Вызывает диалог для выбора иконки. Если нажат Cancel - возвращает пустую строку
QString DlgIconChoose::choose(const QString& _current) {
    QString res;

    // Заполним список картинками
    QStringList icons = m_Icons.getIconList();
    QListIterator<QString> i(icons);
    while (i.hasNext()) {
	QString name = i.next();
	ui->lw_Icons->addItem( new QListWidgetItem(m_Icons.restoreIcon(name), name) );
    }

    // Выберем текущую

    // Покажем диалог
    if( exec()==QDialog::Accepted ) {

    }

    return res;
}

void DlgIconChoose::slot_AddIcon() {
    QString fileName = QFileDialog::getOpenFileName(this,
	tr("Open image"), ".", tr("Image Files (*.png)"));
    // Что-то выбрали?
    if( fileName.isEmpty() )
	return;
    // Нормальная картинка?
    QIcon tmp(fileName);
    if( tmp.isNull() )
	return;

    // Как назовём?
    bool ok;
    QFileInfo fi(fileName);
    QString text = QInputDialog::getText(this, tr("What should be icon name?"),
					 tr("Icon name:"), QLineEdit::Normal,
					 fi.baseName(), &ok);
    if( !ok || text.isEmpty())
	return;

    // Добавим в список
    DEBUG("Icon is ok, will add to storage");
    m_Icons.saveIcon(text, tmp);
    ui->lw_Icons->addItem( new QListWidgetItem(tmp, text) );
}
