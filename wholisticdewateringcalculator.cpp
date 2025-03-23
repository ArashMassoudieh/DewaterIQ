#include "wholisticdewateringcalculator.h"
#include <iostream>
#include <QFile>
#include <QJsonArray>

WholisticDewateringCalculator::WholisticDewateringCalculator() {}


bool WholisticDewateringCalculator::BuildSystem(const QString &filename)
{
    system.clear();
    Expressions.clear();
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

    if (!rootObj.contains("Tables"))
    {
        qDebug() << "No 'Tables' object was found";
        return false;
    }

    if (!rootObj.contains("Expressions"))
    {
        qDebug() << "No 'Expressions' object was found";
        return false;
    }
    QJsonArray Tables = rootObj["Tables"].toArray();
    //qDebug()<<Tables;
    for (int i=0; i<Tables.count(); i++)
    {
        DataStructure table;
        if (!table.readFromJsonFile(Tables[i].toObject()["FileName"].toString()))
        {
            QString fullpath = findFileRecursive(".",Tables[i].toObject()["FileName"].toString());
            if (!fullpath.isEmpty())
            {   table.readFromJsonFile(fullpath);
                qDebug()<< fullpath + " was successfully loaded";
            }
            else
                qDebug()<<"Table '" + Tables[i].toObject()["FileName"].toString() + "' was not found";
        }
        if (table.count()!=0)
            system[Tables[i].toObject()["Name"].toString()] = table;
    }

    QJsonArray ExpressionJson = rootObj["Expressions"].toArray();
    //qDebug()<<Tables;
    for (int i=0; i<ExpressionJson.count(); i++)
    {
        expression anexpression;
        anexpression.Expression = ExpressionJson[i].toObject()["Expression"].toString();
        anexpression.Type = ExpressionJson[i].toObject()["Type"].toString();
        anexpression.Name = ExpressionJson[i].toObject()["Variable"].toString();
        Expressions.append(anexpression);
    }
    qDebug()<<"Instruction in '" + filename + "' was loaded successfully";

    return true;
}



AquaTable WholisticDewateringCalculator::PerformCalculation()
{

    AquaTable out;
    for (int i=0; i<Expressions.count(); i++)
    {
        if (Expressions[i].Type == "Sequence")
        {
            QStringList parameters = Expressions[i].Expression.split(":");
            out.AppendSequence(Expressions[i].Name,parameters[0].toDouble(),parameters[1].toDouble(),parameters[2].toInt(),sequencemode::linear);
        }
        if (Expressions[i].Type == "Scalar")
        {
            double var = system.Calculate(Expressions[i].Expression);
            system.InsertScalar(Expressions[i].Name,var);
        }
        if (Expressions[i].Type == "Array")
        {
            AquaArray var = system.Calculate(&out,Expressions[i].Expression);
            out.AppendColumn(Expressions[i].Name, var);
        }
    }

    std::cout<<"Finished"<<std::endl;
    //out.WritetoCSV("Calculated.csv");
    return out;
}

bool WholisticDewateringCalculator::SetValue(const QString &expression, const double& value)
{
    return system.SetValue(expression,value);
}

#include <QDirIterator>
#include <QString>
#include <QFileInfo>

// Returns the full path of the file if found, or an empty QString otherwise.
QString WholisticDewateringCalculator::findFileRecursive(const QString& rootFolder, const QString& fileName)
{
    QDirIterator iterator(rootFolder,
                          QStringList() << fileName,
                          QDir::Files,
                          QDirIterator::Subdirectories);

    while (iterator.hasNext()) {
        iterator.next();
        QFileInfo fileInfo = iterator.fileInfo();
        if (fileInfo.fileName() == fileName) {
            return fileInfo.absoluteFilePath();
        }
    }

    // If not found, return empty QString
    return QString();
}

