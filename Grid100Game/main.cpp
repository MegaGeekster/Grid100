#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QApplication::setOrganizationName("MegaGeekster");
    QApplication::setApplicationName("Grid100");

    MainWindow w;
    w.show();
    return a.exec();
}
