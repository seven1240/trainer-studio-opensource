#include "TCPClient.h"
#include "parser.h"
#include "switch.h"


TCPClient *tcp_client;

TCPClient::TCPClient()
{
    ping = false;
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

        qDebug() << "TCPClient running ...";
        if(ping) {
            //tcpSocket->write("{\"action\":\"Ping\"}");
        }
        switch_sleep(10000000);
    }
}

void TCPClient::connectToHost(QString host, int port)
{
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
    }else{
        qDebug() << status;
        if(status == "Pong") {
            qDebug() << "Got Pong";
        }else if(status == "Authenticated") {
            emit(authenticated(result));
            qDebug() << "blahh..... Authed";
            ping = true;
        } else if(status == "AuthenticateError"){
            emit(authenticateError(result["reason"].toString()));
        } else if(status== "Paused"){
            emit(paused(true));
        } else if(status == "Unpaused"){
            emit(paused(false));
        } else if(status == "ForcedPause"){
            emit(forcedPause(result["reason"].toString()));
        } else if(status == "ReservedForInteraction"){
            emit(reservedForInteraction(result));
            qDebug() << "ReservedForInteraction....";
        } else {
            qDebug() << "Unknown JSON";
        }
qDebug() << "en??";
    }
}

void TCPClient::onSocketError(QAbstractSocket::SocketError)
{
    qDebug() << "Errorrrrrrrrrrrrr";

}

void TCPClient::onConnected()
{
    connected = true;
    qDebug() << "Socket Connected";

}

void TCPClient::onDisconnected()
{
    ping = false;
    connected = false;
    qDebug() << "Disconnected, reconnecting in 10 seconds...";
//    sleep(1);
//    ConnectToHost();
}

void TCPClient::write(QByteArray ba)
{
    _tcpSocket->write(ba.append("\n"));
}

bool TCPClient::isConnected()
{
    return connected;
}

void TCPClient::pause(bool action)
{
	qDebug() << "Pause: " << action;
    if(action){
        _tcpSocket->write("{\"action\":\"Pause\"}");
    }else{
        _tcpSocket->write("{\"action\":\"Unpause\"}");
    }
}
