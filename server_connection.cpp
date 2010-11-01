#include <switch.h>
#include "server_connection.h"
#include "qJSON.h"

ServerConnection *server_connection;

ServerConnection::ServerConnection()
{
  _ping = false;
  _connected = false;
  _socket = new QTcpSocket(this);
  connect(_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
  connect(_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketError(QAbstractSocket::SocketError)));
  connect(_socket, SIGNAL(connected()), this, SLOT(onConnected()));
  connect(_socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
}

void ServerConnection::run()
{
  //        tcpSocket->connectToHost("localhost", 3000);
  //    ConnectToHost();
  for (;;) {

    qDebug() << "ServerConnection running: " << _socket->state() << _connected;
    if(_connected && _ping) {
      //tcpSocket->write("{\"action\":\"Ping\"}");
    }
    switch_sleep(10000000);
  }
}

void ServerConnection::connectToHost(QString host, int port)
{
  if (_connected) return;
  _host = host;
  _port = port;
  _socket->connectToHost(host, port);
}

void ServerConnection::close()
{
  _socket->close();
}

void ServerConnection::onReadyRead()
{
  QByteArray ba;
  while( _socket->canReadLine() )
  {
    // read and process the line
    ba = _socket->readAll();

  }

  QString s(ba);
  qDebug() << ba.data();

  qJSON *qjson = new qJSON();
  bool ok;
  QVariantMap result;
  qjson->parse(ba.data(), &ok);

  if(!ok) {
    qDebug() << "Invalid JSON! " << ba;
    return;
  }
  result = qjson->toMap();

  qDebug() << result;
  QString status = result["status"].toString();
  qDebug() << status;

  if(status == "Pong") {
    qDebug() << "Got Pong";
  }else if(status == "Authenticated") {
    qDebug() << "blahh..... Authed";
    emit authenticated(result);
    //            ping = true;
  } else if (status == "AuthenticateError"){
    emit authenticateError(result["reason"].toString());
  } else if (status== "Paused"){
    emit paused(true);
  } else if (status == "Unpaused"){
    emit paused(false);
  } else if (status == "ForcedPause"){
    emit forcedPause(result["reason"].toString());
  } else if (status == "ReservedForInteraction"){
    emit reservedForInteraction(result);
    qDebug() << "ReservedForInteraction....";
  } else if (status == "Unregistered") {

  } else if (status == "Message") {
    emit invokeMessage(result["message"].toString());
  } else {
    QString action = result["action"].toString();

    if (action == "LostConnection") {
      emit lostConnection();
    } else if (action == "Reconnected") {
      qDebug() << "hhh: " << action;
      emit interactionReconnected();
    } else {
      qDebug() << "Unknown JSON";
    }
  }
}

void ServerConnection::onSocketError(QAbstractSocket::SocketError)
{
  _connected = false;
  // Let others know
  emit socketError(_socket->errorString());
  qDebug() << "Socket Error: " << _socket->error() << " "
   << _socket->errorString();
}

void ServerConnection::onConnected()
{
  _connected = true;
  qDebug() << "Socket Connected";

}

void ServerConnection::onDisconnected()
{
  _ping = false;
  _connected = false;
  // Let others know
  emit
   qDebug() << "Disconnected, reconnecting in 10 seconds...";
  //  Don't auto reconnect for now
  //    QTimer::singleShot(10000, this, SLOT(onTimer()));
}

void ServerConnection::onTimer()
{
  qDebug() << "Reconnect";
  connectToHost();
}

void ServerConnection::sendAction(char *action)
{
  write(QString("{\"action\": \"%1\"}").arg(action));
  qDebug() << QString("{\"action\": \"%1\"}").arg(action);
}

void ServerConnection::write(QByteArray ba)
{
  _socket->write(ba.append("\n"));
}

void ServerConnection::write(QString s)
{
  write(s.toAscii());
}

void ServerConnection::write(char *s)
{
  QByteArray ba(s);
  write(ba);
}

bool ServerConnection::isConnected()
{
  return _connected;
  //    return _socket->isValid();
}

void ServerConnection::pause(bool action)
{
  qDebug() << "Pause: " << action;
  if (action){
    sendAction("Pause");
  }
  else {
    sendAction("Unpause");
  }
}
