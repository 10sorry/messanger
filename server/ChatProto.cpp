#include "ChatProto.h"
#include <QIODevice>

ChatProto::ChatProto()
{

}

QByteArray ChatProto::textMessage(QString message)
{
    return getData(Text, message);
}
QByteArray ChatProto::isTypingMessage()
{
    return getData(IsTyping, "");
}
QByteArray ChatProto::setNameMessage(QString name)
{
    return getData(Name, name);
}

void ChatProto::loadData(QByteArray data)
{
    QDataStream in(&data, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_6_0);
    in >> _messType;
    switch(_messType) {
    case Text:
        in >> _message;
        break;
    case Name:
        in >> _name;
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
