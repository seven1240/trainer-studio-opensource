#include <QtGui/QApplication>
#include "mainwindow.h"
#include "fshost.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //start fs in background

    fshost = new FSHost();
    fshost->start();

    MainWindow w;
    w.showLoginDialog();
    w.show();

    return a.exec();
}
