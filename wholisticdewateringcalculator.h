#ifndef WHOLISTICDEWATERINGCALCULATOR_H
#define WHOLISTICDEWATERINGCALCULATOR_H
#include "aquatable.h"
#include "System.h"

struct expression {
    QString Name;
    QString Type;
    QString Expression;
};

class WholisticDewateringCalculator
{
public:
    WholisticDewateringCalculator();
    AquaTable PerformCalculation();
    bool SetValue(const QString &exression, const double& value);
    bool BuildSystem(const QString &JsonFileName);
private:
    System system;
    QVector<expression> Expressions;



};

#endif // WHOLISTICDEWATERINGCALCULATOR_H
