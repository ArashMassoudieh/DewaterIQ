#pragma once
#include <QString> 
#include <QMap>
#include <QStack>
#include <QRegularExpression>
#include <QDebug>
#include <cmath>
#include <stdexcept>

class DataStructure; 

class ExpressionCalculator: public QString
{

public:
    // Supported mathematical functions
    inline static QMap<QString, double(*)(double)> functions = {
        {"sin", sin}, {"cos", cos}, {"tan", tan},
        {"log", log}, {"exp", exp}, {"sqrt", sqrt}
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

    // Convert infix expression to postfix (Reverse Polish Notation)
    static QStringList infixToPostfix(const QString& expression, const QMap<QString, double>& variables) {
        QStack<QString> operators;
        QStringList output;
        QRegularExpression tokenRegex(R"([A-Za-z_][A-Za-z0-9_]*|\d+(\.\d+)?|[\+\-\*/\^\(\)])");

        QRegularExpressionMatchIterator it = tokenRegex.globalMatch(expression);
        while (it.hasNext()) {
            QRegularExpressionMatch match = it.next();
            QString token = match.captured(0);

            // Number or variable
            if (token[0].isDigit() || variables.contains(token)) {
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

    // Evaluate postfix expression (RPN)
    static double evaluatePostfix(const QStringList& postfix, const QMap<QString, double>& variables) {
        QStack<double> stack;

        for (const QString& token : postfix) {
            // If token is a number
            bool isNumber;
            double num = token.toDouble(&isNumber);

            if (isNumber) {
                stack.push(num);
            }
            // If token is a variable
            else if (variables.contains(token)) {
                stack.push(variables[token]);
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

    // Main function to evaluate an expression
    static double calc(const QString& expression, const QMap<QString, double>& variables) {
        try {
            QStringList postfix = infixToPostfix(expression, variables);
            return evaluatePostfix(postfix, variables);
        }
        catch (const std::runtime_error& e) {
            qDebug() << "Error:" << e.what();
            return NAN; // Return NaN on error
        }
    }

    double calc(const QMap<QString, double>* variables) {
        try {
            QStringList postfix = infixToPostfix(*this, *variables);
            return evaluatePostfix(postfix, *variables);
        }
        catch (const std::runtime_error& e) {
            qDebug() << "Error:" << e.what();
            return NAN; // Return NaN on error
        }
    }


    double calc(const DataStructure* variables);

    QStringList infixToPostfix(const DataStructure* variables);

    double evaluatePostfix(const QStringList& postfix, const DataStructure* variables);

};

