#include "DewaterIQ.h"
#include "System.h"

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
    qDebug() << data_single_table.sumKeys("Materials and Supplies");
    qDebug() << data_single_table.sumSubkeys("Stromo Compost");
    qDebug() << data_single_table.sumTotal();
    
    ui.setupUi(this);
}

DewaterIQ::~DewaterIQ()
{}
