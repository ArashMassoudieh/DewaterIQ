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
    double ExpensePerTon = system.Calculate("(DistributionCosts+TransporationCosts) / (BudgetBasicInputs:value:DryTons/BudgetBasicInputs:value:Dewatered_Cake_TS_percent*100)");
    system.InsertScalar("ExpensePerTon",ExpensePerTon);
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
    AquaArray Total_RRR_OM_Variable_Expenses = system.Calculate(&TotalRRExpenses,"x1*ExpensePerTon");
    AquaArray Total_Labor_Expenses = system.Calculate(&DewateredCakeTSValues,"hsd(x1-BudgetBasicInputs:value:Dewatered_Cake_TS_percent)");


    TotalRRExpenses.AppendColumn("Total_RRnR_OnM_Variable_Expenses",Total_RRR_OM_Variable_Expenses);
    TotalRRExpenses.WritetoCSV("../../RRexpenses.csv");
    TotalRRExpenses.WritetoJson("../../RRexpenses.json");
    AquaTable TotalRRExpensesLoaded;
    TotalRRExpensesLoaded.ReadFromJson("../../RRexpenses.json");
    qDebug()<<"Finished";

}

DewaterIQ::~DewaterIQ()
{}
