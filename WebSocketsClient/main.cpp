#include <QCoreApplication>
#include "websocketclient.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    WebSocketClient client(QUrl("ws://ec2-34-215-213-124.us-west-2.compute.amazonaws.com:12345"));
    //WebSocketClient client(QUrl("ws://localhost:12345"));

    return a.exec();
}

