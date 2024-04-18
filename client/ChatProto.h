#ifndef CHATPROTO_H
#define CHATPROTO_H

#include <QByteArray>
#include <QString>

class ChatProto
{
public:
    enum MessageType
    {
        Text,
        File,
        IsTyping,
        Name
    };
    ChatProto();
    QByteArray textMessage(QString message);
    QByteArray isTypingMessage();
    QByteArray setNameMessage(QString name);
    void loadData(QByteArray data);
    const QString &message() const;

    const QString &name() const;

    MessageType messType() const;

private:
    QByteArray getData(MessageType messType, QString data);
    MessageType _messType;
    QString _message;
    QString _name;
};

#endif // CHATPROTO_H
