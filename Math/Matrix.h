#ifndef MATRIX_H
#define MATRIX_H
#include <string.h>
#include "Array.h"

typedef unsigned int __uint32;

template<__uint32 __line, __uint32 __column>
class Matrix
{
private:
    typedef __uint32 _line_;
    typedef __uint32 _column_;

    double __data[__line][__column];
public:
    __uint32 line() { return __line; }
    __uint32 column() { return __column; }

    Matrix<__column, __line> T();//转置

    explicit Matrix(double default_value = 0);
    Matrix operator=(Matrix<__line, __column> &arg);
    Matrix operator+(Matrix<__line, __column> &arg);
    Matrix operator-(Matrix<__line, __column> &arg);
    Matrix operator*(double arg);

    template<__uint32 _line_, __uint32 _column_>
    Matrix operator*(Matrix<_line_, _column_> &arg)
    {
        if(__column != _line_ || __line != _column_)
            return Matrix<0, 0>();
        Matrix<__line, _column_> result;
        for(__uint32 line = 0; line < __line; line++)
            for(__uint32 column = 0; column < _column_; column++)
                for(__uint32 index = 0; index < __column; index++)
                    result.__data[line][column] = __data[line][index] * arg.__data[index][column];
        return result;
    }

    double operator()(_line_ line, _column_ column)
    { return __data[line][column]; }

    Matrix operator+=(Matrix<__line, __column> &arg);
    Matrix operator-=(Matrix<__line, __column> &arg);
    Matrix operator*=(double arg);
};

template<__uint32 __line, __uint32 __column>
Matrix<__column, __line> Matrix<__line, __column>::T()
{
    Matrix<__column, __line> result;
    for(__uint32 i = 0; i < __line; i++)
        for(__uint32 j = 0; j < __column; j++)
            result.__data[i][j] = __data[j][i];
    return result;
}

template<__uint32 __line, __uint32 __column>
Matrix<__line, __column>::Matrix(double default_value)
{
    for(__uint32 i = 0; i < __line; i++)
        for(__uint32 j = 0; j < __line; j++)
            __data[i][j] = default_value;
}

template<__uint32 __line, __uint32 __column>
Matrix<__line, __column> Matrix<__line, __column>::operator=(Matrix<__line, __column> &arg)
{
    for(__uint32 i = 0; i < __line; i++)
        memcpy(__data[i], arg.__data[i], __column * 8);
    return *this;
}

template<__uint32 __line, __uint32 __column>
Matrix<__line, __column> Matrix<__line, __column>::operator+(Matrix<__line, __column> &arg)
{
    Matrix<__line, __column> result;
    return result += arg;
}

template<__uint32 __line, __uint32 __column>
Matrix<__line, __column> Matrix<__line, __column>::operator-(Matrix<__line, __column> &arg)
{
    Matrix<__line, __column> result;
    return result -= arg;
}

template<__uint32 __line, __uint32 __column>
Matrix<__line, __column> Matrix<__line, __column>::operator*(double arg)
{
    Matrix<__line, __column> result;
    return result *= arg;
}

template<__uint32 __line, __uint32 __column>
Matrix<__line, __column> Matrix<__line, __column>::operator+=(Matrix<__line, __column> &arg)
{
    for(__uint32 i = 0; i < __line; i++)
        for(__uint32 j = 0; j < __column; j++)
            __data[i][j] = arg.__data[i][j];
    return *this;
}

template<__uint32 __line, __uint32 __column>
Matrix<__line, __column> Matrix<__line, __column>::operator-=(Matrix<__line, __column> &arg)
{
    for(__uint32 i = 0; i < __line; i++)
        for(__uint32 j = 0; j < __column; j++)
            __data[i][j] -= arg.__data[i][j];
    return *this;
}

template<__uint32 __line, __uint32 __column>
Matrix<__line, __column> Matrix<__line, __column>::operator*=(double arg)
{
    for(__uint32 i = 0; i < __line; i++)
        for(__uint32 j = 0; j < __column; j++)
            __data[i][j] *= arg;
    return *this;
}

#endif // MATRIX_H

