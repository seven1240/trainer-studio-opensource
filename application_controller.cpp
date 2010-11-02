#include "application_controller.h"
#include "main_window.h"
#include "login_dialog.h"
#include "server_connection.h"
#include "fs_host.h"
#include "user.h"

ServerConnection *ApplicationController::_server;
FSHost *ApplicationController::_fs;

ApplicationController::ApplicationController()
{
	_main_window = NULL;
	_server = NULL;
	_fs = NULL;
	_login_dialog = NULL;
}

ApplicationController::~ApplicationController()
{
	if (_main_window != NULL) {
		qDebug() << "Freeing MainWindow";
		delete _main_window;
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
	qDebug() << "Stopped";
}

int32_t ApplicationController::run()
{
	_fs = new FSHost();
	_fs->start();

	_server= new ServerConnection();
	_server->start();

	connect(_server, SIGNAL(authenticated(User*)), this, SLOT(authenticated(User*)));
	connect(_server, SIGNAL(disconnected()), this, SLOT(disconnected()));

	loginDialog()->raise();
	loginDialog()->show();
	loginDialog()->activateWindow();

	return 0;
}

void ApplicationController::authenticated(User *user)
{
	loginDialog()->hide();
	mainWindow()->show();
}

void ApplicationController::disconnected()
{
	mainWindow()->hide();
	loginDialog()->show();
}

MainWindow *ApplicationController::mainWindow()
{
	if (_main_window == NULL) {
		_main_window = new MainWindow();
	}
	return _main_window;
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
