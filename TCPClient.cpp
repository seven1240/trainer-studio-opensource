#include "TCPClient.h"
#include "parser.h"


TCPClient *tcp_client;

TCPClient::TCPClient()
{
    ping = false;
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketError(QAbstractSocket::SocketError)));
    connect(tcpSocket, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));

}


void TCPClient::run()
{


//        tcpSocket->connectToHost("localhost", 3000);


//    ConnectToHost();
    for (;;) {

        qDebug() << "TCPClient running ...";
        if(ping) {
            tcpSocket->write("{\"action\":\"Ping\"}");
        }
        sleep(10);
    }
}

void TCPClient::connectToHost()
{
    tcpSocket->connectToHost("voip.idapted.com", 7000);
}

void TCPClient::close()
{
    tcpSocket->close();
}

void TCPClient::onReadyRead()
{
    QByteArray ba;
    while( tcpSocket->canReadLine() )
    {
        // read and process the line
        ba = tcpSocket->readAll();

    }

    QString s(ba);
    qDebug() << s;

    QJson::Parser parser;
    bool ok;
    QVariantMap result;

    result = parser.parse (ba, &ok).toMap();
    qDebug() << result;
    if(!ok) {
        qDebug() << "Invalid JSON! " << ba;
    }else{
        if(result["status"] == "Pong") {
            qDebug() << "Got Pong";
        }else if(result["status"] == "Authenticated") {
            emit(authenticated(result));
            ping = true;
        } else if(result["status"] == "AuthenticateError"){
            emit(authenticateError(result["reason"].toString()));
        } else if(result["status"] == "Paused"){
            emit(paused(true));
        } else if(result["status"] == "Unpaused"){
            emit(paused(false));
        } else if(result["status"] == "ForcedPause"){
            emit(forcedPause(result["reason"].toString()));
        } else if(result["status"] == "ReservedForInteraction"){
            emit(reservedForInteraction(result));
            qDebug() << "ReservedForInteraction....";
        } else {
            qDebug() << "Unknown JSON";
        }

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
    tcpSocket->write(ba.append("\n"));
}

bool TCPClient::isConnected()
{
    return connected;
}

void TCPClient::pause(bool action)
{
    if(action){
        tcpSocket->write("{\"action\":\"Pause\"}");
    }else{
        tcpSocket->write("{\"action\":\"Unpause\"}");
    }
}
