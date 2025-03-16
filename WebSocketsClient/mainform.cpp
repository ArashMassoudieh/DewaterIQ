#include "mainform.h"
#include "ui_mainform.h"
#include "websocketclient.h"
#include <QJsonParseError>
#include <aquatable.h>
#include <QTableView>
#include "aquaplotter.h"



MainForm::MainForm(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainForm)
{

    ui->setupUi(this);
    this->showMaximized();
    WebSocketClient* client = new WebSocketClient(QUrl("ws://ec2-54-213-147-59.us-west-2.compute.amazonaws.com:12345"));
    //WebSocketClient* client = new WebSocketClient(QUrl("ws://localhost:12345"));
    connect(client, &WebSocketClient::textMessageRecieved, this, &MainForm::onTextMessageRecieved);
    tableviewer = new QTableView(this);
    tableviewer->setStyleSheet(
        "QTableView {"
        "    background-color: white;"   // ✅ Set background to white
        "    gridline-color: black;"      // ✅ Set grid lines to black
        "    border: 1px solid black;"    // ✅ Optional: Set outer border
        "}"
        "QHeaderView::section {"
        "    background-color: lightgray;"  // ✅ Light gray headers
        "    border: 1px solid black;"      // ✅ Black border for headers
        "}"
        );

    ui->horizontalLayout->addWidget(tableviewer);
    chart = new AquaPlotter(this);
    ui->horizontalLayout_2->addWidget(chart);



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
        QStringList columns_to_be_shown;
        columns_to_be_shown<<"Dewatered_Cake_TS_Percent";
        columns_to_be_shown<<"WetTons";
        columns_to_be_shown<<"Total_RRnR_OnM_Variable_Expenses";
        columns_to_be_shown<<"WetTons";
        columns_to_be_shown<<"TotalEmulsionPolymerCost";

        tablemodel = new AquaTableModel(table,columns_to_be_shown);
        tableviewer->setModel(tablemodel);
        tableviewer->resizeColumnsToContents();

        QMap<QString,QPair<AquaArray,AquaArray>> dataSeries;
        AquaArray x_value = table->GetColumn("Dewatered_Cake_TS_Percent");
        AquaArray y_value = table->GetColumn("Total_Cost_Emulsion");
        QPair<AquaArray,AquaArray> xy = QPair(x_value,y_value);
        dataSeries["TotalEmulsionPolymerCost"] = xy;

        x_value = table->GetColumn("Dewatered_Cake_TS_Percent");
        y_value = table->GetColumn("Total_Cost_Dry");
        xy = QPair(x_value,y_value);
        dataSeries["TotalDryPolymerCost"] = xy;

        chart->setData(dataSeries);
        chart->update();

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
