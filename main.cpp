/*
 * Eleutian Trainer Studio Application Opensource Edition, a VoIP client with Webkit and Flash support.
 * Copyright (C) 2011, Eleutian Technology, LLC <http://www.eleutian.com>
 *
 * Version: Apache License 2.0
 *
 * The content of this file is licensed under the Apache License, Version 2.0. (the "License").
 * You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.apache.org/licenses/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is Eleutian Trainer Studio Application Opensource Edition.
 *
 * The Initial Developer of the Original Code is
 * Seven Du <seven@eleutian.com>
 * Portions created by the Initial Developer are Copyright (C)
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * Jadob Lewallen <jacob@eleutian.com>
 */
#include <QtGui/QApplication>
#include <stdio.h>
#include <stdlib.h>
#include "application_controller.h"
#include "main_window.h"
#include "freeswitch.h"
#include "server_connection.h"
#include <lib/qtsingleapplication-2.6.1/QtSingleApplication>
#include <QMessageBox>
#include <QSplashScreen>

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
	settings.setValue("url", "http://localhost:9393");
	settings.setValue("trainer_server", "localhost");
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
	QApplication::setApplicationVersion("1.0.0");
	QApplication::setOrganizationName("Eleutian Inc.");
	QApplication::setOrganizationDomain("eleutian.com");

	QPixmap image(":/images/splash.png");
    QSplashScreen *splash = new QSplashScreen(image);
    splash->show();
    splash->showMessage("Initializing ...", Qt::AlignLeft|Qt::AlignBottom, Qt::blue);
	splash->repaint();
	splash->raise();

	configureLogging();

	QSettings settings;
	if (settings.value("General/url").isNull()) {
		if (!setDefaultSettings()) {
			qDebug() << "Faltal: cannot write default settings!";
			return 99;
		}
	}

	ApplicationController *controller = new ApplicationController();
	controller->run();

    QObject::connect(ApplicationController::fs(), SIGNAL(loadedForSplash(QString,int,QColor)),
		splash, SLOT(showMessage(QString,int,QColor)));
    QObject::connect(ApplicationController::fs(), SIGNAL(ready()), splash, SLOT(close()));

	//raise more hard for windows
	splash->raise();
	int status = a.exec();
//	delete controller;
	return status;
}
