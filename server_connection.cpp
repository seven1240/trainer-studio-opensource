#include <QTCPSocket>
#include <QStateMachine>
#include <switch.h>
#include "server_connection.h"
#include "qJSON.h"
#include "utils.h"

ServerConnection *server_connection;

QStateMachine *ServerConnection::createStateMachine()
{
  QState *disconnected = new QState();
  connect(disconnected, SIGNAL(entered()), this, SLOT(changed()));
  disconnected->setObjectName("disconnected");
  // off->assignProperty(&button, "text", "Off");

  QState *connected = new QState();
  connect(connected, SIGNAL(entered()), this, SLOT(changed()));
  connected->setObjectName("connected");

  QState *authenticating = new QState();
  connect(authenticating, SIGNAL(entered()), this, SLOT(changed()));
  authenticating->setObjectName("authenticating");

  QState *authenticated = new QState();
  connect(authenticated, SIGNAL(entered()), this, SLOT(changed()));
  authenticated->setObjectName("authenticated");

  QState *pausing = new QState();
  connect(pausing, SIGNAL(entered()), this, SLOT(changed()));
  pausing->setObjectName("pausing");

  QState *unpausing = new QState();
  connect(unpausing, SIGNAL(entered()), this, SLOT(changed()));
  unpausing->setObjectName("unpausing");

  QState *working = new QState();
  connect(working, SIGNAL(entered()), this, SLOT(changed()));
  working->setObjectName("working");

  QState *paused = new QState();
  connect(paused, SIGNAL(entered()), this, SLOT(changed()));
  paused->setObjectName("paused");

  disconnected->addTransition(_socket, SIGNAL(connected()), connected);
  connected->addTransition(_socket, SIGNAL(disconnected()), disconnected);
  connected->addTransition(this, SIGNAL(authenticating()), authenticating);
  authenticated->addTransition(_socket, SIGNAL(disconnected()), disconnected);
  authenticating->addTransition(this, SIGNAL(authenticated(User*)), authenticated);
  authenticating->addTransition(this, SIGNAL(authenticateError(QString)), disconnected);

  authenticated->addTransition(this, SIGNAL(pausing()), pausing);
  pausing->addTransition(this, SIGNAL(paused()), paused);
  paused->addTransition(this, SIGNAL(unpausing()), unpausing);
  unpausing->addTransition(this, SIGNAL(working()), unpausing);

  QStateMachine *machine = new QStateMachine();
  machine->addState(disconnected);
  machine->addState(connected);
  machine->addState(authenticating);
  machine->addState(authenticated);

  machine->setInitialState(disconnected);
  machine->start();

  return machine;
}

void ServerConnection::changed()
{
  QSetIterator<QAbstractState*> i(_machine->configuration());
  while (i.hasNext())
  {
    qDebug() << "ServerConnection:" << i.next()->objectName();
  }
}

ServerConnection::ServerConnection()
{
  _connected = false;
  _socket = new QTcpSocket(this);
  _machine = createStateMachine();

  connect(_socket, SIGNAL(connected()), this, SLOT(onConnected()));
  connect(_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
  connect(_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketError(QAbstractSocket::SocketError)));
  connect(_socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
}

void ServerConnection::run()
{
  for (;;) {
    qDebug() << "ServerConnection:" << _socket->state() << _connected;
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

  emit authenticating();

  write(json);

  free(json);
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

  qJSON *qjson = new qJSON();
  bool ok;
  qjson->parse(ba.data(), &ok);

  if (!ok) {
    qDebug() << "Invalid JSON! " << ba;
    return;
  }
  QVariantMap data = qjson->toMap();
  QString status = data["status"].toString();

  qDebug() << ba.data();
  #if _EXTRA_VERBOSE
  qDebug() << data;
  qDebug() << status;
  #endif

  if (status == "Pong") {
  }
  else if (status == "Authenticated") {
    emit authenticated(new User(data));
  }
  else if (status == "AuthenticateError") {
    emit authenticateError(data["reason"].toString());
  }
  else if (status == "Paused") {
    emit pauseChanged(true);
    emit paused();
  }
  else if (status == "Unpaused") {
    emit pauseChanged(false);
    emit unpaused();
  }
  else if (status == "ForcedPause") {
    emit forcedPause(data["reason"].toString());
    emit paused();
  }
  else if (status == "ReservedForInteraction") {
    emit reservedForInteraction(data);
  }
  else if (status == "Unregistered") {
  }
  else if (status == "Message") {
    emit invokeMessage(data["message"].toString());
  }
  else {
    QString action = data["action"].toString();

    if (action == "LostConnection") {
      emit lostConnection();
    }
    else if (action == "Reconnected") {
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
}

void ServerConnection::onDisconnected()
{
  _connected = false;
  emit socketDisconnected();
}

void ServerConnection::onTimer()
{
  open();
}

void ServerConnection::review()
{
  sendAction("Review");
}

void ServerConnection::sendAction(char *action)
{
  write(QString("{\"action\": \"%1\"}").arg(action));
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
    emit pausing();
  }
  else {
    sendAction("Unpause");
    emit unpausing();
  }
}
