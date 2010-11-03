#include <QStateMachine>
#include "application_controller.h"
#include "main_window.h"
#include "progress_dialog.h"
#include "progress_controller.h"
#include "login_dialog.h"
#include "server_connection.h"
#include "fs_host.h"
#include "user.h"

ServerConnection *ApplicationController::_server;
FSHost *ApplicationController::_fs;
User *ApplicationController::_user;

ApplicationController::ApplicationController() : Controller(NULL)
{
	_mainWindow = NULL;
	_server = NULL;
	_fs = NULL;
	_loginDialog = NULL;
	_progressDialog = NULL;
	_user = NULL;
}

ApplicationController::~ApplicationController()
{
	if (_mainWindow != NULL) {
		qDebug() << "Freeing MainWindow";
		delete _mainWindow;
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
	if (_user != NULL) {
		delete _user;
	}
	qDebug() << "Stopped";
}

int ApplicationController::run()
{
	_fs = new FSHost();
	_fs->start();

	_server= new ServerConnection();
	_server->start();

	_progressController = new ProgressController(this);

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

	starting->setObjectName("starting");
	authenticating->setObjectName("authenticating");
	authenticated->setObjectName("authenticated");
	stopped->setObjectName("stopped");

	connect(starting, SIGNAL(entered()), this, SLOT(starting()));
	connect(authenticating, SIGNAL(entered()), this, SLOT(authenticating()));

	starting->addTransition(fs(), SIGNAL(allModulesLoaded()), authenticating);
	authenticating->addTransition(server(), SIGNAL(authenticated(User*)), authenticated);
	authenticated->addTransition(server(), SIGNAL(disconnected()), authenticating);

	QStateMachine *machine = new QStateMachine(this);
	machine->addState(starting);
	machine->addState(authenticating);
	machine->addState(authenticated);
	machine->addState(stopped);
	machine->setInitialState(starting);
	return machine;
}

void ApplicationController::starting()
{
	progressDialog()->show();
}

void ApplicationController::authenticating()
{
	progressDialog()->hide();
	loginDialog()->show();
}

void ApplicationController::authenticated(User *user)
{
	if (_user != NULL) {
		delete _user;
	}
	_user = user;
	loginDialog()->hide();
	mainWindow()->show();
}

MainWindow *ApplicationController::mainWindow()
{
	if (_mainWindow == NULL) {
		_mainWindow = new MainWindow();
	}
	return _mainWindow;
}

ProgressDialog *ApplicationController::progressDialog()
{
	if (_progressDialog == NULL) {
		_progressDialog = new ProgressDialog(_progressController);
	}
	return _progressDialog;
}

LoginDialog *ApplicationController::loginDialog()
{
	if (_loginDialog == NULL) {
		_loginDialog = new LoginDialog(_progressController);
	}
	return _loginDialog;
}

ServerConnection *ApplicationController::server()
{
	return _server;
}

FSHost *ApplicationController::fs()
{
	return _fs;
}

User *ApplicationController::user()
{
	return _user;
}
