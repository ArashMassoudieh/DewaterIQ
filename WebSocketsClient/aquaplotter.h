#ifndef AQUAPLOTTER_H
#define AQUAPLOTTER_H

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QVector>
#include <QPair>
#include "AquaArray.h"
#include <QScatterSeries>

//QT_CHARTS_USE_NAMESPACE  // Required for Qt Charts

struct plot_parameters
{
    QString X_axis_title;
    QString Y_axis_title;
    QString Title;
    bool symbols = false;
    QScatterSeries::MarkerShape markersymbol = QScatterSeries::MarkerShapeCircle;
};

class AquaPlotter : public QChartView {
    Q_OBJECT

public:
    explicit AquaPlotter(QWidget *parent = nullptr);

    // Set multiple data series
    void setData(const QMap<QString,QPair<AquaArray,AquaArray>> &dataSeries);
    void setPlotParameters(const plot_parameters &params) {PlotParameters = params;}
private:
    QChart *chart;  // Main chart object
    QMap<QString,QPair<AquaArray,AquaArray>> data;  // Data storage
    plot_parameters PlotParameters;
    void updatePlot();  // Function to refresh plot
};

#endif // AQUAPLOTTER_H
