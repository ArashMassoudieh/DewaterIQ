#include "System.h"
#include <QFile>

QJsonObject System::toJsonObject() const
{
    QJsonObject rootObj;
        
    for (QMap<QString, DataStructure>::const_iterator it = begin(); it != end(); ++it) {
        rootObj.insert(it.key(), it.value().toJsonObject());
    }
        
    return rootObj;
}


bool System::FromJsonObject(const QJsonObject &jsonobject)
{
    bool outcome = true; 
    this->clear(); // Clear existing data
    for (const QString& key1 : jsonobject.keys()) {
            DataStructure item;
            outcome = outcome && item.FromJson(jsonobject[key1].toObject());
            (*this)[key1] = item;
        }
        return outcome; 
}

bool System::WritetoJsonFile (const QString &filename) const
{
    QJsonDocument jsonDoc = QJsonDocument(this->toJsonObject());
    QFile file(filename);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file for writing:" << filename;
        return false;
    }

    file.write(jsonDoc.toJson(QJsonDocument::Indented));
    file.close();
    return true;
}

bool System::readFromJsonFile(const QString& filename) {
    QFile file(filename);

    // Open the file
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file:" << filename;
        return false;
    }

    // Read the file content
    QByteArray fileData = file.readAll();
    file.close();

    // Parse the JSON content
    QJsonDocument jsonDoc = QJsonDocument::fromJson(fileData);
    if (jsonDoc.isNull() || !jsonDoc.isObject()) {
        qDebug() << "Invalid JSON format in file:" << filename;
        return false;
    }

    QJsonObject rootObj = jsonDoc.object();
    return FromJsonObject(rootObj);

}
