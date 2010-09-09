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
    void connectToHost() { connectToHost(_host, _port); }
    void connectToHost(QString host, int port);
    void close();
    void sendAction(char *);
    void write(QByteArray);
    void write(QString);
    void write(char *);
    bool isConnected();
    void pause(bool action);

protected:
    void run();

signals:
    void authenticated(QVariantMap);
    void authenticateError(QString reason);
    void paused(bool state);
    void forcedPause(QString reason);
    void reservedForInteraction(QVariantMap);
    void invokeMessage(QString msg);
    void lostConnection();
    void interactionReconnected();

private slots:
    void onReadyRead();
    void onSocketError(QAbstractSocket::SocketError);
    void onConnected();
    void onDisconnected();


private:
    bool ping;
    bool connected;
    QTcpSocket *_tcpSocket;
    QString _host;
    int _port;
};


extern TCPClient *tcp_client;

#endif // TCPCLIENT_H
