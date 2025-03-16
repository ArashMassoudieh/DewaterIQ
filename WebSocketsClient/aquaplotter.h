#ifndef AQUAPLOTTER_H
#define AQUAPLOTTER_H

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QVector>
#include <QPair>
#include "AquaArray.h"

//QT_CHARTS_USE_NAMESPACE  // Required for Qt Charts



class AquaPlotter : public QChartView {
    Q_OBJECT

public:
    explicit AquaPlotter(QWidget *parent = nullptr);

    // Set multiple data series
    void setData(const QMap<QString,QPair<AquaArray,AquaArray>> &dataSeries);

private:
    QChart *chart;  // Main chart object
    QMap<QString,QPair<AquaArray,AquaArray>> data;  // Data storage

    void updatePlot();  // Function to refresh plot
};

#endif // AQUAPLOTTER_H
