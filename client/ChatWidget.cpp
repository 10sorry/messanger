#include <QDateTime>
#include "ChatWidget.h"
#include "ui_ChatWidget.h"

ChatWidget::ChatWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatWidget)
{
    ui->setupUi(this);
}

ChatWidget::~ChatWidget()
{
    delete ui;
}

void ChatWidget::setMessage(QString message, bool isMyMessage)
{
    if(isMyMessage)
    {
        ui->labelMessage->setAlignment(Qt::AlignRight);
    }
    ui->labelMessage->setText(message);
    ui->labelTime->setText(QDateTime::currentDateTime().toString("HH:mm"));
}

