/*
 * Eleutian Trainer Studio Application Opensource Edition, a VoIP client with Webkit and Flash support.
 * Copyright (C) 2011, Eleutian Technology, LLC <http://www.eleutian.com>
 *
 * Version: Apache License 2.0
 *
 * The content of this file is licensed under the Apache License, Version 2.0.  (the "License").
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
 * Jadob Lewallen <jacob@eleutian.com>
 * Portions created by the Initial Developer are Copyright (C)
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * Seven Du <seven@eleutian.com>
 */

#include <QStateMachine>
#include <QtGui/QApplication>
#include <QMessageBox>
#include <QDesktopServices>
#include "application_controller.h"
#include "main_window.h"
#include "flash_dialog.h"
#include "flash_controller.h"
#include "login_dialog.h"
#include "echo_test_dialog.h"
#include "incoming_call_dialog.h"
#include "call_dialog.h"
#include "server_connection.h"
#include "freeswitch.h"
#include "user.h"
#include "utils.h"

ServerConnection *ApplicationController::_server;
FreeSwitch *ApplicationController::_fs;
User *ApplicationController::_user;

ApplicationController::ApplicationController() : Controller(NULL)
{
	_mainWindow = NULL;
	_server = NULL;
	_fs = NULL;
	_loginDialog = NULL;
	_echoTestDialog = NULL;
	_incomingCallDialog = NULL;
	_flashDialog = NULL;
	_flashController = NULL;
	_callDialog = NULL;
	_user = NULL;
	_wrapupTimer = NULL;
}

ApplicationController::~ApplicationController()
{
	if (_mainWindow != NULL) {
		qDebug() << "Freeing MainWindow";
		delete _mainWindow;
	}
	if (_incomingCallDialog != NULL) {
		qDebug() << "Freeing IncomingCallDialog";
		delete _incomingCallDialog;
	}
	if (_callDialog != NULL) {
		qDebug() << "Freeing CallDialog";
		delete _callDialog;
	}
	if (_loginDialog != NULL) {
		qDebug() << "Freeing LoginDialog";
		delete _loginDialog;
	}
	if (_fs != NULL) {
		qDebug() << "Freeing FS";
		_fs->shutdown();
		_fs->wait();
		delete _fs;
	}
	if (_server != NULL) {
		qDebug() << "Freeing ServerConnection";
		_server->shutdown();
		_server->wait();
		delete _server;
	}
	if (_user != NULL)
		delete _user;

	qDebug() << "Stopped";
}

int ApplicationController::run()
{
	_fs = new FreeSwitch();
	_fs->start();

	_server = new ServerConnection();
	_server->start();

	_wrapupTimer = new QTimer(this);
	_wrapupTimer->setInterval(5000);
	_wrapupTimer->setSingleShot(true);

	_flashController = new FlashController(flashDialog(), this);

	connect(server(), SIGNAL(authenticated(User*)), this, SLOT(authenticated(User*)));

	startStateMachine();

	return 0;
}

QStateMachine *ApplicationController::createStateMachine()
{
	QState *starting = new QState();
	QState *authenticating = new QState();
	QState *authenticated = new QState();
	QState *stopped = new QState();
	QState *testEcho = new QState();
	QState *ready = new QState();
	QState *incoming = new QState();
	QState *training = new QState();
	QState *wrapup = new QState();
	QState *wrapupTimeout = new QState();
	QState *testFlash = new QState();
	QState *calling = new QState();

	starting->setObjectName("starting");
	authenticating->setObjectName("authenticating");
	authenticated->setObjectName("authenticated");
	stopped->setObjectName("stopped");
	ready->setObjectName("ready");
	testEcho->setObjectName("test-echo");
	testFlash->setObjectName("test-flash");
	incoming->setObjectName("incoming");
	training->setObjectName("training");
	wrapup->setObjectName("wrapup");
	wrapupTimeout->setObjectName("wrapupTimeout");
	calling->setObjectName("calling");

	connect(starting, SIGNAL(entered()), this, SLOT(starting()));
	connect(authenticating, SIGNAL(entered()), this, SLOT(authenticating()));
	connect(ready, SIGNAL(entered()), this, SLOT(ready()));
	connect(incoming, SIGNAL(entered()), this, SLOT(incoming()));
	connect(training, SIGNAL(entered()), this, SLOT(training()));
	connect(wrapup, SIGNAL(entered()), this, SLOT(wrapup()));
	connect(wrapupTimeout, SIGNAL(entered()), this, SLOT(wrapupTimeout()));
	connect(testEcho, SIGNAL(entered()), this, SLOT(testEcho()));
	connect(testFlash, SIGNAL(entered()), this, SLOT(testFlash()));
	connect(calling, SIGNAL(entered()), this, SLOT(calling()));

	starting->addTransition(fs(), SIGNAL(ready()), authenticating);
	authenticating->addTransition(server(), SIGNAL(authenticated(User*)), authenticated);
	authenticated->addTransition(server(), SIGNAL(disconnected()), authenticating);
	authenticated->addTransition(echoTestDialog(), SIGNAL(finished(int)), ready);
	ready->addTransition(server(), SIGNAL(disconnected()), authenticating);
	ready->addTransition(fs(), SIGNAL(newInteractionCall(QString,QString,QString)), incoming);
	ready->addTransition(fs(), SIGNAL(newIncomingCall(QString,QString,QString)), calling);
	ready->addTransition(mainWindow(), SIGNAL(testFlash()), testFlash);
	ready->addTransition(mainWindow(), SIGNAL(testEcho()), testEcho);
	ready->addTransition(mainWindow(), SIGNAL(call()), calling);
	incoming->addTransition(fs(), SIGNAL(callAnswered(QString,QString,QString)), training);
	incoming->addTransition(fs(), SIGNAL(callEnded(QString,QString,QString)), ready);
	training->addTransition(flashDialog(), SIGNAL(closed()), wrapup);
	wrapup->addTransition(server(), SIGNAL(pausing()), ready);
	wrapup->addTransition(_wrapupTimer, SIGNAL(timeout()), wrapupTimeout);
	wrapupTimeout->addTransition(server(), SIGNAL(unpaused()), ready);
	wrapupTimeout->addTransition(server(), SIGNAL(paused()), ready);
	calling->addTransition(callDialog(), SIGNAL(closed()), ready);
	testFlash->addTransition(flashDialog(), SIGNAL(closed()), ready);
	testEcho->addTransition(echoTestDialog(), SIGNAL(closed()), ready);

	QStateMachine *machine = new QStateMachine(this);
	machine->setObjectName("AC");
	machine->addState(starting);
	machine->addState(authenticating);
	machine->addState(authenticated);
	machine->addState(ready);
	machine->addState(calling);
	machine->addState(stopped);
	machine->addState(training);
	machine->addState(wrapup);
	machine->addState(wrapupTimeout);
	machine->addState(incoming);
	machine->addState(testEcho);
	machine->addState(testFlash);
	machine->setInitialState(starting);
	return machine;
}

void ApplicationController::starting()
{
	incomingCallDialog()->hide();
}

void ApplicationController::authenticating()
{
	echoTestDialog()->hide();
	mainWindow()->hide();
	loginDialog()->show();
	loginDialog()->activateWindow();
}

void ApplicationController::ready()
{
	mainWindow()->hide();
	loginDialog()->hide();
	echoTestDialog()->hide();
	mainWindow()->show();
	mainWindow()->raise();
}

void ApplicationController::incoming()
{
	incomingCallDialog()->show();
	incomingCallDialog()->raise();
	incomingCallDialog()->activateWindow();
}

void ApplicationController::authenticated(User *user)
{
	int x;
	QString serverVersion;
	QString clientVersion;

	if (_user != NULL)	delete _user;
	_user = user;

	serverVersion = _user->getServerVersion();
	clientVersion = QApplication::applicationVersion();

	qDebug() << "ServerVersion: " << serverVersion;
	qDebug() << "ClientVersion: " << clientVersion;

	x = Utils::compareVersion(clientVersion, serverVersion);

	qDebug() << "compareVersion: " << x;

	if (x > 1) {
		QString title = QString("New Version Available");
		QString msg = QString("A new version of TS [%1] available, "
			"please download the new version from the website.\n"
			" TS will be closed when you click [OK]").arg(serverVersion);
		QMessageBox::critical(NULL, title, msg);
		QDesktopServices::openUrl(QUrl(TS_DOWNLOAD_URL));
		exit(0);
	} else if (x == 1) {
		QString title = QString("New Version Available");
		QString msg = QString("A new version of TS [%1] available, "
			"it is highly recommended to upgrade to the new version.\n"
			"Click [Yes] ignore this message, or [No] to close TS"
			" and download the new version from the website.").arg(serverVersion);
		int ret = QMessageBox::warning(NULL, title, msg,
			QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
		if (ret == QMessageBox::No) {
			QDesktopServices::openUrl(QUrl(TS_DOWNLOAD_URL));
			exit(0);
		}
	}

	loginDialog()->hide();
	mainWindow()->hide();
	echoTestDialog()->show();
}

void ApplicationController::testEcho()
{
	echoTestDialog()->show();
}

void ApplicationController::testFlash()
{
	flashDialog()->show();
}

void ApplicationController::calling()
{
	server()->pause(true);
	callDialog()->show();
}

void ApplicationController::training()
{
	flashDialog()->show();
}

void ApplicationController::wrapup()
{
	mainWindow()->raise();
	mainWindow()->activateWindow();
	_wrapupTimer->start();
}

void ApplicationController::wrapupTimeout()
{
	server()->pause(false);
}

MainWindow *ApplicationController::mainWindow()
{
	if (_mainWindow == NULL) {
		_mainWindow = new MainWindow();
		connect(_mainWindow, SIGNAL(testEcho()), this, SLOT(testEcho()));
	}
	return _mainWindow;
}

EchoTestDialog *ApplicationController::echoTestDialog()
{
	if (_echoTestDialog == NULL)
		_echoTestDialog = new EchoTestDialog();
	return _echoTestDialog;
}

FlashDialog *ApplicationController::flashDialog()
{
	if (_flashDialog == NULL)
		_flashDialog = new FlashDialog();
	return _flashDialog;
}

IncomingCallDialog *ApplicationController::incomingCallDialog()
{
	if (_incomingCallDialog == NULL)
		_incomingCallDialog = new IncomingCallDialog();
	return _incomingCallDialog;
}

CallDialog *ApplicationController::callDialog()
{
	if (_callDialog == NULL)
		_callDialog = new CallDialog();
	return _callDialog;
}

LoginDialog *ApplicationController::loginDialog()
{
	if (_loginDialog == NULL)
		_loginDialog = new LoginDialog();
	return _loginDialog;
}

ServerConnection *ApplicationController::server()
{
	return _server;
}

FreeSwitch *ApplicationController::fs()
{
	return _fs;
}

User *ApplicationController::user()
{
	return _user;
}

bool ApplicationController::isDebugging()
{
	QSettings settings;
	if (!settings.value("General/debugging").isNull()) {
		return settings.value("General/debugging").toBool();
	}
	return false;
}
