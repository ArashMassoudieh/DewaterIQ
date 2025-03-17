#include <QMap>
#include "DataStructure.h"
#include <stdexcept>
#include <QJsonObject>


class System : public QMap<QString, DataStructure> {
public:
    // Default Constructor
    System() = default;

    // Copy Constructor
    System(const System& other) : QMap<QString, DataStructure>(other) {}

    // Assignment Operator
    System& operator=(const System& other) {
        if (this != &other) {
            QMap<QString, DataStructure>::operator=(other);
        }
        return *this;
    }

    // Destructor
    ~System() = default;

    // Eval Function for "x.y.z"
    double Eval(const QString& expression) const {
        QStringList keys = expression.split(':'); // Split into ["x", "y", "z"]
        if (keys.size()==1)
        {
            if (!this->contains(keys[0])) {
                throw std::out_of_range(QString("Key '%1' not found").arg(keys[0]).toStdString());
                return 0;
            }
            else
            {
                return operator[](keys[0]).sumTotal();
            }
        }

        if (keys.size()==2)
        {
            if (!this->contains(keys[0])) {
                throw std::out_of_range(QString("Key '%1' not found").arg(keys[0]).toStdString());
                return 0;
                if (!this->operator[](keys[0]).contains(keys[1]))
                {
                    throw std::out_of_range(QString("Key '%1' not found").arg(keys[1]).toStdString());
                    return 0;
                }
            }
            else
            {
                return operator[](keys[0]).sumKeys(keys[1]);
            }
        }

        QString key1 = keys[0];
        QString key2 = keys[1];
        QString key3 = keys[2];

        if (!this->contains(key1)) {
            throw std::out_of_range(QString("Key '%1' not found").arg(key1).toStdString());
        }
        if (!this->value(key1).contains(key2)) {
            throw std::out_of_range(QString("Subkey '%1' not found in '%2'").arg(key2, key1).toStdString());
        }
        if (!this->value(key1).value(key2).contains(key3)) {
            throw std::out_of_range(QString("Sub-subkey '%1' not found in '%2.%3'").arg(key3, key1, key2).toStdString());
        }

        return this->value(key1).value(key2).value(key3);
    }

    bool SetValue(const QString& expression, const double &Value) {
        QStringList keys = expression.split(':'); // Split into ["x", "y", "z"]
        if (keys.size()!=3)
            return false;

        if (!contains(expression))
            return false;
        QString key1 = keys[0];
        QString key2 = keys[1];
        QString key3 = keys[2];

        this->operator[](key1).operator[](key2).operator[](key3)=Value;
        return true;
    }

    QJsonObject toJsonObject() const;

    bool FromJsonObject(const QJsonObject& jsonobject);

    bool WritetoJsonFile(const QString& filename) const;

    bool readFromJsonFile(const QString& filename);
    
    double Calculate(const QString& expression) const;
    AquaArray Calculate(const AquaTable* array, const QString& expression);

    bool contains(const QString& variable) const;

    int level(const QString& item) const;
    
    bool InsertScalar(const QString &VariableName, const double& value);
    bool InsertVector(const QString &MapName, const QStringList &VariableNames, const QVector<double>& values);
};
