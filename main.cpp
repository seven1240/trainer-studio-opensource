#include <QtGui/QApplication>
#include "mainwindow.h"
#include "fshost.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setApplicationName("Trainer Studio");
    QApplication::setApplicationVersion("3.0.1");
    QApplication::setOrganizationName("Idapted Ltd.");
    QApplication::setOrganizationDomain("idapted.com");

    //start fs in background

    fshost = new FSHost();
    fshost->start();

    MainWindow w;
    w.showLoginDialog();
    w.show();

    return a.exec();
}
