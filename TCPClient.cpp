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
//    qDebug() << obj->metaObject()->className()
}

void TCPClient::ConnectToHost()
{
    tcpSocket->connectToHost("voip.idapted.com", 7000);
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
}

void TCPClient::onSocketError(QAbstractSocket::SocketError)
{
    qDebug() << "Errorrrrrrrrrrrrr";

}

void TCPClient::onConnected()
{
    connected = true;
    qDebug() << "Socket Connected";
//    QJson::Parser parser;
//    bool ok;
//    QVariant result;
//
//    qDebug() << "Connnected";
////    char *s = "{'action':'Authenticate','username':'jpalley', 'password':'veecue', 'system_info':[{'memory':'1024'}]}\n";
//    char *s="{\"password\":\"veecue\",\"username\":\"jpalley\",\"system_info\":[{\"memory\":\"100\"}],\"action\":\"Authenticate\"}\n";
//    qDebug() << s;
//    result = parser.parse (s, &ok);
//    qDebug() << result;
//    tcpSocket->write(s);
}

void TCPClient::onDisconnected()
{
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
