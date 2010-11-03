#include <QTCPSocket>
#include <QStateMachine>
#include <QTimer>
#include <switch.h>
#include "server_connection.h"
#include "qJSON.h"
#include "utils.h"

ServerConnection::ServerConnection()
{
	_connected = false;
	_socket = new QTcpSocket(this);
	_timer = new QTimer(this);
	_machine = createStateMachine();

	setObjectName("ServerConnection");

	connect(_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
	connect(_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketError(QAbstractSocket::SocketError)));
}

static QState *createState(ServerConnection *connection, QString name)
{
	QState *state = new QState();
	connection->connect(state, SIGNAL(entered()), connection, SLOT(changed()));
	state->setObjectName(name);
	return state;
}

QStateMachine *ServerConnection::createStateMachine()
{
	QState *disconnected = createState(this, "disconnected");
	connect(disconnected, SIGNAL(entered()), this, SLOT(onDisconnected()));
	QState *connecting = createState(this, "connecting");
	QState *connected = createState(this, "connected");
	connect(connected, SIGNAL(entered()), this, SLOT(onConnected()));
	QState *authenticating = createState(this, "authenticating");
	QState *authenticated = createState(this, "authenticated");
	QState *pausing = createState(this, "pausing");
	QState *unpausing = createState(this, "unpausing");
	QState *working = createState(this, "working");
	QState *paused = createState(this, "paused");
	QState *timeout = createState(this, "timeout");

	disconnected->addTransition(this, SIGNAL(connecting()), connecting);
	connecting->addTransition(_socket, SIGNAL(connected()), connected);
	connecting->addTransition(_timer, SIGNAL(timeout()), timeout);
	connected->addTransition(_socket, SIGNAL(disconnected()), disconnected);
	connected->addTransition(this, SIGNAL(authenticating()), authenticating);
	authenticating->addTransition(this, SIGNAL(authenticated(User*)), authenticated);
	authenticating->addTransition(this, SIGNAL(authenticateError(QString)), disconnected);
	authenticating->addTransition(_timer, SIGNAL(timeout()), timeout);
	authenticated->addTransition(_socket, SIGNAL(disconnected()), disconnected);

	authenticated->addTransition(this, SIGNAL(pausing()), pausing);
	pausing->addTransition(this, SIGNAL(paused()), paused);
	paused->addTransition(this, SIGNAL(unpausing()), unpausing);
	unpausing->addTransition(this, SIGNAL(working()), unpausing);

	working->addTransition(_socket, SIGNAL(disconnected()), disconnected);
	pausing->addTransition(_socket, SIGNAL(disconnected()), disconnected);
	paused->addTransition(_socket, SIGNAL(disconnected()), disconnected);
	unpausing->addTransition(_socket, SIGNAL(disconnected()), disconnected);

	QStateMachine *machine = new QStateMachine();
	machine->addState(disconnected);
	machine->addState(connecting);
	machine->addState(timeout);
	machine->addState(connected);
	machine->addState(authenticating);
	machine->addState(authenticated);
	machine->addState(paused);
	machine->addState(unpausing);
	machine->addState(pausing);
	machine->addState(working);

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

void ServerConnection::run()
{
	_running = true;
	int32_t counter = 0;
	while (_running) {
		if (counter++ == 5) {
			qDebug() << "ServerConnection:" << _socket->state() << _connected;
			counter = 0;
		}
		msleep(2000);
	}
}

void ServerConnection::shutdown()
{
	_running = false;
}

void ServerConnection::open(QString host, int port)
{
	if (_connected)
		return;

	startTimer(5000);
	emit connecting();

	_host = host;
	_port = port;
	_socket->connectToHost(host, port);
}

void ServerConnection::startTimer(int milliseconds)
{
	_timer->setInterval(milliseconds);
	_timer->start();
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

	startTimer(5000);
	emit authenticating();

	write(json);

	free(json);
}

void ServerConnection::logout()
{
	close();
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

	QString s(ba.trimmed());
	if (s.length() == 0) {
		return;
	}

	qJSON *qjson = new qJSON();
	bool ok;
	qjson->parse(ba.data(), &ok);

	if (!ok) {
		qDebug() << "Invalid JSON:" << ba;
		delete qjson;
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
	delete qjson;
}

void ServerConnection::onSocketError(QAbstractSocket::SocketError)
{
	_connected = false;
	emit socketError(_socket->errorString());
}

void ServerConnection::onConnected()
{
	_connected = true;
	emit connected();
}

void ServerConnection::onDisconnected()
{
	_connected = false;
	emit disconnected();
}

void ServerConnection::onTimer()
{
}

void ServerConnection::review()
{
	sendAction("Review");
}

void ServerConnection::sendAction(const char *action)
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

void ServerConnection::write(const char *s)
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
	qDebug() << "Sending Pause: " << action;
	if (action) {
		sendAction("Pause");
		emit pausing();
	}
	else {
		sendAction("Unpause");
		emit unpausing();
	}
}
