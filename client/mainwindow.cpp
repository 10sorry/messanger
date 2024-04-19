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
    setupClient();

    ui->centralwidget->setEnabled(false);
    QString iconName = "://C:/Users/foxhalf/Downloads/attachment.png";
    QIcon icon(iconName);
    ui->attachButton->setIcon(icon);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupClient()
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
    connect(_client, &ClientManager::isTyping, this, &MainWindow::onTyping);
    connect(_client, &ClientManager::rejectReceivingFile, this, &MainWindow::onRejectReceivingFile);
    connect(_client, &ClientManager::initReceivingFile, this, &MainWindow::onInitReceivingFile);

    connect(ui->lineMessage, &QLineEdit::textChanged, _client, &ClientManager::sendIsTyping);
    connect(_client, &ClientManager::connection, this, &MainWindow::onConnection);
    connect(_client, &ClientManager::newClientConnectedToServer, this, &MainWindow::onNewClientConnectedToServer);
    connect(_client, &ClientManager::clientDisconnected, this, &MainWindow::onClientDisconnected);
    connect(_client, &ClientManager::clientNameChanged, this, &MainWindow::onClientNameChanged);
}

void MainWindow::on_actionConnect_triggered()
{
    //setupClient();

    _client->connectToServer();
}

void MainWindow::on_buttonSend_clicked()
{
    auto message = ui->lineMessage->text().trimmed();
    _client->sendMessage(message, ui->comboBoxReceiver->currentText());
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

void MainWindow::on_lineNickName_editingFinished()
{
    auto name = ui->lineNickName->text().trimmed();
    _client->sendName(name);
}

void MainWindow::onRejectReceivingFile()
{
    //QMessageBox::critical(this, "Sending File", "Operation rejected...");

}

void MainWindow::onInitReceivingFile(QString clientName, QString fileName, qint64 fileSize)
{
    _client->sendAcceptFile();
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

void MainWindow::onConnection(QString myName, QStringList clientsName)
{
    ui->comboBoxReceiver->clear();
    clientsName.prepend("All");
    clientsName.prepend("Server");
    foreach (auto client, clientsName) {
        ui->comboBoxReceiver->addItem(client);
    }
    setWindowTitle(myName);
}

void MainWindow::onNewClientConnectedToServer(QString clientName)
{
    ui->comboBoxReceiver->addItem(clientName);
}

void MainWindow::onClientNameChanged(QString previousName, QString clientName)
{
    for (int i = 0; i < ui->comboBoxReceiver->count(); ++i)
    {
        if (ui->comboBoxReceiver->itemText(i) == previousName)
        {
            ui->comboBoxReceiver->setItemText(i, clientName);
            return;
        }
    }
}

void MainWindow::onClientDisconnected(QString clientName)
{
    for (int i = 0; i < ui->comboBoxReceiver->count(); ++i)
    {
        if (ui->comboBoxReceiver->itemText(i) == clientName)
        {
            ui->comboBoxReceiver->removeItem(i);
            return;
        }
    }
}


