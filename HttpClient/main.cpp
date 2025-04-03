#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QNetworkAccessManager *manager = new QNetworkAccessManager(&app);

    // Create JSON payload
    QJsonObject json;

    json["BudgetBasicInputs:value:Polymer_Unit_Price_Emulsion"] = 2;
    json["BudgetBasicInputs:value:Polymer_Unit_Price_Dry"] = 2;
    QJsonDocument doc(json);
    QByteArray jsonData = doc.toJson();

    // Prepare the request
    QNetworkRequest request(QUrl("http://localhost:8080/calculate"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Send the POST request
    QNetworkReply *reply = manager->post(request, jsonData);

    QObject::connect(reply, &QNetworkReply::finished, [&]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);
            QJsonObject responseObj = responseDoc.object();

            qDebug() << "Response JSON:" << responseObj;
        } else {
            qWarning() << "Request failed:" << reply->errorString();
        }

        reply->deleteLater();
        app.quit();
    });

    // Exit if it takes too long
    QTimer::singleShot(5000, &app, &QCoreApplication::quit); // timeout safeguard

    return app.exec();
}
