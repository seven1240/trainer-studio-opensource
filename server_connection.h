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
	void changed();

private:
	QStateMachine *createStateMachine();
	void startTimer(int milliseconds);
	void sendAction(const char *action);
	void write(QByteArray);
	void write(QString);
	void write(const char *json);

private:
	bool _running;
	bool _connected;
	QTcpSocket *_socket;
	QStateMachine *_machine;
	QTimer *_timer;
	QString _host;
	int _port;
};

#endif // TCPCLIENT_H
