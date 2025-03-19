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
        //qDebug() << "Invalid JSON format in file:" << filename;
        return false;
    }

    QJsonObject rootObj = jsonDoc.object();
    QJsonArray Tables = rootObj["Tables"].toArray();
    qDebug()<<Tables;
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
    //qDebug()<<system;
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
    double ExpensePerTon = system.Calculate("(DistributionCosts+TransporationCosts) / (BudgetBasicInputs:value:DryTons/BudgetBasicInputs:value:Dewatered_Cake_TS_percent*100)");
    system.InsertScalar("ExpensePerTon",ExpensePerTon);

    AquaTable TotalRRExpenses;
    TotalRRExpenses.AppendSequence("Dewatered_Cake_TS_Percent",18,24,12,sequencemode::linear);

    double drytons = system.Eval("BudgetBasicInputs:value:DryTons");
    AquaArray Wet_Tons = system.Calculate(&TotalRRExpenses,"BudgetBasicInputs:value:DryTons/Dewatered_Cake_TS_Percent*100");
    ////qDebug()<<Wet_Tons;

    TotalRRExpenses.AppendColumn("WetTons",Wet_Tons);
    AquaArray Total_RRR_OM_Variable_Expenses = system.Calculate(&TotalRRExpenses,"WetTons*ExpensePerTon");

    AquaArray OperatorTerm;
    AquaArray MechanicTerm;

    OperatorTerm.append(system.Calculate(&TotalRRExpenses,"hsd(BudgetBasicInputs:value:Dewatered_Cake_TS_percent-Dewatered_Cake_TS_Percent)*floor((BudgetBasicInputs:value:Dewatered_Cake_TS_percent-Dewatered_Cake_TS_Percent)/(100*DiscreteAnnualCostswithDecreaseTS:Percent_Decrease:Each_New_Operator))*DiscreteAnnualCostswithDecreaseTS:Amount:Each_New_Operator"));
    MechanicTerm.append(system.Calculate(&TotalRRExpenses,"hsd(BudgetBasicInputs:value:Dewatered_Cake_TS_percent-Dewatered_Cake_TS_Percent)*floor((BudgetBasicInputs:value:Dewatered_Cake_TS_percent-Dewatered_Cake_TS_Percent)/(100*DiscreteAnnualCostswithDecreaseTS:Percent_Decrease:Each_New_Mechanic))*DiscreteAnnualCostswithDecreaseTS:Amount:Each_New_Mechanic"));

    TotalRRExpenses.AppendColumn("OperatorExpense",OperatorTerm);
    TotalRRExpenses.AppendColumn("MechanicExpense",MechanicTerm);

    AquaArray Base_Labor_Expenses = system.Calculate(&TotalRRExpenses,"(StepFunctionTransportationExpenses+StepFunctionDistributionExpenses)");
    TotalRRExpenses.AppendColumn("BaseExpense",Base_Labor_Expenses);
    AquaArray Total_Labor_Expenses = system.Calculate(&TotalRRExpenses,"OperatorExpense + MechanicExpense + BaseExpense");
    TotalRRExpenses.AppendColumn("TotalLaborExpense",Total_Labor_Expenses);

    AquaArray AdditionalExpense = system.Calculate(&TotalRRExpenses, "hsd(BudgetBasicInputs:value:Dewatered_Cake_TS_percent-Dewatered_Cake_TS_Percent)*floor((BudgetBasicInputs:value:Dewatered_Cake_TS_percent-Dewatered_Cake_TS_Percent)/(100*DiscreteAnnualCostswithDecreaseTS:Percent_Decrease:Tractor_Trailer_Combination))*DiscreteAnnualCostswithDecreaseTS:Amount:Tractor_Trailer_Combination");

    TotalRRExpenses.AppendColumn("Total_RRnR_OnM_Variable_Expenses",Total_RRR_OM_Variable_Expenses);
    TotalRRExpenses.AppendColumn("Additional_Expenses",AdditionalExpense);
    AquaArray Total_RRR_Variable_Expenses = system.Calculate(&TotalRRExpenses,"TotalLaborExpense+Total_RRnR_OnM_Variable_Expenses+Additional_Expenses");
    TotalRRExpenses.AppendColumn("Total_RRR_Variable_Expenses",Total_RRR_Variable_Expenses);

    double intercept = system.Eval("CakeTSvsPolyDoseTrend:Intercept:Dry");
    AquaArray Dry_Polymer_Use = system.Calculate(&TotalRRExpenses,"exp((Dewatered_Cake_TS_Percent-CakeTSvsPolyDoseTrend:Intercept:Dry)/CakeTSvsPolyDoseTrend:Slope:Dry)*(Dewatered_Cake_TS_Percent/100*WetTons)");
    TotalRRExpenses.AppendColumn("Dry_Polymer_Use",Dry_Polymer_Use);

    AquaArray Emulsion_Polymer_Use = system.Calculate(&TotalRRExpenses,"exp((Dewatered_Cake_TS_Percent-CakeTSvsPolyDoseTrend:Intercept:Emulsion)/CakeTSvsPolyDoseTrend:Slope:Emulsion)*(Dewatered_Cake_TS_Percent/100*WetTons)");
    TotalRRExpenses.AppendColumn("Emulsion_Polymer_Use",Emulsion_Polymer_Use);

    AquaArray Dry_Polymer_Cost = system.Calculate(&TotalRRExpenses,"Dry_Polymer_Use*BudgetBasicInputs:value:Polymer_Unit_Price_Dry+0.092296*Dry_Polymer_Use");
    TotalRRExpenses.AppendColumn("Dry_Polymer_Cost",Dry_Polymer_Cost);

    AquaArray Emulsion_Polymer_Cost = system.Calculate(&TotalRRExpenses,"Emulsion_Polymer_Use*BudgetBasicInputs:value:Polymer_Unit_Price_Emulsion");
    TotalRRExpenses.AppendColumn("Emulsion_Polymer_Cost",Emulsion_Polymer_Cost);

    AquaArray Total_Cost_Dry = system.Calculate(&TotalRRExpenses,"Dry_Polymer_Cost+Total_RRR_Variable_Expenses");
    TotalRRExpenses.AppendColumn("Total_Cost_Dry",Total_Cost_Dry);

    AquaArray Total_Cost_Emulsion = system.Calculate(&TotalRRExpenses,"Emulsion_Polymer_Cost+Total_RRR_Variable_Expenses");
    TotalRRExpenses.AppendColumn("Total_Cost_Emulsion",Total_Cost_Emulsion);

    std::cout<<"Finished"<<std::endl;
    return TotalRRExpenses;
}

bool WholisticDewateringCalculator::SetValue(const QString &expression, const double& value)
{
    return system.SetValue(expression,value);
}
