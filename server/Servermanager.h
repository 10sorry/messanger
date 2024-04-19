#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include "ChatProto.h"
#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>

class ServerManager : public QObject
{
    Q_OBJECT
public:
    explicit ServerManager(ushort port = 4500, QObject *parent = nullptr);
    void notifyOtherClients(QString previousName, QString name);

public slots:
    void onMessForClients(QString message, QString receiver, QString sender);
private slots:
    void newClientConnection();
    void onClientDisconnected();

signals:
    void newClientConnected(QTcpSocket *client);
    void clientDisconnected(QTcpSocket *client);

private: //fields
    QTcpServer *_server;
    QMap<QString, QTcpSocket *> _clients;
    ChatProto _proto;

private: //methods
    void setupServer(ushort port);
};

#endif // SERVERMANAGER_H
