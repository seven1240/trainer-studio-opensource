#include <switch.h>
#include "server_connection.h"
#include "qJSON.h"
#include "utils.h"

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
  for (;;) {
    qDebug() << "ServerConnection running: " << _socket->state() << _connected;
    switch_sleep(10000000);
  }
}

void ServerConnection::open(QString host, int port)
{
  if (_connected) return;
  _host = host;
  _port = port;
  _socket->connectToHost(host, port);
}

void ServerConnection::startInteractionReconnection(QString interactionId)
{
  write(QString("{\"action\": \"Reconnect\"," "\"interaction_id\": \"%1\"}").arg(interactionId));
}

void ServerConnection::login(QString username, QString password)
{
  QVariantMap map = Utils::getSystemInfos();

  char *json = NULL;

  cJSON *info = cJSON_CreateArray();

  cJSON *item;
  item = cJSON_CreateObject();
  cJSON_AddItemToObject(item, "os", cJSON_CreateString(map["os"].toString().toAscii().data()));
  cJSON_AddItemToObject(item, "memory", cJSON_CreateString(map["memory"].toString().toAscii().data()));
  cJSON_AddItemToObject(item, "screen_res", cJSON_CreateString(map["screen_res"].toString().toAscii().data()));
  cJSON_AddItemToObject(item, "flash_player_version", cJSON_CreateString(map["flash_player_version"].toString().toAscii().data()));
  cJSON_AddItemToArray(info, item);

  cJSON *cj = cJSON_CreateObject();

  cJSON_AddItemToObject(cj, "username", cJSON_CreateString(username.toAscii().data()));
  cJSON_AddItemToObject(cj, "password", cJSON_CreateString(password.toAscii().data()));
  cJSON_AddItemToObject(cj, "action", cJSON_CreateString("Authenticate"));
  cJSON_AddItemToObject(cj, "system_info", info);

  json = cJSON_Print(cj);
  cJSON_Delete(cj);

  qDebug() << json;

  write(json);

  // TODO: free json?
}

void ServerConnection::close()
{
  _socket->close();
}

void ServerConnection::onReadyRead()
{
  QByteArray ba;
  while (_socket->canReadLine())
  {
    ba = _socket->readAll();
  }

  QString s(ba);
  qDebug() << ba.data();

  qJSON *qjson = new qJSON();
  bool ok;
  QVariantMap result;
  qjson->parse(ba.data(), &ok);

  if (!ok) {
    qDebug() << "Invalid JSON! " << ba;
    return;
  }
  result = qjson->toMap();

  qDebug() << result;
  QString status = result["status"].toString();
  qDebug() << status;

  if (status == "Pong") {
    qDebug() << "Got Pong";
  }
  else if (status == "Authenticated") {
    qDebug() << "blahh..... Authed";
    emit authenticated(result);
  }
  else if (status == "AuthenticateError") {
    emit authenticateError(result["reason"].toString());
  }
  else if (status== "Paused") {
    emit paused(true);
  }
  else if (status == "Unpaused") {
    emit paused(false);
  }
  else if (status == "ForcedPause") {
    emit forcedPause(result["reason"].toString());
  }
  else if (status == "ReservedForInteraction") {
    emit reservedForInteraction(result);
    qDebug() << "ReservedForInteraction....";
  }
  else if (status == "Unregistered") {

  }
  else if (status == "Message") {
    emit invokeMessage(result["message"].toString());
  }
  else {
    QString action = result["action"].toString();

    if (action == "LostConnection") {
      emit lostConnection();
    }
    else if (action == "Reconnected") {
      qDebug() << "hhh: " << action;
      emit interactionReconnected();
    }
    else {
      qDebug() << "Unknown JSON";
    }
  }
}

void ServerConnection::onSocketError(QAbstractSocket::SocketError)
{
  _connected = false;
  emit socketError(_socket->errorString());
  qDebug() << "Socket Error: " << _socket->error() << " " << _socket->errorString();
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
  emit socketDisconnected();
  qDebug() << "Disconnected, reconnecting in 10 seconds...";
}

void ServerConnection::onTimer()
{
  qDebug() << "Reconnect";
  open();
}

void ServerConnection::review()
{
  sendAction("Review");
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
}

void ServerConnection::pause(bool action)
{
  qDebug() << "Pause: " << action;
  if (action) {
    sendAction("Pause");
  }
  else {
    sendAction("Unpause");
  }
}
