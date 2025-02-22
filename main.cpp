#include "DewaterIQ.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DewaterIQ w;
    w.show();
    return a.exec();
}
