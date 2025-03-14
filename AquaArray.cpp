#include "AquaArray.h"
#include <cmath>

bool AquaArray::CreateSequence(const double &x_min, const double &x_max, const unsigned int &number_of_intervals, sequencemode mode)
{
    clear();

    for (int i = 0; i<=number_of_intervals; i++)
    {
        if (mode==sequencemode::linear)
            append(x_min + (x_max-x_min)/double(number_of_intervals)*i);
        else
            append(exp(log(x_min) + (log(x_max)-log(x_min))/double(number_of_intervals)*i));
    }

    return true;
}



