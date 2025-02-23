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
        QStringList keys = expression.split('.'); // Split into ["x", "y", "z"]
        if (keys.size() != 3) {
            throw std::invalid_argument("Invalid format for System. Use 'key1.key2.key3'");
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

    QJsonObject toJsonObject() const;

    bool FromJsonObject(const QJsonObject& jsonobject);

    bool WritetoJsonFile(const QString& filename) const;

    bool readFromJsonFile(const QString& filename);
    
    
};
