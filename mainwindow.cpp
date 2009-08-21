#include "mainwindow.h"

#include "exceptions.h"
#include <QMessageBox>

TM::TM(QWidget *parent)
    : QMainWindow(parent)
{
	ui.setupUi(this);

	Task task("First"), task2("Second");
	task2.setParent(task.getId());

	m_Tasks.addChild(QModelIndex(), task);
	m_Tasks.addChild(task.getId(), task2);
	ui.treeView->setModel(&m_Tasks);
	ui.treeView->reset();
	ui.treeView->expandAll();
	ui.listView->setModel(&m_Tasks);


	/// Slots
	connect( ui.actionAdd, SIGNAL(triggered(bool)), this, SLOT(slot_AddItem()) );
	connect( ui.actionAddChild, SIGNAL(triggered(bool)), this, SLOT(slot_AddChildItem()) );
	connect( ui.actionDel, SIGNAL(triggered(bool)), this, SLOT(slot_DelItem()) );
}

TM::~TM()
{

}

void TM::slot_AddItem()
{
	try
	{
		QModelIndex idx = ui.treeView->selectionModel()->currentIndex();
		m_Tasks.addChild(idx);
	}
	catch(std::exception& ex)
	{
		QMessageBox::critical(this, tr("title"), ex.what());
	}
}

void TM::slot_AddChildItem()
{
	TRACE;
}

void TM::slot_DelItem()
{
	TRACE;
}
