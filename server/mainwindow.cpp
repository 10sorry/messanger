#include "mainwindow.h"
#include "ClientChatWidget.h"
#include "qtcpsocket.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupServer();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newClientConnected(QTcpSocket *client)
{
    auto id = client->property("id").toInt();
    ui->listClients->addItem(QString("New Client added: %1").arg(id));
    auto chatWidget = new ClientChatWidget(client);
    ui->tabChats->addTab(chatWidget, QString("Client (%1").arg(id));
    connect(chatWidget, &ClientChatWidget::clientNameChanged, this, &MainWindow::setClientName);
    connect(chatWidget, &ClientChatWidget::isTyping, [this] (QString name) {this->statusBar()->showMessage(name, 800);});

    connect(chatWidget, &ClientChatWidget::messForClients, _server, &ServerManager::onMessForClients);

}

void MainWindow::clientDisconnected(QTcpSocket *client)
{
    auto id = client->property("id").toInt();
    ui->listClients->addItem(QString("Client disconnected: %1").arg(id));

}

void MainWindow::setClientName(QString previousName, QString name)
{
    auto widget = qobject_cast<QWidget *>(sender());
    auto index = ui->tabChats->indexOf(widget);
    ui->tabChats->setTabText(index, name);
    _server->notifyOtherClients(previousName, name);
}

void MainWindow::setupServer()
{
    _server = new ServerManager();
    connect(_server, &ServerManager::newClientConnected, this, &MainWindow::newClientConnected);
    connect(_server, &ServerManager::clientDisconnected, this, &MainWindow::clientDisconnected);
}

void MainWindow::on_tabChats_tabCloseRequested(int index)
{
    auto chatWidget =qobject_cast<ClientChatWidget *>(ui->tabChats->widget(index));
    chatWidget->disconnect();
    ui->tabChats->removeTab(index);
}

