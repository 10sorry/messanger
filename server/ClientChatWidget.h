#ifndef CLIENTCHATWIDGET_H
#define CLIENTCHATWIDGET_H

#include "ClientManager.h"
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
    //void dataReceived();
    void on_buttonSend_clicked();
    void clientDisconnected();
    void textMessageReceived(QString message);
    void onTyping();
signals:
    void clientNameChanged(QString nickName);
    void isTyping(QString message);

private:
    Ui::ClientChatWidget *ui;
    ClientManager *_client;
};

#endif // CLIENTCHATWIDGET_H
