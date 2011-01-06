#include <QtGui/QApplication>
#include <stdio.h>
#include <stdlib.h>
#include "application_controller.h"
#include "main_window.h"
#include "freeswitch.h"
#include "server_connection.h"
#include <lib/qtsingleapplication-2.6.1/QtSingleApplication>
#include <QMessageBox>

#define LOG_MAX_SIZE 600 * 1024

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
	QDateTime now = QDateTime::currentDateTime();

	if (!home.exists(DOTDIR "/log")) {
		if (!home.mkpath(DOTDIR "/log")) {
			fprintf(stderr, "Error opening log dir %s%s\n", home.absolutePath().toAscii().data(), DOTDIR "/log");
		}
	}

	QFile log(home.absoluteFilePath(DOTDIR "/log/trainer_studio.log"));

	if (QFileInfo(log).size() > LOG_MAX_SIZE) {
		QString fileName = log.fileName();
		log.rename(home.absoluteFilePath(DOTDIR "/log/trainer_studio_" +
			now.toString("yyyy-MM-dd-hh-mm-ss") + ".log"));
		log.setFileName(fileName);
	}

	if (!(_logFile = fopen(log.fileName().toAscii(), "a+"))) {
		fprintf(stderr, "Error opening log file %s\n", log.fileName().toAscii().data());
	}
	else {
		oldMsgHandler = qInstallMsgHandler(customMessageHandler);
	}

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

int main(int argc, char *argv[])
{
	// QApplication a(argc, argv);
	QtSingleApplication a(argc, argv);

	if (a.isRunning()) {
		QMessageBox::critical(NULL, "Trainer Studio", "Another Trainer Studio running");
		return 0;
	}
	
	QApplication::setApplicationName("Trainer Studio");
	QApplication::setApplicationVersion("3.0.3");
	QApplication::setOrganizationName("Eleutian Inc.");
	QApplication::setOrganizationDomain("eleutian.com");


	configureLogging();

	QSettings settings;
	if (settings.value("General/url").isNull()) {
		if (!setDefaultSettings()) {
			qDebug() << "Faltal: cannot write default settings!";
			return 99;
		}
	}

	// QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
	// QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

	ApplicationController *controller = new ApplicationController();
	controller->run();
	int status = a.exec();
	delete controller;
	return status;
}
