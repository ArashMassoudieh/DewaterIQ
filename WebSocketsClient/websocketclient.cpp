#include "websocketclient.h"
#include <QDebug>

WebSocketClient::WebSocketClient(const QUrl &url, QObject *parent)
    : QObject(parent), m_url(url)
{
    connect(&m_webSocket, &QWebSocket::connected, this, &WebSocketClient::onConnected);
    connect(&m_webSocket, &QWebSocket::textMessageReceived, this, &WebSocketClient::onTextMessageReceived);
    //connect(&m_webSocket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::errorOccurred), this, &WebSocketClient::onError);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &WebSocketClient::onDisconnected);
    qDebug() << "Attempting to connect to WebSocket server at" << url.toString();
    m_webSocket.open(m_url);

}

void WebSocketClient::onConnected()
{
    qDebug() << "Connected to WebSocket server!";
    m_webSocket.sendTextMessage("Connection Established");
    isConnected = true;
}

void WebSocketClient::SendTextMessage(const QString &message)
{
    if (isConnected)
    {   qDebug() << "Message sent" << message;
        m_webSocket.sendTextMessage(message);
    }
    else
        qDebug() << "Not connected";
}


void WebSocketClient::onTextMessageReceived(QString message)
{
    qDebug() << "Message from server:" << message;
    emit textMessageRecieved(message);
}

void WebSocketClient::onDisconnected()
{
    qDebug() << "Disconnected from WebSocket server!";
    isConnected = false;
}

void WebSocketClient::onError(QAbstractSocket::SocketError error)
{
    qDebug() << "WebSocket error occurred:" << m_webSocket.errorString();
    isConnected = false;
}
