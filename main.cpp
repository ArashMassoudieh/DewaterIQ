#include <QtWidgets/QApplication>
#include <iostream>
#include "wholisticdewateringcalculator.h"
#ifdef USE_WEBSOCKETS
#include "websocketserver.h"
#endif

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

#ifdef USE_WEBSOCKETS
    WebSocketServer server(12345); // Start the WebSocket server on port 12345
#else
    WholisticDewateringCalculator calculator;
    AquaTable TotalRRExpenses = calculator.PerformCalculation();
    TotalRRExpenses.WritetoCSV("json_output_files/RRexpenses.csv");
    TotalRRExpenses.WritetoJson("json_output_files/RRexpenses.json");
#endif
    a.exec();
}
