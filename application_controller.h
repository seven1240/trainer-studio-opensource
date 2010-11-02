#ifndef APPLICATION_CONTROLLER_H
#define APPLICATION_CONTROLLER_H

#include <QObject>

class MainWindow;
class ServerConnection;
class FSHost;

class ApplicationController : public QObject
{
	Q_OBJECT

public:
	 ApplicationController();
	 ~ApplicationController();

private:
	static ServerConnection *_server;
	static FSHost *_fs;
	MainWindow *_main_window;

public:
	static ServerConnection *server();
	static FSHost *fs();
	int32_t run();

};

#endif // APPLICATION_CONTROLLER_H
