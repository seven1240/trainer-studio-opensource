/*
 * Eleutian Trainer Studio Application Opensource Edition, a VoIP client with Webkit and Flash support.
 * Copyright (C) 2011, Eleutian Technology, LLC <http://www.eleutian.com>
 *
 * Version: Apache License 2.0
 *
 * The content of this file is licensed under the Apache License, Version 2.0.  (the "License").
 * You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.apache.org/licenses/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is Eleutian Trainer Studio Application Opensource Edition.
 *
 * The Initial Developer of the Original Code is
 * Jadob Lewallen <jacob@eleutian.com>
 * Portions created by the Initial Developer are Copyright (C)
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * Seven Du <seven@eleutian.com>
 */

#ifndef APPLICATION_CONTROLLER_H
#define APPLICATION_CONTROLLER_H

#include <QTimer>
#include "controller.h"

QT_BEGIN_NAMESPACE
class QStateMachine;
QT_END_NAMESPACE

class MainWindow;
class ServerConnection;
class FreeSwitch;
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
	FlashController *_flashController;
	LoginDialog *_loginDialog;
	EchoTestDialog *_echoTestDialog;
	FlashDialog *_flashDialog;
	IncomingCallDialog *_incomingCallDialog;
	CallDialog *_callDialog;
	MainWindow *_mainWindow;

private:
	LoginDialog *loginDialog();
	EchoTestDialog *echoTestDialog();
	FlashDialog *flashDialog();
	IncomingCallDialog *incomingCallDialog();
	MainWindow *mainWindow();
	CallDialog *callDialog();
	QTimer *_wrapupTimer;

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
	void wrapup();
	void wrapupTimeout();
	void incoming();

public:
	static ServerConnection *server();
	static FreeSwitch *fs();
	static User *user();
	static bool isDebugging();
	int run();

};

#endif // APPLICATION_CONTROLLER_H
