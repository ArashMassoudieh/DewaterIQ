#include "DataStructure.h"
#include <stdexcept>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDebug>
#include "ExpressionCalculator.h"

double DataStructure::Eval(const QString& expression) const {
    QStringList keys = expression.split(":"); // Split "x.y" into ["x", "y"]
    if (keys.size() != 2) {
        throw std::invalid_argument("Invalid format. Use 'key.subkey' format.");
    }

    QString key1 = keys[0];
    QString key2 = keys[1];

    if (!this->contains(key1)) {
        throw std::out_of_range(QString("Key '%1' not found").arg(key1).toStdString());
    }
    if (!this->value(key1).contains(key2)) {
        throw std::out_of_range(QString("Subkey '%1' not found in '%2'").arg(key2, key1).toStdString());
    }

    return this->value(key1).value(key2); // Access data: at("x").at("y")
}; 

bool DataStructure::readFromJsonFile(const QString& filename) {
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
    return FromJson(rootObj);
    
}

bool DataStructure::FromJson(const QJsonObject& rootObj)
{
    // Convert JSON object to DataStructure
    int level = SetLevels(rootObj);
    this->clear(); // Clear existing data
    if (levels == 2)
    {
        for (const QString& key1 : rootObj.keys()) {
            QJsonObject subObj = rootObj[key1].toObject();
            QMap<QString, double> innerMap;

            for (const QString& key2 : subObj.keys()) {
                innerMap[key2] = subObj[key2].toDouble();
            }

            (*this)[key1] = innerMap;
        }
    }
    else if (levels == 1)
    {
        QJsonObject subObj = rootObj;
        QMap<QString, double> innerMap;

        for (const QString& key2 : subObj.keys()) {
            innerMap[key2] = subObj[key2].toDouble();
        }

        (*this)["root"] = innerMap;
    }
    else
        return false; 
    return true; 
}

QJsonObject DataStructure::toJsonObject() const
{
    QJsonObject rootObj;

    for (auto it1 = this->begin(); it1 != this->end(); ++it1) {
        QJsonObject subObj;
        for (auto it2 = it1.value().begin(); it2 != it1.value().end(); ++it2) {
            subObj.insert(it2.key(), it2.value());
        }
        rootObj.insert(it1.key(), subObj);
    }
    return rootObj;
}

QJsonDocument DataStructure::toJsonDocument() const {

    return QJsonDocument(toJsonObject());
}

double DataStructure::sumSubkeys(const QString& key) const {
    if (!this->contains(key)) {
        qDebug() << "Key not found:" << key;
        return 0;
    }

    double sum = 0;
    QMap<QString, double> subMap = this->value(key);
    for (const double& value : subMap) {
        sum += value;
    }

    return sum;
}

// Sum of all values across keys (First level)
double DataStructure::sumKeys(const QString &key) const {
    double totalSum = 0;
    for (const QMap<QString, double>& subMap : this->values()) {
        if (subMap.contains(key))
        {
            totalSum += subMap[key];
        }
    }

    return totalSum;
}

double DataStructure::sumTotal() const {
    double totalSum = 0;
    for (const QMap<QString, double>& Map : this->values()) {
        for (const double &subMap : Map.values())
        {
            totalSum += subMap;
        }
    }

    return totalSum;
}

// Write DataStructure to JSON file
bool DataStructure::writeToJsonFile(const QString& filename) const {
    QJsonDocument jsonDoc = toJsonDocument();
    QFile file(filename);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file for writing:" << filename;
        return false;
    }

    file.write(jsonDoc.toJson(QJsonDocument::Indented));
    file.close();
    return true;
}

int DataStructure::SetLevels(const QJsonObject& jsonobject)
{
    if (jsonobject.count() == 0)
        return 0; 
    else
    {
        if (jsonobject.begin()->isObject())
            return 2;
        else
            return 1; 
    }
}

double DataStructure::Calculate(const QString& expression)
{
    ExpressionCalculator exp(expression);
    return exp.calc(this);
}

bool DataStructure::contains(const QString& variable) const
{
    QStringList terms = variable.split(":");
    if (terms.count() == 0) return false;
    bool outcome = true;
    outcome = outcome && QMap<QString, QMap<QString, double>>::contains(terms[0]);
    if (terms.count() == 2)
        outcome = outcome && value(terms[0]).contains(terms[1]);
    
    return outcome;
}


