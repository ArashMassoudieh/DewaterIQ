#include <QtWidgets/QApplication>
#include <iostream>
#include "wholisticdewateringcalculator.h"
#ifdef USE_WEBSOCKETS
#include "websocketserver.h"
#elif USE_FLASK_TYPE
#include "websocketserver.h"
#endif

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    std::cout<<"Version built, 3/22/2025, 10:49 am"<<std::endl;
#ifdef USE_WEBSOCKETS
    WebSocketServer server(12345); // Start the WebSocket server on port 12345
#elif USE_FLASK_TYPE
    WebSocketServer server(8080); // Start the HttpServer on port 8080
#else
    WholisticDewateringCalculator calculator;
    AquaTable TotalRRExpenses = calculator.PerformCalculation();
    TotalRRExpenses.WritetoCSV("json_output_files/RRexpenses.csv");
    TotalRRExpenses.WritetoJson("json_output_files/RRexpenses.json");
#endif
    a.exec();
}
