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
    ~ClientChatWidget();

private slots:
    void on_buttonSend_clicked();
    void clientDisconnected();
    void textMessageReceived(QString message);
    void onTyping();
    void onInitReceivingFile(QString clientName, QString fileName, qint64 fileSize);
    void onFileSaved(QString path);

    void on_labelOpen_linkActivated(const QString &link);

    void onClientNameChanged(QString name);

signals:
    void clientNameChanged(QString name);
    void isTyping(QString message);

private:
    Ui::ClientChatWidget *ui;
    ClientManager *_client;
    QDir directory;
};

#endif // CLIENTCHATWIDGET_H
