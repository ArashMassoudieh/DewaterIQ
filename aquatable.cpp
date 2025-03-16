#include "aquatable.h"
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <cmath>

// Default Constructor
AquaTable::AquaTable() : QVector<AquaArray>() {}

// Copy Constructor
AquaTable::AquaTable(const AquaTable& other) : QVector<AquaArray>(other) {}

// Assignment Operator
AquaTable& AquaTable::operator=(const AquaTable& other) {
    if (this != &other) { // Avoid self-assignment
        QVector<AquaArray>::operator=(other); // Use QVector's assignment operator
    }
    return *this;
}

// Destructor
AquaTable::~AquaTable() {
    // No explicit cleanup needed unless AquaArray has dynamic memory
}

bool AquaTable::AppendSequence(const QString &columnname, const double &x_min, const double &x_max, const unsigned int &number_of_intervals, sequencemode mode)
{
    clear();

    for (int i = 0; i<=number_of_intervals; i++)
    {
        AquaArray tobeadded;
        if (mode==sequencemode::linear)
            tobeadded.append(x_min + (x_max-x_min)/double(number_of_intervals)*i);
        else
            tobeadded.append(exp(log(x_min) + (log(x_max)-log(x_min))/double(number_of_intervals)*i));
        append(tobeadded);
    }
    columnnames.append(columnname);
    return true;
}

bool AquaTable::AppendColumn(const QString &columnname, const AquaArray &column)
{
    if (size()!=0)
    {
        if (column.size()!=size())
        {   throw std::invalid_argument("The number of elements does not match the table");
            return false;
        }
        else
        {
            for (int i = 0; i<column.size(); i++)
            {
                operator[](i).append(column[i]);
            }
        }

    }
    else
    {
        resize(column.size());
        for (int i = 0; i<column.size(); i++)
        {
            operator[](i).append(column[i]);
        }
    }
    columnnames.append(columnname);
    return true;
}

// Function to write AquaTable data to a CSV file
bool AquaTable::WritetoCSV(const QString &filename)
{
    QFile file(filename);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false; // File could not be opened

    QTextStream out(&file);

    // Write column names
    if (!columnnames.isEmpty()) {
        out << columnnames.join(",") << "\n";
    }

    // Write data row by row
    for (const AquaArray& row : *this) {
        QStringList rowValues;
        for (double value : row) {
            rowValues.append(QString::number(value, 'g', 10)); // Convert to string
        }
        out << rowValues.join(",") << "\n";
    }

    file.close();
    return true;
}

// Function to write AquaTable data to a JSON file
bool AquaTable::WritetoJson(const QString &filename) const
{
    if (count()==0)
        return false;

    QFile file(filename);

    if (!file.open(QIODevice::WriteOnly))
        return false; // File could not be opened

    QJsonDocument doc = toJson();
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    return true;
}

QJsonDocument AquaTable::toJson() const
{
    QJsonObject root;
    // Iterate over rows and convert to JSON
    for (int j=0; j<begin()->count(); j++) {
        QJsonArray jsonArray;
        QString key = (j < columnnames.size()) ? columnnames[j] : QString("Column%1").arg(j);
        for (int i = 0; i < size(); ++i) {
            jsonArray.append(operator[](i).value(j));

        }
        root[key] = jsonArray;
    }

    // Create the JSON document
    QJsonDocument doc(root);

    return doc;
}

// Function to read an AquaTable from a JSON file
bool AquaTable::ReadFromJson(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        return false; // File could not be opened
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (doc.isNull() || !doc.isObject()) {
        return false; // Invalid JSON format
    }

    QJsonObject root = doc.object();
    if (root.isEmpty()) {
        return false; // No data found
    }

    clear(); // Clear existing data

    // Extract column names from JSON keys
    columnnames = root.keys();
    if (columnnames.isEmpty()) {
        return false; // No valid data
    }

    int numRows = 0;
    QVector<QJsonArray> columns;

    // Determine the number of rows and store column data
    for (const QString &key : columnnames) {
        QJsonArray jsonArray = root[key].toArray();
        columns.append(jsonArray);
        if (jsonArray.size() > numRows) {
            numRows = jsonArray.size();
        }
    }

    // Populate AquaTable with data
    for (int i = 0; i < numRows; ++i) {
        AquaArray row;
        for (int j = 0; j < columns.size(); ++j) {
            if (i < columns[j].size()) {
                row.append(columns[j][i].toDouble());
            } else {
                row.append(0.0); // Fill missing values with 0.0
            }
        }
        append(row);
    }

    return true;
}

// Function to read an AquaTable from a JSON file
bool AquaTable::FromJsonString(const QString &jsonString)
{

    QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8());
    if (doc.isNull() || !doc.isObject()) {
        return false; // Invalid JSON format
    }

    QJsonObject root = doc.object();
    if (root.isEmpty()) {
        return false; // No data found
    }

    clear(); // Clear existing data

    // Extract column names from JSON keys
    columnnames = root.keys();
    if (columnnames.isEmpty()) {
        return false; // No valid data
    }

    int numRows = 0;
    QVector<QJsonArray> columns;

    // Determine the number of rows and store column data
    for (const QString &key : columnnames) {
        QJsonArray jsonArray = root[key].toArray();
        columns.append(jsonArray);
        if (jsonArray.size() > numRows) {
            numRows = jsonArray.size();
        }
    }

    // Populate AquaTable with data
    for (int i = 0; i < numRows; ++i) {
        AquaArray row;
        for (int j = 0; j < columns.size(); ++j) {
            if (i < columns[j].size()) {
                row.append(columns[j][i].toDouble());
            } else {
                row.append(0.0); // Fill missing values with 0.0
            }
        }
        append(row);
    }

    return true;
}


