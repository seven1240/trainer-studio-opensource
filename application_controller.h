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
class ProgressWidget;
class FlashController;
class FlashDialog;
class EchoTestDialog;
class IncomingCallDialog;
class LoginDialog;
class CallDialog;
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
	ProgressWidget *_progressWidget;
	ProgressController *_progressController;
	FlashController *_flashController;
	ProgressDialog *_progressDialog;
	LoginDialog *_loginDialog;
	EchoTestDialog *_echoTestDialog;
	FlashDialog *_flashDialog;
	IncomingCallDialog *_incomingCallDialog;
	CallDialog *_callDialog;
	MainWindow *_mainWindow;

private:
	ProgressDialog *progressDialog();
	LoginDialog *loginDialog();
	EchoTestDialog *echoTestDialog();
	FlashDialog *flashDialog();
	IncomingCallDialog *incomingCallDialog();
	MainWindow *mainWindow();
	CallDialog *callDialog();

protected:
	virtual QStateMachine *createStateMachine();

private slots:
	void testEcho();
	void testFlash();
	void starting();
	void calling();
	void authenticating();
	void authenticated(User *user);
	void ready();
	void training();
	void incoming();

public:
	static ServerConnection *server();
	static FreeSwitch *fs();
	static User *user();
	static bool isDebugging();
	int run();

};

#endif // APPLICATION_CONTROLLER_H
