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

    QByteArray setClientNameMessage(QString previousName, QString name);
    QByteArray setConnectionMessage(QString clientName, QStringList otherClients);
    QByteArray setNewClientMessage(QString clientName);
    QByteArray setClientDisconnectedMessage(QString clientName);


    void loadData(QByteArray data);
    //константы
    const QString &message() const;
    const QString &name() const;
    MessageType messType() const;
    qint64 fileSize() const;
    QString fileName() const;
    QByteArray fileData() const;

    QString receiver() const;

private: //fields
    QByteArray getData(MessageType messType, QString data);
    MessageType _messType;
    QString _message;
    QString _name;
    QString _fileName;
    qint64 _fileSize;
    QByteArray _fileData;
    QString _receiver;
};

#endif // CHATPROTO_H
