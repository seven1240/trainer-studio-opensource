#include "application_controller.h"
#include "main_window.h"
#include "login_dialog.h"
#include "server_connection.h"
#include "fs_host.h"

ServerConnection *ApplicationController::_server;
FSHost *ApplicationController::_fs;

ApplicationController::ApplicationController()
{
	_main_window = NULL;
	_server = NULL;
	_fs = NULL;
}

ApplicationController::~ApplicationController()
{
	if (_main_window != NULL) {
		qDebug() << "Freeing MainWindow";
		delete _main_window;
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

	// _main_window = new MainWindow();
	// _main_window->showLoginDialog();
	// _main_window->show();

	LoginDialog *login_dialog = new LoginDialog();
	login_dialog->raise();
	login_dialog->show();
	login_dialog->activateWindow();

	return 0;
}

ServerConnection *ApplicationController::server()
{
	return _server;
}

FSHost *ApplicationController::fs()
{
	return _fs;
}
