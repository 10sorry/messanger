#include "Servermanager.h"
#include "qtcpsocket.h"

ServerManager::ServerManager(ushort port, QObject *parent)
    : QObject{parent}
{
    setupServer(port);
}
void ServerManager::newClientConnection()
{
    auto client = _server->nextPendingConnection();
    auto id = _clients.count();
    auto clientName = QString("Client (%1)").arg(id);
    client->setProperty("id", id);
    client->setProperty("clientName", clientName);
    connect(client, &QTcpSocket::disconnected, this, &ServerManager::onClientDisconnected);
    emit newClientConnected(client);
    if(id > 1)
    {
        auto message = _proto.setConnectionMessage(clientName, _clients.keys());
        client->write(message);
        auto newClientMessage = _proto.setNewClientMessage(clientName);
        foreach(auto client, _clients)
        {
            client->write(newClientMessage);
        }
    }
    _clients[clientName] = client;
}
void ServerManager::onClientDisconnected()
{
    auto client = qobject_cast<QTcpSocket *>(sender());
    auto clientName = client->property("clientName").toString();
    _clients.remove(clientName);
    auto message = _proto.setClientDisconnectedMessage(clientName);
    foreach(auto client, _clients)
    {
        client->write(message);
    }
    emit clientDisconnected(client);
}

void ServerManager::notifyOtherClients(QString previousName, QString name)
{
    auto message = _proto.setClientNameMessage(previousName, name);
    foreach (auto client, _clients) {
        auto clientName = client->property("clientName").toString();
        if (clientName != name) {
            client->write(message);
        }
    }
}

void ServerManager::onMessForClients(QString message, QString receiver, QString sender)
{
    auto msg = _proto.textMessage(message, receiver);
    if (receiver == "All")
    {
        foreach (auto client, _clients) {
            auto clientName = client->property("clientName").toString();
            if (clientName != sender) {
                client->write(msg);
            }
        }
    }
    else
    {
        foreach (auto client, _clients) {
            auto clientName = client->property("clientName").toString();
            if (clientName == receiver) {
                client->write(msg);
                return;
            }
        }
    }
}

void ServerManager::setupServer(ushort port)
{
    _server = new QTcpServer(this);
    connect(_server, &QTcpServer::newConnection, this, &ServerManager::newClientConnection);
    _server->listen(QHostAddress::Any, port);
}
