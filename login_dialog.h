/*
 * Eleutian Trainer Studio Application Opensource Edition, a VoIP client with Webkit and Flash support.
 * Copyright (C) 2011, Eleutian Technology, LLC <http://www.eleutian.com>
 *
 * Version: Apache License 2.0
 *
 * The content of this file is licensed under the Apache License, Version 2.0. (the "License").
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
 * Seven Du <seven@eleutian.com>
 * Portions created by the Initial Developer are Copyright (C)
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * Jadob Lewallen <jacob@eleutian.com>
 */
#ifndef LOGIN_DIALOG_H
#define LOGIN_DIALOG_H

#include <QDialog>
#include <QKeyEvent>
#include <QVariant>

QT_BEGIN_NAMESPACE
class QGroupBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QTextEdit;
class QFrame;
QT_END_NAMESPACE

class User;
class ProgressWidget;

class LoginDialog : public QDialog {
	Q_OBJECT

public:
	LoginDialog(QWidget *parent = 0);
	~LoginDialog();

protected:
	void changeEvent(QEvent *e);
	void closeEvent(QCloseEvent *e);
	void onKeyPressEvent(QKeyEvent *e);

private:
	ProgressWidget *_progress;
	QLineEdit *_username;
	QLineEdit *_password;
	QPushButton *_settings;
	QPushButton *_login;
	QFrame *_loginFrame;
	QFrame *_progressFrame;
	bool _authenticated;
	bool _abort;
	User *_user;

	void abortLogin();
	void abortLogin(QString msg);

private:
	void showProgress();
	void showLogin();

public slots:

private slots:
	void onServerConnectionConnected();
	void onServerConnectionAuthenticated(User *user);
	void onServerConnectionDisconnected();
	void onSettingsClicked();
	void onAuthenticateError(QString);
	void onSocketError(QString);
	void onCancelClicked();
	void onLoginClicked();
};

#endif // LOGIN_DIALOG_H
