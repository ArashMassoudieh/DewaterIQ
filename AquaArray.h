#pragma once
#include <QVector>
class AquaArray : public QVector<double>
{
public:
    AquaArray() : QVector<double>() {}
    AquaArray(const AquaArray& other)
        : QVector<double>(other)  // Call the base class copy constructor
    {
    }

    // Assignment operator
    AquaArray& operator=(const AquaArray& other)
    {
        if (this != &other) {
            // Call the base class assignment operator
            QVector<double>::operator=(other);
        }
        return *this;
    }

    // Destructor
    ~AquaArray() {
        // No additional cleanup is needed because QVector handles its own memory.
    }

};

