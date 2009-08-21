#include "mainwindow.h"

#include <QtGui>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TM w;
    w.show();
    return a.exec();
}
