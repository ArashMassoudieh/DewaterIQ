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
    client = new WebSocketClient(QUrl("ws://ec2-54-213-147-59.us-west-2.compute.amazonaws.com:12345"));
    //client = new WebSocketClient(QUrl("ws://localhost:12345"));
    connect(client, &WebSocketClient::textMessageRecieved, this, &MainForm::onTextMessageRecieved);
    connect(ui->pushButtonUpdate, &QPushButton::clicked, this, &MainForm::onUpdateRequested);
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
    ui->doubleSpinBoxDryPolymerUnitPrice->setValue(2.33);
    ui->doubleSpinBoxEmulsionPolymerUnitPrice->setValue(3.96);



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
        columns_to_be_shown<<"Total_RRR_Variable_Expenses";
        columns_to_be_shown<<"Total_Emulsion_Polymer_Cost";
        columns_to_be_shown<<"Total_Dry_Polymer_Cost";

        QStringList aliases;
        aliases << "Dewatered Cake TS (%)";
        aliases << "Wet Tons";
        aliases << "Total RR&R O&M Variable Expenses";
        aliases << "Total Emulsion Cost";
        aliases << "Total Dry Cost";

        AquaTable *extracted_columns = new AquaTable(table->Extract(columns_to_be_shown));

        extracted_columns->SetColumnAliases(aliases);
        tablemodel = new AquaTableModel(extracted_columns);
        tableviewer->setModel(tablemodel);
        tableviewer->resizeColumnsToContents();

        QMap<QString,QPair<AquaArray,AquaArray>> dataSeries;
        AquaArray x_value = table->GetColumn("Dewatered_Cake_TS_Percent");
        AquaArray y_value = table->GetColumn("Total_Emulsion_Polymer_Cost");
        QPair<AquaArray,AquaArray> xy = QPair(x_value,y_value);
        dataSeries["TotalEmulsionPolymerCost"] = xy;

        x_value = table->GetColumn("Dewatered_Cake_TS_Percent");
        y_value = table->GetColumn("Total_Dry_Polymer_Cost");
        xy = QPair(x_value,y_value);
        dataSeries["TotalDryPolymerCost"] = xy;
        plot_parameters chartparameters;
        chartparameters.Title = "Wholistic Dewatering Cost Analysis";
        chartparameters.X_axis_title = "Dewatered Cake TS (%)";
        chartparameters.Y_axis_title = "Cost ($)";
        chartparameters.markersymbol = QScatterSeries::MarkerShapeRectangle;
        chartparameters.symbols = true;
        chart->setPlotParameters(chartparameters);
        chart->setData(dataSeries);
        chart->update();

    }
}

void MainForm::onUpdateRequested()
{
    QString message;
    message+="BudgetBasicInputs:value:Polymer_Unit_Price_Emulsion="+QString::number(ui->doubleSpinBoxEmulsionPolymerUnitPrice->value())+",";
    message+="BudgetBasicInputs:value:Polymer_Unit_Price_Dry="+QString::number(ui->doubleSpinBoxDryPolymerUnitPrice->value())+",";
    message+="Perform Calculations";
    client->SendTextMessage(message);
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
