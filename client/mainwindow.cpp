#include "mainwindow.h"
#include "ChatWidget.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->centralwidget->setEnabled(false);
    QString iconName = "://C:/Users/foxhalf/Downloads/attachment.png"; // Укажи путь к иконке
    QIcon icon(iconName);
    ui->attachButton->setIcon(icon);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionConnect_triggered()
{
    _client = new ClientManager();
    connect(_client, &ClientManager::connected, [this]()
    {
        ui->centralwidget->setEnabled(true);
    });
    connect(_client, &ClientManager::disconnected, [this]()
            {
                ui->centralwidget->setEnabled(false);
            });
    connect(_client, &ClientManager::textMessageReceived, this, &MainWindow::dataReceived);
    connect(ui->lineMessage, &QLineEdit::textChanged, _client, &ClientManager::sendIsTyping);
    //connect(_client, &ClientManager::nameSet, this, &MainWindow::setWindowTitle);
    connect(_client, &ClientManager::rejectReceivingFile, this, &MainWindow::onRejectReceivingFile);
    connect(_client, &ClientManager::initReceivingFile, this, &MainWindow::onInitReceivingFile);

    _client->connectToServer();
}

void MainWindow::on_buttonSend_clicked()
{
    auto message = ui->lineMessage->text().trimmed();
    _client->sendMessage(message);
    //ui->listMessages->addItem(message);
    ui->lineMessage->setText("");
    ui->lineMessage->setFocus();


    auto chatWidget = new ChatWidget();
    chatWidget->setMessage(message, true);
    auto listItemWidget = new QListWidgetItem();
    listItemWidget->setSizeHint(QSize(0,65));
    ui->listMessages->addItem(listItemWidget);
    ui->listMessages->setItemWidget(listItemWidget, chatWidget);
}

void MainWindow::dataReceived(QString message)
{
    auto chatWidget = new ChatWidget();
    chatWidget->setMessage(message);
    auto listItemWidget = new QListWidgetItem();
    listItemWidget->setSizeHint(QSize(0,65));
    ui->listMessages->addItem(listItemWidget);
    listItemWidget->setBackground(QColor(151, 100, 27));
    ui->listMessages->setItemWidget(listItemWidget, chatWidget);
}

void MainWindow::onTyping()
{
    statusBar()->showMessage("Server is typing...", 800);
}

void MainWindow::on_lineMessage_editingFinished()
{

}

void MainWindow::on_lineNickName_editingFinished()
{
    auto name = ui->lineNickName->text().trimmed();
    _client->sendName(name);
}

void MainWindow::onRejectReceivingFile()
{
    QMessageBox::critical(this, "Sending File", "Operation rejected...");

}

void MainWindow::onInitReceivingFile(QString clientName, QString fileName, qint64 fileSize)
{
    auto message = QString("Client (%1) wants to send a file. Do you wand to accept it?\nFile Name:%2\nFile Size: %3 bytes").arg(clientName, fileName).arg(fileSize);
    auto result = QMessageBox::critical(this, "Receiving File", message);
    if(result ==QMessageBox::Yes)
    {
        _client->sendAcceptFile();
    }
    else
    {
        _client->sendRejectFile();
    }

}




void MainWindow::on_attachButton_clicked()
{
    /*QString iconName = "://C:/Users/foxhalf/Downloads/attachment.png";
    auto icon = QIcon(iconName);
    ui->attachButton->setIcon(icon);
    */
    auto fileName = QFileDialog::getOpenFileName(this, "Select a file", "/home");
    _client->sendInitSendingFile(fileName);
}

