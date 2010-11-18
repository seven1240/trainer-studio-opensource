#include <QStateMachine>
#include "application_controller.h"
#include "main_window.h"
#include "progress_dialog.h"
#include "progress_widget.h"
#include "progress_controller.h"
#include "flash_dialog.h"
#include "flash_controller.h"
#include "login_dialog.h"
#include "echo_test_dialog.h"
#include "incoming_call_dialog.h"
#include "call_dialog.h"
#include "server_connection.h"
#include "freeswitch.h"
#include "user.h"

ServerConnection *ApplicationController::_server;
FreeSwitch *ApplicationController::_fs;
User *ApplicationController::_user;

ApplicationController::ApplicationController() : Controller(NULL)
{
	_mainWindow = NULL;
	_server = NULL;
	_fs = NULL;
	_loginDialog = NULL;
	_progressController = NULL;
	_progressDialog = NULL;
	_echoTestDialog = NULL;
	_incomingCallDialog = NULL;
	_flashDialog = NULL;
	_flashController = NULL;
	_callDialog = NULL;
	_user = NULL;
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
	if (_progressDialog != NULL) {
		qDebug() << "Freeing ProgressDialog";
		delete _progressDialog;
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

	_server= new ServerConnection();
	_server->start();

	_progressWidget = new ProgressWidget();
	_progressController = new ProgressController(_progressWidget, this);
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
	calling->setObjectName("calling");

	connect(starting, SIGNAL(entered()), this, SLOT(starting()));
	connect(authenticating, SIGNAL(entered()), this, SLOT(authenticating()));
	connect(ready, SIGNAL(entered()), this, SLOT(ready()));
	connect(incoming, SIGNAL(entered()), this, SLOT(incoming()));
	connect(training, SIGNAL(entered()), this, SLOT(training()));
	connect(testEcho, SIGNAL(entered()), this, SLOT(testEcho()));
	connect(testFlash, SIGNAL(entered()), this, SLOT(testFlash()));
	connect(calling, SIGNAL(entered()), this, SLOT(calling()));

	starting->addTransition(fs(), SIGNAL(ready()), authenticating);
	authenticating->addTransition(server(), SIGNAL(authenticated(User*)), authenticated);
	authenticated->addTransition(server(), SIGNAL(disconnected()), authenticating);
	authenticated->addTransition(echoTestDialog(), SIGNAL(finished(int)), ready);
	ready->addTransition(server(), SIGNAL(disconnected()), authenticating);
	ready->addTransition(fs(), SIGNAL(callIncoming(QString,QString,QString)), incoming);
	ready->addTransition(mainWindow(), SIGNAL(testFlash()), testFlash);
	ready->addTransition(mainWindow(), SIGNAL(testEcho()), testEcho);
	ready->addTransition(mainWindow(), SIGNAL(call()), calling);
	incoming->addTransition(fs(), SIGNAL(callAnswered(QString,QString,QString)), training);
	incoming->addTransition(fs(), SIGNAL(callEnded(QString,QString,QString)), ready);
	training->addTransition(fs(), SIGNAL(callEnded(QString,QString,QString)), ready);
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
	machine->addState(incoming);
	machine->addState(testEcho);
	machine->addState(testFlash);
	machine->setInitialState(starting);
	return machine;
}

void ApplicationController::starting()
{
	incomingCallDialog()->hide();
	progressDialog()->show();
}

void ApplicationController::authenticating()
{
	progressDialog()->hide();
	mainWindow()->hide();
	loginDialog()->show();
}

void ApplicationController::ready()
{
	progressDialog()->hide();
	mainWindow()->hide();
	loginDialog()->hide();
	echoTestDialog()->hide();
	mainWindow()->show();
}

void ApplicationController::incoming()
{
	incomingCallDialog()->show();
	incomingCallDialog()->raise();
	incomingCallDialog()->activateWindow();
}

void ApplicationController::authenticated(User *user)
{
	if (_user != NULL)
		delete _user;
	_user = user;
	progressDialog()->hide();
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
	callDialog()->show();
}

void ApplicationController::training()
{
	flashDialog()->show();
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

ProgressDialog *ApplicationController::progressDialog()
{
	if (_progressDialog == NULL)
		_progressDialog = new ProgressDialog(_progressWidget);
	return _progressDialog;
}

LoginDialog *ApplicationController::loginDialog()
{
	if (_loginDialog == NULL)
		_loginDialog = new LoginDialog(_progressWidget);
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
		return settings.value("Generatel/debugging").toBool();
	}
	return false;
}