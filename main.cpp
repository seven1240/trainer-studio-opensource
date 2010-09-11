#include <QtGui/QApplication>
#include "mainwindow.h"
#include "fshost.h"

bool setDefaultSettings();

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

    QSettings settings;
    qDebug() << settings.fileName();
    if (settings.value("General/url").isNull()) {
        qDebug() << "set default settings";
        if(!setDefaultSettings()) {
            qDebug() << "Error set default settings!";
            exit(99);
        }
    }

    MainWindow w;
    w.showLoginDialog();
//    w.show();

    return a.exec();
}

bool setDefaultSettings()
{
    QSettings settings;
    settings.beginGroup("General");
    settings.setValue("url", "http://www.eqenglish.com");
    settings.setValue("trainer_server", "voip.idapted.com");
    settings.setValue("trainer_server_port", 7000);
    settings.endGroup();
    return true;
}
