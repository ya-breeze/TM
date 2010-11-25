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
QPair<bool, QString> DlgIconChoose::choose(const QString& _current) {
    QPair<bool, QString> res = qMakePair(false, QString());

    // Добавим пустую картинку
    ui->lw_Icons->addItem( new QListWidgetItem(tr("<Empty>")) );

    // Заполним список картинками
    QStringList icons = m_Icons.getIconList();
    QListIterator<QString> i(icons);
    while (i.hasNext()) {
	QString name = i.next();
	QIcon icon = m_Icons.restoreIcon(name);
	if( icon.isNull() )
	    continue;

	ui->lw_Icons->addItem( new QListWidgetItem(icon, name) );
	// Выберем текущую
	if( name==_current ) {
	    ui->lw_Icons->setCurrentRow(ui->lw_Icons->count()-1);
	}
    }

    // Покажем диалог
    if( exec()==QDialog::Accepted ) {
	res.first = true;

	QListWidgetItem *curr = ui->lw_Icons->currentItem();
	if( !curr->icon().isNull() ) {
	    res.second = curr->text();
	}
    }

    return res;
}

void DlgIconChoose::slot_AddIcon() {
    QStringList fileNames = QFileDialog::getOpenFileNames(this,
	tr("Open image"), ".", tr("Image Files (*.png)"));

    QListIterator<QString> i(fileNames);
    while( i.hasNext() ) {
	QString fileName = i.next();
	// Что-то выбрали?
	if( fileName.isEmpty() )
	    continue;
	// Нормальная картинка?
	QIcon tmp(fileName);
	if( tmp.isNull() )
	    continue;

	// Как назовём?
	QFileInfo fi(fileName);
//	bool ok;
//	QString text = QInputDialog::getText(this, tr("What should be icon name?"),
//					     tr("Icon name:"), QLineEdit::Normal,
//					     fi.baseName(), &ok);
//	if( !ok || text.isEmpty())
//	    continue;

	// Добавим в список
	DEBUG("Icon is ok, will add to storage");
	m_Icons.saveIcon(fi.baseName(), tmp);
	ui->lw_Icons->addItem( new QListWidgetItem(tmp, fi.baseName()) );
    }
}
