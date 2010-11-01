#include <QtGui/QApplication>
#include <stdio.h>
#include <stdlib.h>
#include "main_window.h"
#include "fs_host.h"

FILE *logFile;

void myMessageHandler(QtMsgType type, const char *msg)
{
  switch (type) {
  case QtDebugMsg:
    fprintf(logFile, "Debug: %s\n", msg);
    break;
  case QtWarningMsg:
    fprintf(logFile, "Warning: %s\n", msg);
    break;
  case QtCriticalMsg:
    fprintf(logFile, "Critical: %s\n", msg);
    break;
  case QtFatalMsg:
    fprintf(logFile, "Fatal: %s\n", msg);
    abort();
  }
  fflush(logFile);
}

bool setDefaultSettings()
{
  QSettings settings;
  settings.beginGroup("General");
  settings.setValue("url", "http://www.idapted.com");
  settings.setValue("trainer_server", "voip.idapted.com");
  settings.setValue("trainer_server_port", 7000);
  settings.endGroup();
  return true;
}

int main(int argc, char *argv[])
{

  //    QtSingleApplication a(argc, argv);
  QApplication a(argc, argv);
  QApplication::setApplicationName("Trainer Studio");
  QApplication::setApplicationVersion("3.0.1");
  QApplication::setOrganizationName("Idapted Ltd.");
  QApplication::setOrganizationDomain("idapted.com");

  QDir home = QDir::home();

  if (!home.exists(DOTDIR "/log")) {
    if (!home.mkpath(DOTDIR "/log")) {
      fprintf(stderr, "Error opening log dir %s\n", home.absolutePath() + DOTDIR "/log");
    }
  }
  QFile log(home.absoluteFilePath(DOTDIR "/log/trainer_studio.log"));

  if (!(logFile = fopen(log.fileName().toAscii(), "a+"))) {
    fprintf(stderr, "Error opening log file %s\n", log.fileName().toAscii());
  }
  else {
    qInstallMsgHandler(myMessageHandler);
  }

  QDateTime now = QDateTime::currentDateTime();
  qDebug() << "\n\n============================================\n"
   "TS starting at:"
   << now.toString("yyyy-MM-dd hh:mm:ss")
   << "\n============================================\n";

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
  //We have to show the mainwindow as well,
  //Because there are no icon on taskbar if not
  w.show();

  return a.exec();

  if (logFile) fclose(logFile);
}

