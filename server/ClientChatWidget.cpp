#include "ClientChatWidget.h"
#include "ui_ClientChatWidget.h"

ClientChatWidget::ClientChatWidget(QTcpSocket *client, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ClientChatWidget),
    _client(client)
{
    ui->setupUi(this);
    connect(_client, &QTcpSocket::readyRead, this, &ClientChatWidget::dataReceived);
}

ClientChatWidget::~ClientChatWidget()
{
    delete ui;
}
void ClientChatWidget::dataReceived()
{
    auto data = _client->readAll();
    ui->listMessages->addItem(data);
}

void ClientChatWidget::on_buttonSend_clicked()
{
    _client->write(ui->lineMessage->text().trimmed().toUtf8());
    ui->lineMessage->setText("");
}

