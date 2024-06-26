#include "ClientChatWidget.h"
#include "ui_ClientChatWidget.h"
#include <QMessageBox>
#include <QDesktopServices>

ClientChatWidget::ClientChatWidget(QTcpSocket *client, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ClientChatWidget)
{
    ui->setupUi(this);
    _client = new ClientManager(client, this);
    connect(_client, &ClientManager::disconnected, this, &ClientChatWidget::clientDisconnected);
    connect(_client, &ClientManager::textMessageReceived, this, &ClientChatWidget::textMessageReceived);
    connect(_client, &ClientManager::isTyping, this, &ClientChatWidget::onTyping);;
    connect(_client, &ClientManager::nameChanged, this, &ClientChatWidget::onClientNameChanged);
    connect(_client, &ClientManager::initReceivingFile, this, &::ClientChatWidget::onInitReceivingFile);
    connect(_client, &ClientManager::fileSaved, this, &ClientChatWidget::onFileSaved);
    connect(ui->lineMessage, &QLineEdit::textChanged, _client, &ClientManager::sendIsTyping);

    directory.mkdir(_client->name());
    directory.setPath("./" + _client->name());
}

void ClientChatWidget::disconnect()
{
    _client->disconnectFromHost();
}

ClientChatWidget::~ClientChatWidget()
{
    delete ui;
}

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

void ClientChatWidget::textMessageReceived(QString message, QString receiver)
{
    if(receiver == "Server" || receiver == "All")
    {
        ui->listMessages->addItem(message);
    }
    if(receiver != "Server")
    {
        emit messForClients(message, receiver, _client->name());
    }
}

void ClientChatWidget::onTyping()
{
    emit isTyping(QString("%1 is typing...").arg(_client->name()));
}

void ClientChatWidget::onInitReceivingFile(QString clientName, QString fileName, qint64 fileSize)
{
    _client->sendAcceptFile();

}

void ClientChatWidget::onFileSaved(QString path)
{
    auto message = QString("File saved here:\n%1").arg(path);
    QMessageBox::information(this, "File saved", message);
}


void ClientChatWidget::on_labelOpen_linkActivated(const QString &link)
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(_client->name()));
}

void ClientChatWidget::onClientNameChanged(QString previousName, QString name)
{
    QFile::rename(directory.canonicalPath(), name);
    emit clientNameChanged(previousName, name);
}



