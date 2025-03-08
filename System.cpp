#include "System.h"
#include <QFile>
#include "ExpressionCalculator.h"

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

double System::Calculate(const QString& expression) const
{
    ExpressionCalculator exp(expression);
    return exp.calc(this);
}

AquaArray System::Calculate(const QVector<AquaArray>* array, const QString& expression)
{
    ExpressionCalculator exp(expression);
    return exp.calc(array, this);
}

bool System::contains(const QString& variable) const
{
    QStringList terms = variable.split(":");
    if (terms.count() == 0) return false;
    bool outcome = true; 
    outcome = outcome && QMap<QString, DataStructure>::contains(terms[0]);
    if (terms.count() == 1)
    {
        outcome = outcome || begin()->contains(terms[0]);
    }
    if (terms.count() == 2)
        outcome = outcome && (value(terms[0]).contains(terms[1]) || begin()->contains(variable));
    else if (terms.count() == 3)
        outcome = outcome && (value(terms[0]).contains(terms[1]) && begin()->value(terms[1]).contains(terms[2]) );

    return outcome; 
}


int System::level(const QString& item) const // return zero for the top level and one for bottom level; 
{
    if (count() == 0)
        return -1;
    if (QMap<QString, DataStructure>::contains(item))
        return 0;
    if (begin()->contains(item))
        return begin()->level(item)+1;
    return -1;
}
