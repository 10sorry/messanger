#ifndef CHATPROTO_H
#define CHATPROTO_H

#include <QByteArray>
#include <QString>
#include <QStringList>

class ChatProto
{
public:
    enum MessageType
    {
        Text,
        File,
        InitSendingFile,
        AcceptSendingFile,
        RejectSendingFile,
        SendFile,
        IsTyping,
        SetName,
        ClientName,
        Connection,
        NewClient,
        ClientDisconnected
    };
    ChatProto();
    QByteArray textMessage(QString message, QString receiver);
    QByteArray isTypingMessage();
    QByteArray setNameMessage(QString name);
    QByteArray setInitSendingFileMessage(QString fileName);
    QByteArray setAcceptFileMessage();
    QByteArray setRejectFileMessage();
    QByteArray setFileMessage(QString fileName);

    void loadData(QByteArray data);
//константы
    const QString &message() const;
    const QString &name() const;
    MessageType messType() const;
    qint64 fileSize() const;
    QString fileName() const;
    QByteArray fileData() const;

    QString receiver() const;

    QString clientName() const;

    QString previousName() const;

    QStringList clientsName() const;

    QString myName() const;

private: //fields
    QByteArray getData(MessageType messType, QString data);
    MessageType _messType;
    QString _message;
    QString _name;
    QString _fileName;
    qint64 _fileSize;
    QByteArray _fileData;
    QString _receiver;
    QString _clientName;
    QString _previousName;
    QStringList _clientsName;
    QString _myName;
};

#endif // CHATPROTO_H
