#include "ClientManager.h"

ClientManager::ClientManager(QHostAddress ip, ushort port, QObject *parent)
    : QObject{parent},
    _ip(ip),
    _port(port)
{
    _socket = new QTcpSocket(this);
    connect(_socket, &QTcpSocket::connected, this, &ClientManager::connected);
    connect(_socket, &QTcpSocket::disconnected, this, &ClientManager::disconnected);
    connect(_socket, &QTcpSocket::readyRead, this, &ClientManager::readyRead);
}

void ClientManager::connectToServer()
{
    _socket->connectToHost(_ip, _port);
    qDebug() << "Connecting to server: " << _ip.toString() << ":" << _port;
}

void ClientManager::sendMessage(QString message)
{
    _socket->write(_proto.textMessage(message));
}

void ClientManager::sendName(QString name)
{
    _socket->write(_proto.setNameMessage(name));
}

void ClientManager::sendIsTyping()
{
    _socket->write(_proto.isTypingMessage());
}

void ClientManager::readyRead()
{
    auto data = _socket->readAll(); //в дальнейшем пробрасываем в протобаф
    //emit dataReceived(data);
    _proto.loadData(data);
    switch(_proto.messType()) {
    case ChatProto::Text:
        emit textMessageReceived(_proto.message());
        break;
    case ChatProto::IsTyping:
        emit isTyping();
    case ChatProto::Name:
        emit nameSet(_proto.name());
        break;
    default:
        break;
    }
}
