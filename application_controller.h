#ifndef APPLICATION_CONTROLLER_H
#define APPLICATION_CONTROLLER_H

#include "controller.h"

QT_BEGIN_NAMESPACE
class QStateMachine;
QT_END_NAMESPACE

class MainWindow;
class ServerConnection;
class FSHost;
class ProgressDialog;
class ProgressController;
class LoginDialog;
class User;

class ApplicationController : public Controller
{
	Q_OBJECT

public:
	 ApplicationController();
	 ~ApplicationController();

private:
	static ServerConnection *_server;
	static FSHost *_fs;
	static User *_user;
	ProgressController *_progress_controller;
	ProgressDialog *_progress_dialog;
	LoginDialog *_login_dialog;
	MainWindow *_main_window;

private:
	ProgressDialog *progressDialog();
	LoginDialog *loginDialog();
	MainWindow *mainWindow();

protected:
	virtual QStateMachine *createStateMachine();

private slots:
	void starting();
	void authenticating();
	void authenticated(User *user);

public:
	static ServerConnection *server();
	static FSHost *fs();
	static User *user();
	int run();

};

#endif // APPLICATION_CONTROLLER_H
