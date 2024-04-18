#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include "ChatProto.h"
#include <QObject>
#include <QTcpSocket>

class ClientManager : public QObject
{
    Q_OBJECT
public:
    explicit ClientManager(QHostAddress ip = QHostAddress::LocalHost, ushort port = 4500, QObject *parent = nullptr);
    explicit ClientManager(QTcpSocket *client, QObject *parent = nullptr);
    void connectToServer();
    void sendMessage(QString message);
    void sendName(QString name);
    QString name() const;
public:
    void sendIsTyping();
signals:
    void connected();
    void disconnected();
    //void dataReceived(QByteArray data); //более не требуется
    void textMessageReceived(QString message);
    void isTyping();
    void nameSet(QString name);

private slots:
    void readyRead();
private: //fields
    QTcpSocket *_socket;
    QHostAddress _ip;
    ushort _port;
    ChatProto _proto;
};

#endif // CLIENTMANAGER_H
