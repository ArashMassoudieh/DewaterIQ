#pragma once
#include <QString> 
#include <QMap>
#include <QStack>
#include <QRegularExpression>
#include <QDebug>
#include <cmath>
#include <stdexcept>

class DataStructure;
class System;
class AquaArray;
class AquaTable;

double hsd(double x);
double floor(double x);

class ExpressionCalculator: public QString
{

public:
    // Supported mathematical functions
    inline static QMap<QString, double(*)(double)> functions = {
        {"sin", sin}, {"cos", cos}, {"tan", tan},
        {"log", log}, {"exp", exp}, {"sqrt", sqrt}, {"hsd",hsd}, {"floor", floor}
    };

    ExpressionCalculator(const QString& expression) :QString(expression)
    {

    }

    ExpressionCalculator(const ExpressionCalculator& expression) :QString(expression)
    {

    }

    ExpressionCalculator() :QString()
    {

    }

    ExpressionCalculator& operator=(const QString& expression)
    {
        QString::operator=(expression);
    }

    ExpressionCalculator& operator=(const ExpressionCalculator& expression)
    {
        QString::operator=(expression);
    }

    // Operator precedence map
    static int precedence(QChar op) {
        if (op == '+' || op == '-') return 1;
        if (op == '*' || op == '/') return 2;
        if (op == '^') return 3;
        return 0;
    }

    // Perform arithmetic
    static double applyOperator(double a, double b, QChar op) {
        switch (op.toLatin1()) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': if (b == 0) throw std::runtime_error("Division by zero"); return a / b;
        case '^': return pow(a, b);
        default: throw std::runtime_error("Unknown operator");
        }
    }

    template <typename T>
    double calc(const T* variables);

    template <typename T>
    QStringList infixToPostfix(const T* variables);

    template<typename T>
    QStringList infixToPostfix(const AquaArray* array, const T* variables);

    template <typename T>
    double evaluatePostfix(const QStringList& postfix, const T* variables);

    template <typename T>
    double calc(const AquaArray* array, const T* variables);

    template<typename T>
    AquaArray calc(const AquaTable* array, const T* variables);

    template <typename T>
    double evaluatePostfix(const AquaArray* array, const QStringList& postfix, const T* variables);

    static bool isValidXFormat(const QString& str) {
        // The regex pattern: 
        // ^       : start of string
        // x       : literal 'x'
        // \d+     : one or more digits
        // $       : end of string
        QRegularExpression regex("^x\\d+$");
        return regex.match(str).hasMatch();
    }
private:
    QStringList AquaTableVariableNames;

};


#include "ExpressionCalculator.hpp"
