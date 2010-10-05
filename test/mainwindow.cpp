#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    for(int i=0; i<1000; ++i) {
        Task task;
        task.setName("test name");
        task.setUuid( QUuid::createUuid() );

        m_Tasks.addTask(task);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
