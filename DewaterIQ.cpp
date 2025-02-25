#include "DewaterIQ.h"
#include "System.h"
#include "AquaArray.h"

DewaterIQ::DewaterIQ(QWidget *parent)
    : QMainWindow(parent)
{
    DataStructure data_single_table; 
    data_single_table.readFromJsonFile("C:/Projects/DewaterIQ/data.json");
    System system; 
    system["item1"] = data_single_table;
    system["item2"] = data_single_table;
    data_single_table.writeToJsonFile("C:/Projects/DewaterIQ/table.json");
    system.WritetoJsonFile("C:/Projects/DewaterIQ/system.json");
    System system1;
    system1.readFromJsonFile("C:/Projects/DewaterIQ/system.json");
    //qDebug() << data_single_table.sumKeys("Materials_and_Supplies");
    //qDebug() << data_single_table.sumSubkeys("Stromo_Compost");
    //qDebug() << data_single_table.sumTotal();
    //qDebug() << data_single_table.Eval("Stromo_Compost:Materials_and_Supplies");
    //qDebug() << data_single_table.Calculate("Stromo_Compost:Materials_and_Supplies*Metrogro_Farm_Ag:Tires_and_Tubes");
    //qDebug() << system.Calculate("item1:Stromo_Compost:Materials_and_Supplies*item2:Metrogro_Farm_Ag:Tires_and_Tubes");
    //qDebug() << data_single_table.Eval("Materials_and_Supplies");
    //qDebug() << data_single_table.Calculate("Materials_and_Supplies+Metrogro_Farm_Ag");
    QVector<AquaArray> X(10); 
    for (int i = 0; i < 10; i++)
    {
        X[i].append(i);
    }
    qDebug() << data_single_table.Calculate(&X, "x1*Metrogro_Farm_Ag");
    ui.setupUi(this);
}

DewaterIQ::~DewaterIQ()
{}
