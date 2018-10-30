#ifndef FUNCTION_H
#define FUNCTION_H
#include <cmath>

double sigmoid(double arg)
{
    return 1.0 / (1.0 + pow(M_E, -arg));
}

#endif // FUNCTION_H
