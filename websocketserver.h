#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H

#include <QObject>
#include <QWebSocketServer>
#include <QWebSocket>
#include "wholisticdewateringcalculator.h"
#include <QHttpServerRequest>
#include <QHttpServerResponse>
#include <QHttpServer>



class WebSocketServer : public QObject
{
    Q_OBJECT
public:
    explicit WebSocketServer(quint16 port, QObject *parent = nullptr);
    ~WebSocketServer();
    QHttpServerResponse StatementRecieved(const QHttpServerRequest &request);
private slots:
    void onNewConnection();
    void onTextMessageReceived(QString message);
    void onSocketDisconnected();
    void sendMessageToClient(QWebSocket *client, const QString &message);



private:
    QWebSocketServer *m_server;
    QList<QWebSocket *> m_clients;
    WholisticDewateringCalculator calculator;
    QHttpServer server;

};

#endif // WEBSOCKETSERVER_H
