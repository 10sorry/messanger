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
        Name
    };
    ChatProto();
    QByteArray textMessage(QString message);
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

private: //fields
    QByteArray getData(MessageType messType, QString data);
    MessageType _messType;
    QString _message;
    QString _name;
    QString _fileName;
    qint64 _fileSize;
    QByteArray _fileData;
};

#endif // CHATPROTO_H
