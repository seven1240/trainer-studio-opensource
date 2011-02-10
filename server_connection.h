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
#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QThread>
#include <QTCPSocket>
#include "trainer_studio.h"
#include "user.h"

QT_BEGIN_NAMESPACE
class QStateMachine;
class QTcpSocket;
QT_END_NAMESPACE

class ServerConnection : public QThread
{
	Q_OBJECT
public:
	ServerConnection();
	void open() { open(_host, _port); }
	void open(QString host, int port);
	void startInteractionReconnection(QString interactionId);
	void close();
	bool isConnected();
	void pause(bool action);
	void review();
	void login(QString username, QString password);
	void logout();
	void shutdown();

protected:
	void run();

signals:
	void connected();
	void connecting();
	void timeout();
	void disconnected();
	void authenticating();
	void authenticated(User *user);
	void authenticateError(QString reason);
	void pauseChanged(bool state);
	void forcedPause(QString reason);
	void paused();
	void pausing();
	void unpausing();
	void unpaused();
	void working();

	void reservedForInteraction(QVariantMap);
	void invokeMessage(QString msg);
	void lostConnection();
	void interactionReconnected();
	void socketError(QString);

private slots:
	void onReadyRead();
	void onSocketError(QAbstractSocket::SocketError);
	void onConnected();
	void onDisconnected();
	void onTimer();
	void onPing();
	void onTimeout();

private:
	QStateMachine *createStateMachine();
	void startTimer(int milliseconds);
	void sendAction(const char *action);
	void write(QByteArray);
	void write(QString);
	void write(const char *json);

	bool _running;
	bool _connected;
	QTcpSocket *_socket;
	QStateMachine *_machine;
	QTimer *_timer;
	QTimer *_pingTimer;
	QString _host;
	int _port;
	QString _password;
};

#endif // TCPCLIENT_H
