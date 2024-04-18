#include "ClientManager.h"
#include <QDir>

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

ClientManager::ClientManager(QTcpSocket *client, QObject *parent)
    : QObject{parent},
    _socket(client)
{
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

QString ClientManager::name() const
{
    auto id = _socket->property("id").toInt();
    auto name = _proto.name().length() > 0 ? _proto.name() : QString("Client (%1)").arg(id);
    return name;
}

void ClientManager::sendIsTyping()
{
    _socket->write(_proto.isTypingMessage());
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

void ClientManager::sendInitSendingFile(QString fileName)
{
    _tmpFileName = fileName;
    _socket->write(_proto.setInitSendingFileMessage(fileName));
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
    case ChatProto::SendFile:
        saveFile();
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

void ClientManager::saveFile()
{
    QDir directory;
    directory.mkdir(name());
    auto path = QString("%1/%2/%3_%4").arg(directory.canonicalPath(), name(), QDateTime::currentDateTime().toString("ddMMyyy_HHmmss"), _proto.fileName());
    QFile file(path);
    if(file.open(QIODevice::WriteOnly))
    {
        file.write(_proto.fileData());
        file.close();
        emit fileSaved(path);
    }
}

