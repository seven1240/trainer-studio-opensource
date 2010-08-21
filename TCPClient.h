#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QTNetwork>
#include <QThread>
#include <qtcpsocket.h>
#include "trainer_studio.h"
#include "serializer.h"


class TCPClient : public QThread
{
    Q_OBJECT
public:
    TCPClient();
    void connectToHost();
    void close();
    void write(QByteArray);
    bool isConnected();

protected:
    void run();

signals:
    void authenticated(QVariantMap);
    void authenticateError(QString reason);
private slots:
    void onReadyRead();
    void onSocketError(QAbstractSocket::SocketError);
    void onConnected();
    void onDisconnected();


private:
    bool ping;
    bool connected;
    QTcpSocket *tcpSocket;
};


extern TCPClient *tcp_client;

#endif // TCPCLIENT_H
