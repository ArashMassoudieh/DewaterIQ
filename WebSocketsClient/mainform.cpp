#include "mainform.h"
#include "ui_mainform.h"
#include "websocketclient.h"
#include <QJsonParseError>
#include <aquatable.h>
#include <QTableView>


MainForm::MainForm(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MainForm)
{
    ui->setupUi(this);
    WebSocketClient* client = new WebSocketClient(QUrl("ws://ec2-54-213-147-59.us-west-2.compute.amazonaws.com:12345"));
    //WebSocketClient* client = new WebSocketClient(QUrl("ws://localhost:12345"));
    connect(client, &WebSocketClient::textMessageRecieved, this, &MainForm::onTextMessageRecieved);
    tableviewer = new QTableView(this);
    ui->horizontalLayout->addWidget(tableviewer);

}

MainForm::~MainForm()
{
    delete table;
    delete tablemodel;
    delete ui;
}

void MainForm::onTextMessageRecieved(const QString &msg)
{
    qDebug() << "Slot received message:" << msg;
    if (isValidJson(msg))
    {
        table = new AquaTable();
        table->FromJsonString(msg);
        tablemodel = new AquaTableModel(table);
        tableviewer->setModel(tablemodel);
    }
}

bool MainForm::isValidJson(const QString &jsonString)
{
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8(), &error);

    if (error.error == QJsonParseError::NoError) {
        return true;
    } else {
        qDebug() << "Invalid JSON:" << error.errorString();
        return false;
    }
}
