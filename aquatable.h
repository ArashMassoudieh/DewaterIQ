#ifndef AQUATABLE_H
#define AQUATABLE_H

#include "AquaArray.h"
#include <QStringList>
#include <QJsonDocument>

class DataStructure;

class AquaTable : public QVector<AquaArray>
{
public:
    // Default Constructor
    AquaTable();

    // Copy Constructor
    AquaTable(const AquaTable& other);

    // Assignment Operator
    AquaTable& operator=(const AquaTable& other);

    // Destructor
    ~AquaTable();

    bool AppendSequence(const QString &columnname, const double &x_min, const double &x_max, const unsigned int &number_of_intervals, sequencemode mode=sequencemode::linear);
    bool AppendColumn(const QString &columnname, const AquaArray &column);

    QStringList ColumnNames() const {return columnnames;}
    bool WritetoCSV(const QString &filename);
    bool WritetoJson(const QString &filename) const;
    QJsonDocument toJson() const;
    bool ReadFromJson(const QString &filename);
    DataStructure toDataStructure(bool rownamesfromfirstcolumn = true);
private:
    QStringList columnnames;

};

#endif // AQUATABLE_H
