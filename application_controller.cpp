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

ApplicationController::ApplicationController()
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

	connect(_server, SIGNAL(authenticated(User*)), this, SLOT(authenticated(User*)));
	connect(_server, SIGNAL(disconnected()), this, SLOT(disconnected()));
	connect(_fs, SIGNAL(sofiaReady()), this, SLOT(sofiaReady()));

	_progress_controller = new ProgressController(this);

	progressDialog()->show();

	// loginDialog()->raise();
	// loginDialog()->show();
	// loginDialog()->activateWindow();

	return 0;
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

void ApplicationController::disconnected()
{
	mainWindow()->hide();
	loginDialog()->show();
}

void ApplicationController::sofiaReady()
{
	qDebug() << "Sofia is ready.";
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
