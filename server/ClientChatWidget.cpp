#include "ClientChatWidget.h"
#include "ui_ClientChatWidget.h"

ClientChatWidget::ClientChatWidget(QTcpSocket *client, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ClientChatWidget)
{
    ui->setupUi(this);
    _client = new ClientManager(client, this);
    //connect(_client, &QTcpSocket::readyRead, this, &ClientChatWidget::dataReceived); //пока не нужно, но оставим
    connect(_client, &ClientManager::disconnected, this, &ClientChatWidget::clientDisconnected);
    connect(_client, &ClientManager::textMessagereceived, this, &ClientChatWidget::textMessageReceived);
    connect(_client, &ClientManager::isTyping, this, &ClientChatWidget::onTyping);
    connect(_client, &ClientManager::nameSet, this, &ClientChatWidget::clientNameChanged);
    connect(ui->lineMessage, &QLineEdit::textChanged, _client, &ClientManager::sendIsTyping);
}

ClientChatWidget::~ClientChatWidget()
{
    delete ui;
}
/*void ClientChatWidget::dataReceived()
{
    auto data = _client->readAll();
    ui->listMessages->addItem(data);
}*/

void ClientChatWidget::clientDisconnected()
{
    ui->widgetSend->setEnabled(false);
}

void ClientChatWidget::on_buttonSend_clicked()
{
    auto message = ui->lineMessage->text().trimmed();
    _client->sendMessage(message);
    ui->lineMessage->setText("");
    ui->listMessages->addItem(message);
}

void ClientChatWidget::textMessageReceived(QString message)
{

}

void ClientChatWidget::onTyping()
{

}

