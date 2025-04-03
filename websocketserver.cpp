#include "websocketserver.h"
#include <QDebug>
#include <QTcpServer>

using namespace crow;

#ifdef USE_WEBSOCKETS
WebSocketServer::WebSocketServer(quint16 port, QObject *parent)
    : QObject(parent),
    m_server(new QWebSocketServer(QStringLiteral("Echo Server"),
                                  QWebSocketServer::NonSecureMode, this))
{
    if (m_server->listen(QHostAddress::Any, port)) {
        qDebug() << "WebSocket server listening on port" << port;
        connect(m_server, &QWebSocketServer::newConnection,
                this, &WebSocketServer::onNewConnection);
    }

    if (!calculator.BuildSystem("../../json_input_files/Instructions.json"))
    {   QString fullpath = calculator.findFileRecursive(".","Instructions.json");
        if (!fullpath.isEmpty())
        {
            calculator.BuildSystem(fullpath);
        }
        else
        {
            qDebug()<<"The instructions was failed to be loaded";
        }
    }
}
#endif

#ifdef USE_FLASK_TYPE
WebSocketServer::WebSocketServer(quint16 port, QObject *parent): QObject(parent)
{
    CROW_ROUTE(app, "/calculate").methods("POST"_method)
    ([this](const crow::request& req) {
        return StatementReceived(req);
    });

    app.port(port).multithreaded().run();
}

crow::response WebSocketServer::StatementReceived(const crow::request& req)
{
    QByteArray jsonBytes = QByteArray::fromStdString(req.body);
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonBytes, &parseError);

    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        return crow::response(400, "Invalid JSON");
    }

    if (!calculator.BuildSystem("../../json_input_files/Instructions.json"))
        calculator.BuildSystem("json_input_files/Instructions.json");

    QJsonObject obj = doc.object();
    for (auto it = obj.begin(); it != obj.end(); ++it) {
        QString key = it.key();
        QJsonValue value = it.value();
        qDebug() << "Key:" << key << ", Value:" << value;
        calculator.SetValue(key, value.toDouble());
    }

    QJsonDocument responseDoc = calculator.PerformCalculation().toJson();
    QByteArray responseJson = responseDoc.toJson();

    crow::response res;
    res.code = 200;
    res.set_header("Content-Type", "application/json");
    res.body = responseJson.toStdString();
    return res;
}
#endif

WebSocketServer::~WebSocketServer()
{
    m_server->close();
    qDeleteAll(m_clients);
}

void WebSocketServer::onNewConnection()
{
    QWebSocket *socket = m_server->nextPendingConnection();

    connect(socket, &QWebSocket::textMessageReceived,
            this, &WebSocketServer::onTextMessageReceived);
    connect(socket, &QWebSocket::disconnected,
            this, &WebSocketServer::onSocketDisconnected);

    m_clients.append(socket);
    qDebug() << "New client connected!";


}

void WebSocketServer::onTextMessageReceived(QString message)
{
    QWebSocket *senderSocket = qobject_cast<QWebSocket *>(sender());
    if (senderSocket) {
        qDebug() << "Received message from client:" << message;
        QStringList instructions = message.split(",");

        if (!calculator.BuildSystem("../../json_input_files/Instructions.json"))
            calculator.BuildSystem("json_input_files/Instructions.json");


        for (int i=0; i<instructions.count(); i++)
        {
            if (instructions[i].contains("="))
            {
                calculator.SetValue(instructions[i].split("=")[0],instructions[i].split("=")[1].toDouble());
                qDebug()<<"Value of "<<instructions[i].split("=")[0] << " was set to " << instructions[i].split("=")[1];
            }
            if (instructions[i] == "Perform Calculations")
            {

                QJsonDocument json = calculator.PerformCalculation().toJson();
                QString jsonString = QString::fromUtf8(json.toJson(QJsonDocument::Compact));
                sendMessageToClient(senderSocket, jsonString);
            }
            else
            {
                QString response = "Server Response: Received your message - " + message;
                sendMessageToClient(senderSocket, response);
            }

        }

    }
}

void WebSocketServer::onSocketDisconnected()
{
    QWebSocket *socket = qobject_cast<QWebSocket *>(sender());
    if (socket) {
        m_clients.removeAll(socket);
        socket->deleteLater();
        qDebug() << "Client disconnected!";
    }
}


void WebSocketServer::sendMessageToClient(QWebSocket *client, const QString &message)
{
    if (client) {
        client->sendTextMessage(message);
        qDebug() << "Sent message to client:" << message;
    }
}


