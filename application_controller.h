#ifndef APPLICATION_CONTROLLER_H
#define APPLICATION_CONTROLLER_H

#include <QObject>

class MainWindow;
class ServerConnection;
class FSHost;
class LoginDialog;
class User;

class ApplicationController : public QObject
{
	Q_OBJECT

public:
	 ApplicationController();
	 ~ApplicationController();

private:
	static ServerConnection *_server;
	static FSHost *_fs;
	static User *_user;
	LoginDialog *_login_dialog;
	MainWindow *_main_window;

private:
	MainWindow *mainWindow();
	LoginDialog *loginDialog();

private slots:
	void authenticated(User *user);
	void disconnected();
	void sofiaReady();

public:
	static ServerConnection *server();
	static FSHost *fs();
	static User *user();
	int run();

};

#endif // APPLICATION_CONTROLLER_H
