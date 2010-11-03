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
	_main_window = NULL;
	_server = NULL;
	_fs = NULL;
	_login_dialog = NULL;
	_progress_dialog = NULL;
	_user = NULL;
}

ApplicationController::~ApplicationController()
{
	if (_main_window != NULL) {
		qDebug() << "Freeing MainWindow";
		delete _main_window;
	}
	if (_progress_dialog != NULL) {
		qDebug() << "Freeing ProgressDialog";
		delete _progress_dialog;
	}
	if (_login_dialog != NULL) {
		qDebug() << "Freeing LoginDialog";
		delete _login_dialog;
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

	_progress_controller = new ProgressController(this);

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

	starting->addTransition(fs(), SIGNAL(sofiaReady()), authenticating);
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
	if (_main_window == NULL) {
		_main_window = new MainWindow();
	}
	return _main_window;
}

ProgressDialog *ApplicationController::progressDialog()
{
	if (_progress_dialog == NULL) {
		_progress_dialog = new ProgressDialog();
	}
	return _progress_dialog;
}

LoginDialog *ApplicationController::loginDialog()
{
	if (_login_dialog == NULL) {
		_login_dialog = new LoginDialog();
	}
	return _login_dialog;
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
