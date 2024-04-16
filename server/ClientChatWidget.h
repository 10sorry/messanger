#ifndef CLIENTCHATWIDGET_H
#define CLIENTCHATWIDGET_H

#include <QWidget>

namespace Ui {
class ClientChatWidget;
}

class ClientChatWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ClientChatWidget(QWidget *parent = nullptr);
    ~ClientChatWidget();

private:
    Ui::ClientChatWidget *ui;
};

#endif // CLIENTCHATWIDGET_H
