#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ClientManager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionConnect_triggered();
    void dataReceived(QString message);
    void on_buttonSend_clicked();
    void onTyping();
    void on_lineNickName_editingFinished();
    void onRejectReceivingFile();
    void onInitReceivingFile(QString clientName, QString fileName, qint64 fileSize);
    void on_attachButton_clicked();

    void onConnection(QString myName, QStringList clientsName);
    void onNewClientConnectedToServer(QString clientName);
    void onClientNameChanged(QString previousName, QString clientName);
    void onClientDisconnected(QString clientName);


private:
    Ui::MainWindow *ui;
    ClientManager *_client;
void setupClient();
};
#endif // MAINWINDOW_H
