#ifndef WHOLISTICDEWATERINGCALCULATOR_H
#define WHOLISTICDEWATERINGCALCULATOR_H
#include "aquatable.h"
#include "System.h"

class WholisticDewateringCalculator
{
public:
    WholisticDewateringCalculator();
    bool LoadData();
    AquaTable PerformCalculation();
    bool SetValue(const QString &exression, const double& value);
private:
    System system;

};

#endif // WHOLISTICDEWATERINGCALCULATOR_H
