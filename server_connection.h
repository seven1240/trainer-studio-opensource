#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QThread>
#include <QTCPSocket>
#include "trainer_studio.h"
#include "user.h"

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
  void authenticated(User *user);
  void authenticateError(QString reason);
  void paused(bool state);
  void forcedPause(QString reason);
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

private:
  void sendAction(char *action);
  void write(QByteArray);
  void write(QString);
  void write(char *json);

private:
  bool _ping;
  bool _connected;
  QTcpSocket *_socket;
  QString _host;
  int _port;
};

extern ServerConnection *server_connection;

#endif // TCPCLIENT_H
