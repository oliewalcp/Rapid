#ifndef MATRIX_H
#define MATRIX_H
#include <string.h>
#include "Array.h"
#include <type_traits>

typedef unsigned int __uint32;

template<__uint32 __line, __uint32 __column, typename value_type>
class _M_Base
{
protected:
    value_type __data[__line][__column];
public:
    virtual __uint32 line() { return __line; }
    virtual __uint32 column() { return __column; }
    virtual value_type operator()(__uint32 line, __uint32 column)
    { return __data[line][column]; }
};

template<__uint32 __line, __uint32 __column>
class Matrix : public _M_Base<__line, __column, double>
{
public:
    Matrix<__column, __line> T();//转置

    explicit Matrix(double default_value = 0);
    Matrix operator=(const Matrix<__line, __column> &arg);
    Matrix operator+(const Matrix<__line, __column> &arg);
    Matrix operator-(const Matrix<__line, __column> &arg);
    Matrix operator*(const double arg);

    template<__uint32 _line_, __uint32 _column_>
    Matrix operator*(const Matrix<_line_, _column_> &arg) const
    {
        if(__column != _line_ || __line != _column_)
            return Matrix<0, 0>();
        Matrix<__line, _column_> result;
        for(__uint32 line = 0; line < __line; line++)
            for(__uint32 column = 0; column < _column_; column++)
                for(__uint32 index = 0; index < __column; index++)
                    result.__data[line][column] = this->__data[line][index] * arg.__data[index][column];
        return result;
    }
    Matrix operator+=(const Matrix<__line, __column> &arg);
    Matrix operator-=(const Matrix<__line, __column> &arg);
    Matrix operator*=(const double arg);
};

template<__uint32 __line, __uint32 __column>
Matrix<__column, __line> Matrix<__line, __column>::T()
{
    Matrix<__column, __line> result;
    for(__uint32 i = 0; i < __line; i++)
        for(__uint32 j = 0; j < __column; j++)
            result.__data[i][j] = this->__data[j][i];
    return result;
}

template<__uint32 __line, __uint32 __column>
Matrix<__line, __column>::Matrix(double default_value)
{
    for(__uint32 i = 0; i < __line; i++)
        for(__uint32 j = 0; j < __line; j++)
            this->__data[i][j] = default_value;
}

template<__uint32 __line, __uint32 __column>
Matrix<__line, __column> Matrix<__line, __column>::operator=(const Matrix<__line, __column> &arg)
{
    for(__uint32 i = 0; i < __line; i++)
        memcpy(this->__data[i], arg.__data[i], __column * 8);
    return *this;
}

template<__uint32 __line, __uint32 __column>
Matrix<__line, __column> Matrix<__line, __column>::operator+(const Matrix<__line, __column> &arg)
{
    Matrix<__line, __column> result;
    for(__uint32 i = 0; i < __line; i++)
        for(__uint32 j = 0; j < __column; j++)
            result.__data[i][j] = this->__data[i][j] + arg.__data[i][j];
    return result;
}

template<__uint32 __line, __uint32 __column>
Matrix<__line, __column> Matrix<__line, __column>::operator-(const Matrix<__line, __column> &arg)
{
    Matrix<__line, __column> result;
    return result -= arg;
}

template<__uint32 __line, __uint32 __column>
Matrix<__line, __column> Matrix<__line, __column>::operator*(const double arg)
{
    Matrix<__line, __column> result;
    return result *= arg;
}

template<__uint32 __line, __uint32 __column>
Matrix<__line, __column> Matrix<__line, __column>::operator+=(const Matrix<__line, __column> &arg)
{
    for(__uint32 i = 0; i < __line; i++)
        for(__uint32 j = 0; j < __column; j++)
            this->__data[i][j] += arg.__data[i][j];
    return *this;
}

template<__uint32 __line, __uint32 __column>
Matrix<__line, __column> Matrix<__line, __column>::operator-=(const Matrix<__line, __column> &arg)
{
    for(__uint32 i = 0; i < __line; i++)
        for(__uint32 j = 0; j < __column; j++)
            this->__data[i][j] -= arg.__data[i][j];
    return *this;
}

template<__uint32 __line, __uint32 __column>
Matrix<__line, __column> Matrix<__line, __column>::operator*=(const double arg)
{
    for(__uint32 i = 0; i < __line; i++)
        for(__uint32 j = 0; j < __column; j++)
            this->__data[i][j] *= arg;
    return *this;
}

#endif // MATRIX_H

