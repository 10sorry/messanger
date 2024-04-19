#include "ChatProto.h"
#include <QIODevice>
#include <QFileInfo>

ChatProto::ChatProto()
{

}

QByteArray ChatProto::textMessage(QString message, QString receiver)
{
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);
    out << Text << receiver << message;
    return ba;
}
QByteArray ChatProto::isTypingMessage()
{
    return getData(IsTyping, "");
}
QByteArray ChatProto::setNameMessage(QString name)
{
    return getData(SetName, name);
}

QByteArray ChatProto::setInitSendingFileMessage(QString fileName)
{
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);
    QFileInfo info(fileName);
    out << InitSendingFile << info.fileName() << info.size();
    return ba;
}

QByteArray ChatProto::setAcceptFileMessage()
{
    return getData(AcceptSendingFile, "");
}

QByteArray ChatProto::setRejectFileMessage()
{
    return getData(RejectSendingFile, "");
}

QByteArray ChatProto::setFileMessage(QString fileName)
{
    QByteArray ba;
    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly))
    {
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_6_0);
        QFileInfo info(fileName);
        out << SendFile << info.fileName() << info.size() << file.readAll();
        file.close();
    }
    return ba;
}

void ChatProto::loadData(QByteArray data)
{
    QDataStream in(&data, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_6_0);
    in >> _messType;
    switch(_messType) {
    case Text:
        in >> _receiver >> _message;
        break;
    case InitSendingFile:
        in >> _fileName >> _fileSize;
        break;
    case SendFile:
        in >> _fileName >> _fileSize >> _fileData;
        break;
    case SetName:
        in >> _name;
        break;
    case ClientName:
        in >> _previousName >> _clientName;
        break;
    case NewClient:
    case ClientDisconnected:
        in >> _clientName;
        break;
    case Connection:
        in >> _myName >> _clientsName;
        break;
    default:
        break;
    }
}

QByteArray ChatProto::getData(MessageType messType, QString data)
{
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);
    out << messType << data;
    return ba;
}

const QString &ChatProto::myName() const
{
    return _myName;
}

const QStringList &ChatProto::clientsName() const
{
    return _clientsName;
}

const QString &ChatProto::previousName() const
{
    return _previousName;
}

const QString &ChatProto::clientName() const
{
    return _clientName;
}

QString ChatProto::receiver() const
{
    return _receiver;
}

const QByteArray &ChatProto::fileData() const
{
    return _fileData;
}

const QString &ChatProto::fileName() const
{
    return _fileName;
}

qint64 ChatProto::fileSize() const
{
    return _fileSize;
}

ChatProto::MessageType ChatProto::messType() const
{
    return _messType;
}

const QString &ChatProto::name() const
{
    return _name;
}

const QString &ChatProto::message() const
{
    return _message;
}
