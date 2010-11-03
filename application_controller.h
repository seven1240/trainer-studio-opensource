#ifndef APPLICATION_CONTROLLER_H
#define APPLICATION_CONTROLLER_H

#include "controller.h"

QT_BEGIN_NAMESPACE
class QStateMachine;
QT_END_NAMESPACE

class MainWindow;
class ServerConnection;
class FreeSwitch;
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
	static FreeSwitch *_fs;
	static User *_user;
	ProgressController *_progressController;
	ProgressDialog *_progressDialog;
	LoginDialog *_loginDialog;
	MainWindow *_mainWindow;

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
	static FreeSwitch *fs();
	static User *user();
	int run();

};

#endif // APPLICATION_CONTROLLER_H
