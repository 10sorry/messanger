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

QByteArray ChatProto::loadData(QByteArray data)
{
    QDataStream in(&data, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_6_0);
    in >> _messType;
    switch(_messType)
    {
        case Text:
            in >> _message;
            break;
            in >> _name;
        case Name:
            break;
        default:
            break;
    }
}
const QString &ChatProto::message() const
{
    return _message;
}

QByteArray ChatProto::getData(MessageType messType, QString data)
{
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);
    out << messType << data;
    return ba;
}

QString ChatProto::message() const
{
    return _message;
}
