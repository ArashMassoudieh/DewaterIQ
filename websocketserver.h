#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H

#include <QObject>
#include <QWebSocketServer>
#include <QWebSocket>
#include "wholisticdewateringcalculator.h"

#ifdef signals
#undef signals
#endif
#include "crow.h"




class WebSocketServer : public QObject
{
    Q_OBJECT
public:
    explicit WebSocketServer(quint16 port, QObject *parent = nullptr);
    ~WebSocketServer();
    crow::response StatementReceived(const crow::request& req);
private slots:
    void onNewConnection();
    void onTextMessageReceived(QString message);
    void onSocketDisconnected();
    void sendMessageToClient(QWebSocket *client, const QString &message);



private:
    QWebSocketServer *m_server;
    QList<QWebSocket *> m_clients;
    WholisticDewateringCalculator calculator;
    crow::SimpleApp app;

};

#endif // WEBSOCKETSERVER_H
