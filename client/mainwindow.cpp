#include "mainwindow.h"
#include "ChatWidget.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->centralwidget->setEnabled(false);
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
    connect(_client, &ClientManager::textMessagereceived, this, &MainWindow::dataReceived);
    //connect(_client, &ClientManager::nameSet, this, &MainWindow::setWindowTitle);
    connect(_client, &ClientManager::isTyping, this, &MainWindow::onTyping);

    connect(ui->lineMessage, &QLineEdit::textChanged, _client, &ClientManager::sendMessage);
    _client->connectToServer();
}

void MainWindow::dataReceived(QString message)
{
    auto chatWidget = new ChatWidget();
    chatWidget->setMessage(message, true);
    auto listItemWidget = new QListWidgetItem();
    listItemWidget->setSizeHint(QSize(0,5));
    ui->listMessages->addItem(listItemWidget);
    listItemWidget->setBackground(QColor(167, 255, 237));
    ui->listMessages->setItemWidget(listItemWidget, chatWidget);
}


void MainWindow::on_buttonSend_clicked()
{
    auto message = ui->lineMessage->text().trimmed();
    _client->sendMessage(message);
    ui->listMessages->addItem(message);
    //ui->lineMessage->setText("");
    auto chatWidget = new ChatWidget();
    chatWidget->setMessage(message, true);
    auto listItemWidget = new QListWidgetItem();
    listItemWidget->setSizeHint(QSize(0,5));
    ui->listMessages->addItem(listItemWidget);
    //listItemWidget->setBackground(QColor(167, 255, 237));
    ui->listMessages->setItemWidget(listItemWidget, chatWidget);
}

void MainWindow::onTyping()
{
    statusBar()->showMessage("Server is typing...", 800);
}

void MainWindow::on_lineMessage_editingFinished()
{
    auto name = ui->lineNickName->text().trimmed();
    _client->sendName(name);
}

