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

protected:
  void run();

signals:
  void authenticating();
  void authenticated(User *user);
  void authenticateError(QString reason);
  void pauseChanged(bool state);
  void forcedPause(QString reason);
  void paused();
  void pausing();
  void unpausing();
  void unpaused();

  void reservedForInteraction(QVariantMap);
  void invokeMessage(QString msg);
  void lostConnection();
  void interactionReconnected();
  void socketDisconnected();
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
  void sendAction(char *action);
  void write(QByteArray);
  void write(QString);
  void write(char *json);

private:
  bool _connected;
  QTcpSocket *_socket;
  QStateMachine *_machine;
  QString _host;
  int _port;
};

extern ServerConnection *server_connection;

#endif // TCPCLIENT_H
