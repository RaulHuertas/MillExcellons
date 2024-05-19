#include "mainwindow.hpp"

#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("NutyLabs");
    QCoreApplication::setOrganizationDomain("nutylabs.com");
    QCoreApplication::setApplicationName("ExcellonToMill");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
