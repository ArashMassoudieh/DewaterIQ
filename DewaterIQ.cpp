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
    system.WritetoJsonFile("C:/Projects/DewaterIQ/system.json");
    System system1;
    system1.readFromJsonFile("C:/Projects/DewaterIQ/system.json");

    ui.setupUi(this);
}

DewaterIQ::~DewaterIQ()
{}
