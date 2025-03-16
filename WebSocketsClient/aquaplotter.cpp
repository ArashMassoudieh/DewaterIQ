#include "aquaplotter.h"


AquaPlotter::AquaPlotter(QWidget *parent)
    : QChartView(parent), chart(new QChart())
{
    setRenderHint(QPainter::Antialiasing);  // Enable smooth rendering
    setChart(chart);
    chart->setTitle("AquaPlotter - Multiple Line Series");
    chart->legend()->setAlignment(Qt::AlignBottom);
}

void AquaPlotter::setData(const QMap<QString,QPair<AquaArray,AquaArray>> &dataSeries) {
    data = dataSeries;  // Store data

    chart->removeAllSeries();  // Clear previous series
    updatePlot();  // Refresh the plot
}

void AquaPlotter::updatePlot() {
    if (data.isEmpty()) return;  // No data? Exit

    // Set axis ranges dynamically
    double minX = data.begin()->first.first() , maxX = data.begin()->first.last();
    double minY = data.begin()->second.first(), maxY = data.begin()->second.first();

    // Loop through all data series


    for (QMap<QString, QPair<AquaArray,AquaArray>>::iterator it = data.begin(); it != data.end(); ++it) {
        if (it.value().first.size()!=it.value().second.size())
        {
            qDebug()<<"The sizes does not match!";
        }
        QLineSeries *series = new QLineSeries();
        series->setName(it.key());  // Label each series

        for (int i=0; i<it.value().first.size(); i++) {
            series->append(it.value().first[i], it.value().second[i]);

            minX = qMin(minX, it.value().first[i]);
            maxX = qMax(maxX, it.value().first[i]);
            minY = qMin(minY, it.value().second[i]);
            maxY = qMax(maxY, it.value().second[i]);
        }

        chart->addSeries(series);  // Add series to chart
    }

    // Create axes
    QValueAxis *axisX = new QValueAxis();
    axisX->setTitleText("X Axis");
    axisX->setRange(minX, maxX);
    chart->addAxis(axisX, Qt::AlignBottom);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Y Axis");
    axisY->setRange(minY, maxY);
    chart->addAxis(axisY, Qt::AlignLeft);

    // Attach all series to the axes
    for (auto series : chart->series()) {
        series->attachAxis(axisX);
        series->attachAxis(axisY);
    }
}
