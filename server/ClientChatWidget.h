#ifndef CLIENTCHATWIDGET_H
#define CLIENTCHATWIDGET_H

#include "ClientManager.h"
#include "qdir.h"
#include <QWidget>
#include <QTcpSocket>

namespace Ui {
class ClientChatWidget;
}

class ClientChatWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ClientChatWidget(QTcpSocket *client, QWidget *parent = nullptr);
    void disconnect();
    ~ClientChatWidget();

private slots:
    void on_buttonSend_clicked();
    void clientDisconnected();
    void textMessageReceived(QString message, QString receiver);
    void onTyping();
    void onInitReceivingFile(QString clientName, QString fileName, qint64 fileSize);
    void onFileSaved(QString path);

    void on_labelOpen_linkActivated(const QString &link);

    void onClientNameChanged(QString previousName, QString name);

signals:
    void clientNameChanged(QString previousName, QString name);
    void isTyping(QString message);
    void messForClients(QString message, QString receiver, QString sender);

private:
    Ui::ClientChatWidget *ui;
    ClientManager *_client;
    QDir directory;
};

#endif // CLIENTCHATWIDGET_H
