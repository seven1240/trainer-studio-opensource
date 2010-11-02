#include "application_controller.h"
#include "main_window.h"
#include "server_connection.h"
#include "fs_host.h"

ApplicationController::ApplicationController()
{
	_main_window = NULL;
	server_connection = NULL;
	fs = NULL;
}

ApplicationController::~ApplicationController()
{
	if (_main_window != NULL) {
		qDebug() << "Freeing MainWindow";
		delete _main_window;
	}
	if (fs != NULL) {
		qDebug() << "Freeing FS";
		fs->shutdown();
		fs->wait();
		delete fs;
		fs = NULL;
	}
	if (server_connection != NULL) {
		qDebug() << "Freeing ServerConnection";
		server_connection->shutdown();
		server_connection->wait();
		delete server_connection;
		server_connection = NULL;
	}
	qDebug() << "Stopped";
}

int32_t ApplicationController::run()
{
	fs = new FSHost();
	fs->start();

	server_connection = new ServerConnection();
	server_connection->start();

	_main_window = new MainWindow();
	_main_window->showLoginDialog();
	_main_window->show();

	return 0;
}
