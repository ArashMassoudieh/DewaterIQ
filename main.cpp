#include <QtWidgets/QApplication>
#include <AquaArray.h>
#include <DataStructure.h>
#include <aquatable.h>
#include <System.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    System system;
    DataStructure BudgetBasicInputs;
    DataStructure TransportationCosts;
    DataStructure DistributionCosts;
    DataStructure StepFunctionDistributionExpenses;
    DataStructure StepFunctionTransportationExpenses;
    DataStructure DiscreteAnnualCostswithDecreaseTS;
    if (!BudgetBasicInputs.readFromJsonFile("../../BudgetBasicInputs.json"))
        BudgetBasicInputs.readFromJsonFile("BudgetBasicInputs.json");
    if (!TransportationCosts.readFromJsonFile("../../TransportationCosts.json"))
        TransportationCosts.readFromJsonFile("TransportationCosts.json");
    if (!DistributionCosts.readFromJsonFile("../../DistributionCosts.json"))
        DistributionCosts.readFromJsonFile("DistributionCosts.json");
    if (!StepFunctionDistributionExpenses.readFromJsonFile("../../StepFunctionDistributionCosts.json"))
         StepFunctionDistributionExpenses.readFromJsonFile("StepFunctionDistributionCosts.json");
    if (!StepFunctionTransportationExpenses.readFromJsonFile("../../StepFunctionTransportationCosts.json"))
        StepFunctionTransportationExpenses.readFromJsonFile("StepFunctionTransportationCosts.json");
    if (!DiscreteAnnualCostswithDecreaseTS.readFromJsonFile("../../DiscreteAnnualCostswithDecreaseTS.json"))
        DiscreteAnnualCostswithDecreaseTS.readFromJsonFile("DiscreteAnnualCostswithDecreaseTS.json");
    system["BudgetBasicInputs"] = BudgetBasicInputs;
    system["TransporationCosts"] = TransportationCosts;
    system["DistributionCosts"] = DistributionCosts;
    system["StepFunctionTransportationExpenses"] = StepFunctionTransportationExpenses;
    system["StepFunctionDistributionExpenses"] = StepFunctionDistributionExpenses;
    system["DiscreteAnnualCostswithDecreaseTS"] = DiscreteAnnualCostswithDecreaseTS;

    double TotalStepFunctionExpenses = system.Calculate("StepFunctionTransportationExpenses+StepFunctionDistributionExpenses");
    double totalBudget = system.Calculate("(DistributionCosts + TransporationCosts)");
    double ExpensePerTon = system.Calculate("(DistributionCosts+TransporationCosts) / (BudgetBasicInputs:value:DryTons/BudgetBasicInputs:value:Dewatered_Cake_TS_percent*100)");
    system.InsertScalar("ExpensePerTon",ExpensePerTon);

    AquaTable TotalRRExpenses;
    TotalRRExpenses.AppendSequence("Dewatered_Cake_TS_Percent",18,24,12,sequencemode::linear);

    double drytons = system.Eval("BudgetBasicInputs:value:DryTons");
    AquaArray Wet_Tons = system.Calculate(&TotalRRExpenses,"BudgetBasicInputs:value:DryTons/x1*100");
    qDebug()<<Wet_Tons;

    TotalRRExpenses.AppendColumn("WetTons",Wet_Tons);
    AquaArray Total_RRR_OM_Variable_Expenses = system.Calculate(&TotalRRExpenses,"x2*ExpensePerTon");

    AquaArray OperatorTerm;
    AquaArray MechanicTerm;

    OperatorTerm.append(system.Calculate(&TotalRRExpenses,"hsd(BudgetBasicInputs:value:Dewatered_Cake_TS_percent-x1)*floor((BudgetBasicInputs:value:Dewatered_Cake_TS_percent-x1)/(100*DiscreteAnnualCostswithDecreaseTS:Percent_Decrease:Each_New_Operator))*DiscreteAnnualCostswithDecreaseTS:Amount:Each_New_Operator"));
    MechanicTerm.append(system.Calculate(&TotalRRExpenses,"hsd(BudgetBasicInputs:value:Dewatered_Cake_TS_percent-x1)*floor((BudgetBasicInputs:value:Dewatered_Cake_TS_percent-x1)/(100*DiscreteAnnualCostswithDecreaseTS:Percent_Decrease:Each_New_Mechanic))*DiscreteAnnualCostswithDecreaseTS:Amount:Each_New_Mechanic"));

    TotalRRExpenses.AppendColumn("OperatorExpense",OperatorTerm);
    TotalRRExpenses.AppendColumn("MechanicExpense",MechanicTerm);

    AquaArray Base_Labor_Expenses = system.Calculate(&TotalRRExpenses,"(StepFunctionTransportationExpenses+StepFunctionDistributionExpenses)");
    TotalRRExpenses.AppendColumn("BaseExpense",Base_Labor_Expenses);
    AquaArray Total_Labor_Expenses = system.Calculate(&TotalRRExpenses,"x3 + x4 + x5");
    TotalRRExpenses.AppendColumn("TotalLaborExpense",Total_Labor_Expenses);

    AquaArray AdditionalExpense = system.Calculate(&TotalRRExpenses, "hsd(BudgetBasicInputs:value:Dewatered_Cake_TS_percent-x1)*floor((BudgetBasicInputs:value:Dewatered_Cake_TS_percent-x1)/(100*DiscreteAnnualCostswithDecreaseTS:Percent_Decrease:Tractor_Trailer_Combination))*DiscreteAnnualCostswithDecreaseTS:Amount:Tractor_Trailer_Combination");

    TotalRRExpenses.AppendColumn("Total_RRnR_OnM_Variable_Expenses",Total_RRR_OM_Variable_Expenses);
    TotalRRExpenses.AppendColumn("Additional_Expenses",AdditionalExpense);
    AquaArray TotalExpenses = system.Calculate(&TotalRRExpenses,"x6+x7+x8");
    TotalRRExpenses.AppendColumn("Total_RRR_Variable_Expenses",TotalExpenses);
    TotalRRExpenses.WritetoCSV("RRexpenses.csv");
    TotalRRExpenses.WritetoJson("RRexpenses.json");
    AquaTable TotalRRExpensesLoaded;
    TotalRRExpensesLoaded.ReadFromJson("RRexpenses.json");
    qDebug()<<"Finished";

}
