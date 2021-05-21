#include "mainwindow.h"
#include"dialogofabout.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    QDialog* b = new DialogOfAbout();
    b->show();
    return a.exec();
}
