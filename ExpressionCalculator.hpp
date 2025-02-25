#pragma once
#include "ExpressionCalculator.h"
#include "AquaArray.h"

template <typename T>
double ExpressionCalculator::calc(const T* variables) {
    try {
        QStringList postfix = infixToPostfix(variables);
        return evaluatePostfix(postfix, variables);
    }
    catch (const std::runtime_error& e) {
        qDebug() << "Error:" << e.what();
        return NAN; // Return NaN on error
    }
}


template <typename T>
QStringList ExpressionCalculator::infixToPostfix(const T* variables) {
    QStack<QString> operators;
    QStringList output;
    QRegularExpression tokenRegex(R"([A-Za-z_:][A-Za-z0-9_:]*|\d+(\.\d+)?|[\+\-\*/\^\(\)])");

    QRegularExpressionMatchIterator it = tokenRegex.globalMatch(*this);
    while (it.hasNext()) {
        QRegularExpressionMatch match = it.next();
        QString token = match.captured(0);

        // Number or variable
        if (token[0].isDigit() || variables->contains(token)) {
            output << token;
        }
        // Functions (sin, cos, log, etc.)
        else if (functions.contains(token)) {
            operators.push(token);
        }
        // Operators
        else if (token == "+" || token == "-" || token == "*" || token == "/" || token == "^") {
            while (!operators.isEmpty() && precedence(operators.top()[0]) >= precedence(token[0])) {
                output << operators.pop();
            }
            operators.push(token);
        }
        // Left parenthesis
        else if (token == "(") {
            operators.push(token);
        }
        // Right parenthesis
        else if (token == ")") {
            while (!operators.isEmpty() && operators.top() != "(") {
                output << operators.pop();
            }
            if (!operators.isEmpty() && operators.top() == "(") {
                operators.pop(); // Remove '('
            }
            // If there is a function before '('
            if (!operators.isEmpty() && functions.contains(operators.top())) {
                output << operators.pop();
            }
        }
    }

    // Pop remaining operators
    while (!operators.isEmpty()) {
        output << operators.pop();
    }

    return output;
}

template <typename T>
QStringList ExpressionCalculator::infixToPostfix(const AquaArray *array, const T* variables) {
    QStack<QString> operators;
    QStringList output;
    QRegularExpression tokenRegex(R"([A-Za-z_:][A-Za-z0-9_:]*|\d+(\.\d+)?|[\+\-\*/\^\(\)])");

    QRegularExpressionMatchIterator it = tokenRegex.globalMatch(*this);
    while (it.hasNext()) {
        QRegularExpressionMatch match = it.next();
        QString token = match.captured(0);

        // Number or variable
        if (token[0].isDigit() || variables->contains(token)) {
            output << token;
        }
        else if (isValidXFormat(token))
        {
            qDebug() << token.right(token.size()-1);
            if (token.right(token.size() - 1).toInt() > array->size())
                throw std::runtime_error(QString("Array does not have " + token.left(token.size() - 1) + " elements").toStdString());
            output << token;
        }
        // Functions (sin, cos, log, etc.)
        else if (functions.contains(token)) {
            operators.push(token);
        }
        // Operators
        else if (token == "+" || token == "-" || token == "*" || token == "/" || token == "^") {
            while (!operators.isEmpty() && precedence(operators.top()[0]) >= precedence(token[0])) {
                output << operators.pop();
            }
            operators.push(token);
        }
        // Left parenthesis
        else if (token == "(") {
            operators.push(token);
        }
        // Right parenthesis
        else if (token == ")") {
            while (!operators.isEmpty() && operators.top() != "(") {
                output << operators.pop();
            }
            if (!operators.isEmpty() && operators.top() == "(") {
                operators.pop(); // Remove '('
            }
            // If there is a function before '('
            if (!operators.isEmpty() && functions.contains(operators.top())) {
                output << operators.pop();
            }
        }
    }

    // Pop remaining operators
    while (!operators.isEmpty()) {
        output << operators.pop();
    }

    return output;
}


template <typename T>
double ExpressionCalculator::evaluatePostfix(const QStringList& postfix, const T *variables) {
    QStack<double> stack;

    for (const QString& token : postfix) {
        // If token is a number
        bool isNumber;
        double num = token.toDouble(&isNumber);

        if (isNumber) {
            stack.push(num);
        }
        // If token is a variable
        else if (variables->contains(token)) {
            stack.push(variables->Eval(token));
        }
        // If token is a function (sin, log, etc.)
        else if (functions.contains(token)) {
            if (stack.isEmpty()) throw std::runtime_error("Invalid function usage");
            double arg = stack.pop();
            stack.push(functions[token](arg)); // Apply function
        }
        // If token is an operator
        else if (token.length() == 1 && QString("+-*/^").contains(token)) {
            if (stack.size() < 2) throw std::runtime_error("Invalid expression");

            double b = stack.pop();
            double a = stack.pop();
            stack.push(applyOperator(a, b, token[0]));
        }
    }

    if (stack.size() != 1) throw std::runtime_error("Invalid expression");
    return stack.top();
}


template<typename T>
inline double ExpressionCalculator::evaluatePostfix(const AquaArray* array, const QStringList& postfix,  const T* variables)
{
    QStack<double> stack;

    for (const QString& token : postfix) {
        // If token is a number
        bool isNumber;
        double num = token.toDouble(&isNumber);

        if (isNumber) {
            stack.push(num);
        }
        // If token is a variable
        else if (variables->contains(token)) {
            stack.push(variables->Eval(token));
        }
        else if (isValidXFormat(token))
        {
            qDebug() << token.right(token.size()-1);
            if (token.right(token.size() - 1).toInt()>array->size())
                throw std::runtime_error(QString("Array does not have " + token.right(token.size() - 1) + " elements").toStdString());
            stack.push(array->value(token.right(token.size() - 1).toInt() - 1));
        }
        // If token is a function (sin, log, etc.)
        else if (functions.contains(token)) {
            if (stack.isEmpty()) throw std::runtime_error("Invalid function usage");
            double arg = stack.pop();
            stack.push(functions[token](arg)); // Apply function
        }
        // If token is an operator
        else if (token.length() == 1 && QString("+-*/^").contains(token)) {
            if (stack.size() < 2) throw std::runtime_error("Invalid expression");

            double b = stack.pop();
            double a = stack.pop();
            stack.push(applyOperator(a, b, token[0]));
        }
    }

    if (stack.size() != 1) throw std::runtime_error("Invalid expression");
    return stack.top();
}



template<typename T>
double ExpressionCalculator::calc(const AquaArray* array,  const T* variables)
{
    try {
        QStringList postfix = infixToPostfix(array, variables);
        return evaluatePostfix(array, postfix, variables);
    }
    catch (const std::runtime_error& e) {
        qDebug() << "Error:" << e.what();
        return NAN; // Return NaN on error
    }
}

template<typename T>
AquaArray ExpressionCalculator::calc(const QVector<AquaArray>* array, const T* variables)
{
    AquaArray output;
    for (int i = 0; i < array->size(); i++)
    {
        try {
            AquaArray value = array->value(i);
            QStringList postfix = infixToPostfix(&value, variables);
            output.append(evaluatePostfix(&value, postfix, variables));
        }
        catch (const std::runtime_error& e) {
            qDebug() << "Error:" << e.what();
            output.append(NAN); // Return NaN on error
        }
    }
    return output; 
}
