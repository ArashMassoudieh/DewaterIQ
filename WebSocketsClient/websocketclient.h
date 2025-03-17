#ifndef WEBSOCKETCLIENT_H
#define WEBSOCKETCLIENT_H

#include <QObject>
#include <QWebSocket>

class WebSocketClient : public QObject
{
    Q_OBJECT
public:
    explicit WebSocketClient(const QUrl &url, QObject *parent = nullptr);
    void SendTextMessage(const QString &message);
private slots:
    void onConnected();
    void onTextMessageReceived(QString message);
    void onDisconnected();
    void onError(QAbstractSocket::SocketError error);
signals:
    void textMessageRecieved(const QString &message);

private:
    QWebSocket m_webSocket;
    QUrl m_url;
    bool isConnected = false;
};

#endif // WEBSOCKETCLIENT_H


