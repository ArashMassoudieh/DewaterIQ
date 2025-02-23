#pragma once
#include <QMap>
#include <QString>
#include <QJsonDocument>

class DataStructure : public QMap<QString, QMap<QString,double>>
{
    
public:
    DataStructure() = default;
    DataStructure(const DataStructure& other) : QMap<QString, QMap<QString, double>>(other) {}
    DataStructure& operator=(const DataStructure& other) {
        if (this != &other) { // Prevent self-assignment
            QMap<QString, QMap<QString, double>>::operator=(other);
        }
        return *this;
    }

    ~DataStructure() = default;
    double Eval(const QString& expression) const;

    bool readFromJsonFile(const QString& filename);
    bool FromJson(const QJsonObject &jsonobject);

    QJsonDocument toJsonDocument() const;
    double sumSubkeys(const QString& key) const;
    double sumKeys(const QString &key) const;
    QJsonObject toJsonObject() const;

    bool writeToJsonFile(const QString& filename) const;
    int SetLevels(const QJsonObject& jsonobject);
private:
    int levels = 2; 

    
};




