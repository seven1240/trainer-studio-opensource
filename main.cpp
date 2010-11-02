#include <QtGui/QApplication>
#include <stdio.h>
#include <stdlib.h>
#include "application_controller.h"
#include "main_window.h"
#include "fs_host.h"
#include "server_connection.h"

FILE *_logFile = NULL;
QtMsgHandler oldMsgHandler = NULL;

void customMessageHandler(QtMsgType type, const char *msg)
{
	switch (type) {
	case QtDebugMsg:
		fprintf(_logFile, "DBUG: %s\n", msg);
		break;
	case QtWarningMsg:
		fprintf(_logFile, "WARN: %s\n", msg);
		break;
	case QtCriticalMsg:
		fprintf(_logFile, "CRIT: %s\n", msg);
		break;
	case QtFatalMsg:
		fprintf(_logFile, "FATL: %s\n", msg);
		abort();
	}
	fflush(_logFile);
	if (oldMsgHandler != NULL) {
		oldMsgHandler(type, msg);
	}
}

void configureLogging()
{
	QDir home = QDir::home();

	if (!home.exists(DOTDIR "/log")) {
		if (!home.mkpath(DOTDIR "/log")) {
			fprintf(stderr, "Error opening log dir %s%s\n", home.absolutePath().toAscii().data(), DOTDIR "/log");
		}
	}
	QFile log(home.absoluteFilePath(DOTDIR "/log/trainer_studio.log"));

	if (!(_logFile = fopen(log.fileName().toAscii(), "a+"))) {
		fprintf(stderr, "Error opening log file %s\n", log.fileName().toAscii().data());
	}
	else {
		oldMsgHandler = qInstallMsgHandler(customMessageHandler);
	}

	QDateTime now = QDateTime::currentDateTime();
	qDebug() << "\n\n";
	qDebug() << "============================================";
	qDebug() << "TS starting at: " << now.toString("yyyy-MM-dd hh:mm:ss");
	qDebug() << "============================================";
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

int32_t main(int32_t argc, char *argv[])
{
	QApplication a(argc, argv);
	QApplication::setApplicationName("Trainer Studio");
	QApplication::setApplicationVersion("3.0.1");
	QApplication::setOrganizationName("Idapted Ltd.");
	QApplication::setOrganizationDomain("idapted.com");

	configureLogging();

	QSettings settings;
	if (settings.value("General/url").isNull()) {
		if (!setDefaultSettings()) {
			return 99;
		}
	}

	ApplicationController *controller = new ApplicationController();
	controller->run();
	int32_t status = a.exec();
	delete controller;
	return status;
}
