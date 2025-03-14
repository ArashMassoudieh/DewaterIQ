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
    DataStructure StepFunctionDistributionExpenses;
    DataStructure StepFunctionTransportationExpenses;
    DataStructure DiscreteAnnualCostswithDecreaseTS;
    DataStructure CakeTSvsPolyDoseTrend;
    BudgetBasicInputs.readFromJsonFile("../../BudgetBasicInputs.json");
    TransportationCosts.readFromJsonFile("../../TransportationCosts.json");
    DistributionCosts.readFromJsonFile("../../DistributionCosts.json");
    StepFunctionDistributionExpenses.readFromJsonFile("../../StepFunctionDistributionCosts.json");
    StepFunctionTransportationExpenses.readFromJsonFile("../../StepFunctionTransportationCosts.json");
    DiscreteAnnualCostswithDecreaseTS.readFromJsonFile("../../DiscreteAnnualCostswithDecreaseTS.json");
    CakeTSvsPolyDoseTrend.readFromJsonFile("../../Cake_TS_vs_Poly_Dose_Trend.json");
    system["BudgetBasicInputs"] = BudgetBasicInputs;
    system["TransporationCosts"] = TransportationCosts;
    system["DistributionCosts"] = DistributionCosts;
    system["StepFunctionTransportationExpenses"] = StepFunctionTransportationExpenses;
    system["StepFunctionDistributionExpenses"] = StepFunctionDistributionExpenses;
    system["DiscreteAnnualCostswithDecreaseTS"] = DiscreteAnnualCostswithDecreaseTS;
    system["CakeTSvsPolyDoseTrend"] = CakeTSvsPolyDoseTrend;


    double ExpensePerTon = system.Calculate("(DistributionCosts+TransporationCosts) / (BudgetBasicInputs:value:DryTons/BudgetBasicInputs:value:Dewatered_Cake_TS_percent*100)");
    system.InsertScalar("ExpensePerTon",ExpensePerTon);

    AquaTable Expenses;
    Expenses.AppendSequence("Dewatered_Cake_TS_Percent",18,24,12,sequencemode::linear);

    AquaArray Wet_Tons = system.Calculate(&Expenses,"BudgetBasicInputs:value:DryTons/Dewatered_Cake_TS_Percent*100");
    qDebug()<<Wet_Tons;

    Expenses.AppendColumn("WetTons",Wet_Tons);
    AquaArray Total_RRR_OM_Variable_Expenses = system.Calculate(&Expenses,"WetTons*ExpensePerTon");

    AquaArray OperatorCost;
    AquaArray MechanicCost;

    OperatorCost.append(system.Calculate(&Expenses,"hsd(BudgetBasicInputs:value:Dewatered_Cake_TS_percent-Dewatered_Cake_TS_Percent)*floor((BudgetBasicInputs:value:Dewatered_Cake_TS_percent-Dewatered_Cake_TS_Percent)/(100*DiscreteAnnualCostswithDecreaseTS:Percent_Decrease:Each_New_Operator))*DiscreteAnnualCostswithDecreaseTS:Amount:Each_New_Operator"));
    MechanicCost.append(system.Calculate(&Expenses,"hsd(BudgetBasicInputs:value:Dewatered_Cake_TS_percent-Dewatered_Cake_TS_Percent)*floor((BudgetBasicInputs:value:Dewatered_Cake_TS_percent-Dewatered_Cake_TS_Percent)/(100*DiscreteAnnualCostswithDecreaseTS:Percent_Decrease:Each_New_Mechanic))*DiscreteAnnualCostswithDecreaseTS:Amount:Each_New_Mechanic"));

    Expenses.AppendColumn("OperatorExpense",OperatorCost);
    Expenses.AppendColumn("MechanicExpense",MechanicCost);

    AquaArray Base_Labor_Expenses = system.Calculate(&Expenses,"(StepFunctionTransportationExpenses+StepFunctionDistributionExpenses)");
    Expenses.AppendColumn("BaseExpense",Base_Labor_Expenses);
    AquaArray Total_Labor_Expenses = system.Calculate(&Expenses,"OperatorExpense + MechanicExpense + BaseExpense");
    Expenses.AppendColumn("TotalLaborExpense",Total_Labor_Expenses);

    AquaArray AdditionalExpense = system.Calculate(&Expenses, "hsd(BudgetBasicInputs:value:Dewatered_Cake_TS_percent-Dewatered_Cake_TS_Percent)*floor((BudgetBasicInputs:value:Dewatered_Cake_TS_percent-Dewatered_Cake_TS_Percent)/(100*DiscreteAnnualCostswithDecreaseTS:Percent_Decrease:Tractor_Trailer_Combination))*DiscreteAnnualCostswithDecreaseTS:Amount:Tractor_Trailer_Combination");

    Expenses.AppendColumn("Total_RRnR_OnM_Variable_Expenses",Total_RRR_OM_Variable_Expenses);
    Expenses.AppendColumn("Additional_Expenses",AdditionalExpense);
    AquaArray TotalRRnRExpenses = system.Calculate(&Expenses,"TotalLaborExpense+Total_RRnR_OnM_Variable_Expenses+Additional_Expenses");
    Expenses.AppendColumn("Total_RRR_Variable_Expenses",TotalRRnRExpenses);

    AquaArray DryPolymerUse = system.Calculate(&Expenses, "exp((Dewatered_Cake_TS_Percent-CakeTSvsPolyDoseTrend:Intercept:Dry)/CakeTSvsPolyDoseTrend:Slope:Dry)*Dewatered_Cake_TS_Percent/100*WetTons");
    Expenses.AppendColumn("DryPolymerUse",DryPolymerUse);

    AquaArray EmulsionPolymerUse = system.Calculate(&Expenses, "exp((Dewatered_Cake_TS_Percent-CakeTSvsPolyDoseTrend:Intercept:Emulsion)/CakeTSvsPolyDoseTrend:Slope:Emulsion)*Dewatered_Cake_TS_Percent/100*WetTons");
    Expenses.AppendColumn("EmulsionPolymerUse",EmulsionPolymerUse);

    AquaArray DryPolymerCost = system.Calculate(&Expenses,"DryPolymerUse*BudgetBasicInputs:Polymer_Unit_Price_Dry+33.2*DryPolymerUse*0.00278");
    Expenses.AppendColumn("DryPolymerCost",DryPolymerCost);

    AquaArray EmulsionPolymerCost = system.Calculate(&Expenses,"EmulsionPolymerUse*BudgetBasicInputs:Polymer_Unit_Price_Emulsion");
    Expenses.AppendColumn("EmulsionPolymerCost",EmulsionPolymerCost);

    AquaArray TotalDryPolymerCost = system.Calculate(&Expenses, "DryPolymerCost+Total_RRR_Variable_Expenses");
    Expenses.AppendColumn("TotalDryPolymerCost",TotalDryPolymerCost);

    AquaArray TotalEmulsionPolymerCost = system.Calculate(&Expenses, "EmulsionPolymerCost+Total_RRR_Variable_Expenses");
    Expenses.AppendColumn("TotalEmulsionPolymerCost",TotalEmulsionPolymerCost);

    Expenses.WritetoCSV("../../RRexpenses.csv");
    Expenses.WritetoJson("../../RRexpenses.json");
    AquaTable ExpensesLoaded;
    ExpensesLoaded.ReadFromJson("../../RRexpenses.json");
    qDebug()<<"Finished";

}

DewaterIQ::~DewaterIQ()
{}
