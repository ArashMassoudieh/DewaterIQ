#include "DewaterIQ.h"
#include "System.h"
#include "AquaArray.h"
#include "aquatable.h"

DewaterIQ::DewaterIQ(QWidget *parent)
    : QMainWindow(parent)
{
    System system;
    DataStructure BudgetBasicInputs;
    DataStructure TransportationCosts;
    DataStructure DistributionCosts;
    BudgetBasicInputs.readFromJsonFile("../../BudgetBasicInputs.json");
    TransportationCosts.readFromJsonFile("../../TransportationCosts.json");
    DistributionCosts.readFromJsonFile("../../DistributionCosts.json");
    system["BudgetBasicInputs"] = BudgetBasicInputs;
    system["TransporationCosts"] = TransportationCosts;
    system["DistributionCosts"] = DistributionCosts;
    double totalBudget = system.Calculate("(DistributionCosts + TransporationCosts)");
    double BudgetExpensePerTon = system.Calculate("(DistributionCosts+TransporationCosts) / (BudgetBasicInputs:value:DryTons/BudgetBasicInputs:value:Dewatered_Cake_TS_percent*100)");
    double Costat21 = system.Calculate("(BudgetBasicInputs:value:DryTons/BudgetBasicInputs:value:Dewatered_Cake_TS_percent*100)");
    //double MaterialsandSupplies = system.Eval("(DistributionCosts:Materials_and_Supplies) / (BudgetBasicInputs:value:DryTons/BudgetBasicInputs:value:Dewatered_Cake_TS_percent*100)");
    //double MaterialsandSupplies = system.Calculate("( DistributionCosts:Materials_and_Supplies )");
    AquaTable DewateredCakeTSValues;
    DewateredCakeTSValues.AppendSequence(18,24,12,sequencemode::linear);
    qDebug()<<DewateredCakeTSValues.last();
    double drytons = system.Eval("BudgetBasicInputs:value:DryTons");
    AquaArray Wet_Tons = system.Calculate(&DewateredCakeTSValues,"BudgetBasicInputs:value:DryTons/x1*100");
    qDebug()<<Wet_Tons;
    AquaTable TotalRRExpenses;
    TotalRRExpenses.AppendColumn("WetTons",Wet_Tons);
    TotalRRExpenses.WritetoCSV("../../RRexpenses.csv");
    TotalRRExpenses.WritetoJson("../../RRexpenses.json");
    AquaTable TotalRRExpensesLoaded;
    TotalRRExpensesLoaded.ReadFromJson("../../RRexpenses.json");
    qDebug()<<"Finished";

}

DewaterIQ::~DewaterIQ()
{}
