#include "DataStructure.h"
#include <stdexcept>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDebug>
#include "ExpressionCalculator.h"
#include "AquaArray.h"

double DataStructure::Eval(const QString& expression) const {

    if (expression.isEmpty())
    {
        return sumTotal();
    }

    QStringList keys = expression.split(":"); // Split "x.y" into ["x", "y"]
    if (keys.size() > 2) {
        throw std::invalid_argument("Invalid format. Use 'key.subkey' format.");
    }



    if (keys.size() == 1)
    {
        int itemlevel = level(keys[0]);
        if (itemlevel == 0)
            return sumSubkeys(keys[0]);
        else
            return sumKeys(keys[0]);
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
        //qDebug() << "Failed to open file:" << filename;
        return false;
    }

    // Read the file content
    QByteArray fileData = file.readAll();
    file.close();

    // Parse the JSON content
    QJsonDocument jsonDoc = QJsonDocument::fromJson(fileData);
    if (jsonDoc.isNull() || !jsonDoc.isObject()) {
        //qDebug() << "Invalid JSON format in file:" << filename;
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
        //qDebug() << "Key not found:" << key;
        return 0;
    }

    double sum = 0;
    QMap<QString, double> subMap = this->value(key);
    for (const double& value : subMap) {
        sum += value;
    }

    return sum;
}

int DataStructure::level(const QString& item) const // return zero for the top level and one for bottom level; 
{
    if (count() == 0)
        return -1; 
    if (QMap<QString, QMap<QString, double>>::contains(item))
        return 0; 
    if (begin()->contains(item))
        return 1; 
    return -1; 
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
        //qDebug() << "Failed to open file for writing:" << filename;
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

double DataStructure::Calculate(const AquaArray *array, const QString& expression)
{
    ExpressionCalculator exp(expression);
    return exp.calc(array,this);
}

AquaArray DataStructure::Calculate(const AquaTable* array, const QString& expression)
{
    ExpressionCalculator exp(expression);
    return exp.calc(array, this);
}


bool DataStructure::contains(const QString& variable) const
{
    QStringList terms = variable.split(":");
    if (terms.count() == 0) return false;
    bool outcome = true;
    if (terms.count() == 2)
        outcome = outcome && QMap<QString, QMap<QString, double>>::contains(terms[0]);
    if (terms.count() == 2)
        outcome = outcome && value(terms[0]).contains(terms[1]);
    if (terms.count() == 1)
        outcome = outcome && (level(terms[0]) != -1);
            
    return outcome;
}

bool DataStructure::Validate() const
{
    if (NumberOfRows() == -1)
    {
        //qDebug() << "Number of rows are different";
        return false;
    }
    QStringList rowHeaders = RowHeaders(); 
    
    if (rowHeaders.size() == 0)
    {
        //qDebug() << "Row names are different! ";
        return false;
    }
    return true; 
    

}

bool DataStructure::appendColumn(const QString& columnname, AquaArray& array, const QStringList &rownames)
{
    if (array.count() != NumberOfRows() && NumberOfRows() != 0)
    {
        //qDebug() << "Number of array element is different than the number of rows.";
        return false;
    }
    if (rownames.count()!=0 && rownames.count()!=array.count())
    {
        //qDebug() << "Number of array element is different than the number of names provided.";
        return false; 
    }
    int i = 0; 
    for (QMap<QString, double>& Map : this->values())
    {
        if (rownames.count() != 0)
            Map[columnname] = array[i];
        else
            Map[QString::number(array[i])] = array[i];
    }
    return true; 

}

QStringList DataStructure::ColumnHeaders() const
{
    QStringList out; 
    out.append(this->keys());
    return out; 
}

QStringList DataStructure::RowHeaders() const
{
    QStringList out; 
    for (QMap<QString, double>& Map : this->values())
    {
        if (out.isEmpty() || Map.keys() == out)
            out = Map.keys();
        else
        {
            //qDebug() << "Row names are different";
            return QStringList();
        }
    }
    return out; 
}

bool DataStructure::writeToCSVFile(const QString& filename) const
{
    
    QFile file(filename);

    // Attempt to open the file in write-only and text mode.
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        //qDebug() << "Error: Unable to open file for writing:" << file.errorString();
        return false;
    }

    QTextStream out(&file);

    QStringList rowHeaders = RowHeaders();
    if (rowHeaders.size() == 0)
    {
        return false; 
    }
    QStringList colHeaders = ColumnHeaders(); 
    for (int i = 0; i < colHeaders.count(); i++)
        out << "," << colHeaders[i];
    out << "\n";
    
    for (int i = 0; i < rowHeaders.count(); i++)
    {
        out << rowHeaders[i];
        for (QMap<QString, double>& Map : this->values())
            out << "," << Map.value(rowHeaders[i]);
        out << "\n";
    }
    
    file.close();
    return true;
}

bool DataStructure::appendRow(const QString& rowname, AquaArray& array)
{
    if (array.count() != NumberOfColumns() && NumberOfRows() != 0)
    {
        //qDebug() << "Number of array element is different than the number of columns.";
        return false;
    }
    
    int i = 0;
    QMap<QString, double> tobeadded; 
    for (unsigned int i = 0; i < array.size(); i++)
    {
    
    }
    return true;
}

bool DataStructure::appendColumn(const QString& columnname, AquaArray& array)
{
    if (array.count() != NumberOfRows() && NumberOfColumns() != 0)
    {
        //qDebug() << "Number of array element is different than the number of columns.";
        return false;
    }

    int i = 0;
    QMap<QString, double> tobeadded;
    for (unsigned int i = 0; i < array.size(); i++)
    {

    }
    this->operator[](columnname) = tobeadded;
    return true;

}



unsigned int DataStructure::NumberOfColumns() const
{
    return count();
}

unsigned int DataStructure::NumberOfRows() const
{
    int out = -1; 
    for (const QMap<QString, double>& Map : this->values())
    {
        if (Map.count() == out || out == -1)
        {
            out = Map.count();
        }
        else
        {
            //qDebug() << "Number of rows are different";
            return -1; 
        }
    }
    return out; 
}


