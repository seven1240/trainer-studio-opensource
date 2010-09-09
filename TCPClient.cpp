#include "TCPClient.h"
#include "parser.h"
#include "switch.h"


TCPClient *tcp_client;

TCPClient::TCPClient()
{
    _ping = false;
    _tcpSocket = new QTcpSocket(this);
    connect(_tcpSocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(_tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketError(QAbstractSocket::SocketError)));
    connect(_tcpSocket, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(_tcpSocket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));

}


void TCPClient::run()
{


//        tcpSocket->connectToHost("localhost", 3000);
//    ConnectToHost();
    for (;;) {

        qDebug() << "TCPClient running: " << _tcpSocket->state();
        if(_connected && _ping) {
            //tcpSocket->write("{\"action\":\"Ping\"}");
        }
        switch_sleep(10000000);
    }
}

void TCPClient::connectToHost(QString host, int port)
{
    if (_connected) return;
    _host = host;
    _port = port;
    _tcpSocket->connectToHost(host, port);
}

void TCPClient::close()
{
    _tcpSocket->close();
}

void TCPClient::onReadyRead()
{
    QByteArray ba;
    while( _tcpSocket->canReadLine() )
    {
        // read and process the line
        ba = _tcpSocket->readAll();

    }

    QString s(ba);
    qDebug() << s;

    QJson::Parser parser;
    bool ok;
    QVariantMap result;
    result = parser.parse (ba, &ok).toMap();
    qDebug() << result;
    QString status = result["status"].toString();

    qDebug() << status << "---- ---- ";

    if(!ok) {
        qDebug() << "Invalid JSON! " << ba;
        return;
    }

    qDebug() << status;
    if(status == "Pong") {
        qDebug() << "Got Pong";
    }else if(status == "Authenticated") {
        qDebug() << "blahh..... Authed";
        emit(authenticated(result));
        //            ping = true;
    } else if (status == "AuthenticateError"){
        emit (authenticateError(result["reason"].toString()));
    } else if (status== "Paused"){
        emit(paused(true));
    } else if (status == "Unpaused"){
        emit(paused(false));
    } else if (status == "ForcedPause"){
        emit(forcedPause(result["reason"].toString()));
    } else if (status == "ReservedForInteraction"){
        emit(reservedForInteraction(result));
        qDebug() << "ReservedForInteraction....";
    } else if (status == "Unregistered") {

    } else if (status == "Message") {
        emit(invokeMessage(result["message"].toString()));
    } else {
        QString action = result["action"].toString();

        if (action == "LostConnection") {
            emit(lostConnection());
        } else if (action == "Reconnected") {
            emit(interactionReconnected());
        } else {
            qDebug() << "Unknown JSON";
        }
    }
}

void TCPClient::onSocketError(QAbstractSocket::SocketError)
{
    _connected = false;
    qDebug() << "Socket Error: " << _tcpSocket->error() << " "
            << _tcpSocket->errorString();
}

void TCPClient::onConnected()
{
    _connected = true;
    qDebug() << "Socket Connected";

}

void TCPClient::onDisconnected()
{
    _ping = false;
    _connected = false;
    qDebug() << "Disconnected, reconnecting in 10 seconds...";

    QTimer::singleShot(10000, this, SLOT(onTimer()));
}

void TCPClient::onTimer()
{
    qDebug() << "Reconnect";
    connectToHost();
}

void TCPClient::sendAction(char *action)
{
    write(QString("{\"action\": \"%1\"}").arg(action));
    qDebug() << QString("{\"action\": \"%1\"}").arg(action);
}

void TCPClient::write(QByteArray ba)
{
    _tcpSocket->write(ba.append("\n"));
}

void TCPClient::write(QString s)
{
    write(s.toAscii());
}

void TCPClient::write(char *s)
{
    QByteArray ba(s);
    write(ba);
}

bool TCPClient::isConnected()
{
    return _connected;
//    return _tcpSocket->isValid();
}

void TCPClient::pause(bool action)
{
	qDebug() << "Pause: " << action;
    if(action){
        sendAction("Pause");
    }else{
        sendAction("Unpause");
    }
}
