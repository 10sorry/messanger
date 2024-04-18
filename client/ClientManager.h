#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include "ChatProto.h"
#include <QObject>
#include <QTcpSocket>

class ClientManager : public QObject
{
    Q_OBJECT
public:
    explicit ClientManager(QHostAddress ip = QHostAddress::LocalHost, ushort port = 4500, QObject *parent = nullptr);
    void connectToServer();
    void sendMessage(QString message, QString receiver);
    void sendName(QString name);
    void setInitFile(QString fileName);
    void sendIsTyping();

    void sendInitSendingFile(QString fileName);
    void sendAcceptFile();
    void sendRejectFile();

signals:
    void connected();
    void disconnected();
    //void dataReceived(QByteArray data); //более не требуется
    void textMessageReceived(QString message);
    void isTyping();
    void nameSet(QString name);
    void rejectReceivingFile();
    void initReceivingFile(QString clietnName, QString fileName, qint64 fileSize);

    void connection(QString myName, QStringList clientsName);
    void newClientConnectedToServer(QString clienName);
    void clientNameChanged(QString previousName, QString clientName);
    void clientDisconnected(QString clientName);

private slots:
    void readyRead();

private: //fields
    QTcpSocket *_socket;
    QHostAddress _ip;
    ushort _port;
    ChatProto _proto;
    QString _tmpFileName;

private: //methods
    void sendFile();
};

#endif // CLIENTMANAGER_H
