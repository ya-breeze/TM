#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slot_test() {
    for(int i=0;i<100;++i) {
        Task task;
        task.setName("test");
        m_Tasks.addTask(task);

        for(int i=0;i<100;++i) {
            Task task2;
            task2.setParentUuid(task.getUuid());
            task2.setName("test2");
            m_Tasks.addTask(task2);
        }
    }
}
