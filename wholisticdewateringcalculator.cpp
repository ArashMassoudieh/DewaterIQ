#include "wholisticdewateringcalculator.h"
#include <iostream>
#include <QFile>
#include <QJsonArray>

WholisticDewateringCalculator::WholisticDewateringCalculator() {}


bool WholisticDewateringCalculator::BuildSystem(const QString &filename)
{
    system.clear();

    QFile file(filename);

    // Open the file
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file:" << filename;
        return false;
    }

    // Read the file content
    QByteArray fileData = file.readAll();
    file.close();

    // Parse the JSON content
    QJsonDocument jsonDoc = QJsonDocument::fromJson(fileData);
    if (jsonDoc.isNull() || !jsonDoc.isObject()) {
        qDebug() << "Invalid JSON format in file:" << filename;
        return false;
    }

    QJsonObject rootObj = jsonDoc.object();

    if (!rootObj.contains("Tables"))
    {
        qDebug() << "No 'Tables' object was found";
        return false;
    }

    if (!rootObj.contains("Expressions"))
    {
        qDebug() << "No 'Expressions' object was found";
        return false;
    }
    QJsonArray Tables = rootObj["Tables"].toArray();
    //qDebug()<<Tables;
    for (int i=0; i<Tables.count(); i++)
    {
        DataStructure table;
        if (!table.readFromJsonFile(Tables[i].toObject()["FileName"].toString()))
        {   if (!table.readFromJsonFile(Tables[i].toObject()["FileName"].toString().split("/").last()))
                qDebug()<<"Table '" + Tables[i].toObject()["FileName"].toString() + "' was not found";
        }
        if (table.count()!=0)
            system[Tables[i].toObject()["Name"].toString()] = table;
    }

    QJsonArray ExpressionJson = rootObj["Expressions"].toArray();
    //qDebug()<<Tables;
    for (int i=0; i<ExpressionJson.count(); i++)
    {
        expression anexpression;
        anexpression.Expression = ExpressionJson[i].toObject()["Expression"].toString();
        anexpression.Type = ExpressionJson[i].toObject()["Type"].toString();
        anexpression.Name = ExpressionJson[i].toObject()["Variable"].toString();
        Expressions.append(anexpression);
    }

    PerformCalculation();
    return true;
}

bool WholisticDewateringCalculator::LoadData()
{
    system.clear();
    DataStructure BudgetBasicInputs;
    DataStructure TransportationCosts;
    DataStructure DistributionCosts;
    DataStructure StepFunctionDistributionExpenses;
    DataStructure StepFunctionTransportationExpenses;
    DataStructure DiscreteAnnualCostswithDecreaseTS;
    DataStructure CakeTSvsPolyDoseTrend;
    if (!BudgetBasicInputs.readFromJsonFile("../../json_input_files/BudgetBasicInputs.json"))
        if (!BudgetBasicInputs.readFromJsonFile("json_input_files/BudgetBasicInputs.json"))
            return false;
    if (!TransportationCosts.readFromJsonFile("../../json_input_files/TransportationCosts.json"))
        if (!TransportationCosts.readFromJsonFile("json_input_files/TransportationCosts.json"))
            return false;
    if (!DistributionCosts.readFromJsonFile("../../json_input_files/DistributionCosts.json"))
        if (!DistributionCosts.readFromJsonFile("json_input_files/DistributionCosts.json"))
            return false;
    if (!StepFunctionDistributionExpenses.readFromJsonFile("../../json_input_files/StepFunctionDistributionCosts.json"))
        if (!StepFunctionDistributionExpenses.readFromJsonFile("json_input_files/StepFunctionDistributionCosts.json"))
            return false;
    if (!StepFunctionTransportationExpenses.readFromJsonFile("../../json_input_files/StepFunctionTransportationCosts.json"))
        if (!StepFunctionTransportationExpenses.readFromJsonFile("json_input_files/StepFunctionTransportationCosts.json"))
            return false;
    if (!DiscreteAnnualCostswithDecreaseTS.readFromJsonFile("../../json_input_files/DiscreteAnnualCostswithDecreaseTS.json"))
        if (!DiscreteAnnualCostswithDecreaseTS.readFromJsonFile("json_input_files/DiscreteAnnualCostswithDecreaseTS.json"))
            return false;
    if (!CakeTSvsPolyDoseTrend.readFromJsonFile("../../json_input_files/Cake_TS_vs_Poly_Dose_Trend.json"))
        if (!CakeTSvsPolyDoseTrend.readFromJsonFile("json_input_files/Cake_TS_vs_Poly_Dose_Trend.json"))
            return false;

    system["BudgetBasicInputs"] = BudgetBasicInputs;
    system["TransporationCosts"] = TransportationCosts;
    system["DistributionCosts"] = DistributionCosts;
    system["StepFunctionTransportationExpenses"] = StepFunctionTransportationExpenses;
    system["StepFunctionDistributionExpenses"] = StepFunctionDistributionExpenses;
    system["DiscreteAnnualCostswithDecreaseTS"] = DiscreteAnnualCostswithDecreaseTS;
    system["CakeTSvsPolyDoseTrend"] = CakeTSvsPolyDoseTrend;
    return true;
}

AquaTable WholisticDewateringCalculator::PerformCalculation()
{

    AquaTable out;
    for (int i=0; i<Expressions.count(); i++)
    {
        if (Expressions[i].Type == "Sequence")
        {
            QStringList parameters = Expressions[i].Expression.split(":");
            out.AppendSequence(Expressions[i].Name,parameters[0].toDouble(),parameters[1].toDouble(),parameters[2].toInt(),sequencemode::linear);
        }
        if (Expressions[i].Type == "Scalar")
        {
            double var = system.Calculate(Expressions[i].Expression);
            system.InsertScalar(Expressions[i].Name,var);
        }
        if (Expressions[i].Type == "Array")
        {
            AquaArray var = system.Calculate(&out,Expressions[i].Expression);
            out.AppendColumn(Expressions[i].Name, var);
        }
    }

    std::cout<<"Finished"<<std::endl;
    //out.WritetoCSV("Calculated.csv");
    return out;
}

bool WholisticDewateringCalculator::SetValue(const QString &expression, const double& value)
{
    return system.SetValue(expression,value);
}
