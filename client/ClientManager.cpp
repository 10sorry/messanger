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


void ClientManager::sendInitSendingFile(QString fileName)
{
    _tmpFileName = fileName;
    _socket->write(_proto.setInitSendingFileMessage(fileName));
}

//новый метод
void ClientManager::sendAcceptFile()
{
     _socket->write(_proto.setAcceptFileMessage());
}

//новый метод
void ClientManager::sendRejectFile()
{
     _socket->write(_proto.setRejectFileMessage());
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
    case ChatProto::InitSendingFile:
        emit initReceivingFile(_proto.name(), _proto.fileName(), _proto.fileSize());
        break;
    case ChatProto::AcceptSendingFile:
        sendFile();
    case ChatProto::RejectSendingFile:
        emit rejectReceivingFile();
    case ChatProto::Name:
        emit nameSet(_proto.name());
        break;
    default:
        break;
    }
}

void ClientManager::sendFile()
{
    _socket->write(_proto.setFileMessage(_tmpFileName));
}
