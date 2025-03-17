#include "aquaplotter.h"
#include <QScatterSeries>
#include <QLegendMarker>


AquaPlotter::AquaPlotter(QWidget *parent)
    : QChartView(parent), chart(new QChart())
{
    setRenderHint(QPainter::Antialiasing);  // Enable smooth rendering
    setChart(chart);
    chart->setTitle("");
    chart->legend()->setAlignment(Qt::AlignBottom);
}

void AquaPlotter::setData(const QMap<QString,QPair<AquaArray,AquaArray>> &dataSeries) {
    data = dataSeries;  // Store data
    chart->removeAllSeries();  // Clear previous series
    QList<QAbstractAxis *> axes = chart->axes();
    for (QAbstractAxis *axis : axes) {
        chart->removeAxis(axis);
        delete axis;  // Prevent memory leaks
    }
    updatePlot();  // Refresh the plot
}

void AquaPlotter::updatePlot() {
    if (data.isEmpty()) return;  // No data? Exit
    chart->setTitle(PlotParameters.Title);
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

        QScatterSeries *scatterSeries;
        if (PlotParameters.symbols)
        {   scatterSeries = new QScatterSeries();
            scatterSeries->setMarkerShape(PlotParameters.markersymbol); // Set shape to circle
            scatterSeries->setMarkerSize(8.0); // Adjust the size of the circles
            scatterSeries->setName(it.key() + " (Points)");  // Label scatter series
        }


        for (int i=0; i<it.value().first.size(); i++) {
            series->append(it.value().first[i], it.value().second[i]);
            if (PlotParameters.symbols)
                scatterSeries->append(it.value().first[i], it.value().second[i]);
            minX = qMin(minX, it.value().first[i]);
            maxX = qMax(maxX, it.value().first[i]);
            minY = qMin(minY, it.value().second[i]);
            maxY = qMax(maxY, it.value().second[i]);
        }

        chart->addSeries(series);  // Add series to chart
        if (PlotParameters.symbols)
        {   chart->addSeries(scatterSeries);  // Add series to chart

            QPen linePen = series->pen();  // Get the pen used for the line
            scatterSeries->setPen(linePen);    // Apply the same pen to scatter series
            scatterSeries->setBrush(linePen.color()); // Ensure the fill color matches

            // **Remove scatter series from legend**
            for (QLegendMarker *marker : chart->legend()->markers(scatterSeries)) {
                marker->setVisible(false); // Hide marker for scatter points
            }
        }
    }



    // Create axes
    QValueAxis *axisX = new QValueAxis();
    axisX->setTitleText(PlotParameters.X_axis_title);
    axisX->setRange(minX, maxX);
    chart->addAxis(axisX, Qt::AlignBottom);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText(PlotParameters.Y_axis_title);
    axisY->setRange(minY, maxY);
    chart->addAxis(axisY, Qt::AlignLeft);

    // Attach all series to the axes
    for (auto series : chart->series()) {
        series->attachAxis(axisX);
        series->attachAxis(axisY);
    }


}
