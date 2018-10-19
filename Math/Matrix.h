#ifndef MATRIX_H
#define MATRIX_H
#include "TypeBase.h"

template<__uint32 __line, __uint32 __column>
class Matrix : public _M_Base<__line, __column, double>
{
public:
    Matrix<__column, __line> T();//转置

    explicit Matrix(double default_value = 0);
    Matrix operator=(const Matrix<__line, __column> &arg);
    Matrix operator+(const Matrix<__line, __column> &arg);
    Matrix operator-(const Matrix<__line, __column> &arg);
    Matrix operator+=(const Matrix<__line, __column> &arg);
    Matrix operator-=(const Matrix<__line, __column> &arg);

    template<__uint32 _line_, __uint32 _column_>
    Matrix<__line, _column_> operator*(const Matrix<_line_, _column_> &arg) const
    {
        static_assert(!(__column != _line_), "the column of firth matrix is different from the line of second matrix");
        Matrix<__line, _column_> result;
        for(__uint32 line = 0; line < __line; line++)
            for(__uint32 column = 0; column < _column_; column++)
                for(__uint32 index = 0; index < __column; index++)
                    result.__data[line][column] = this->__data[line][index] * arg.__data[index][column];
        return result;
    }

    void Multiply(const double arg, const __uint32 line);
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
void Matrix<__line, __column>::Multiply(const double arg, const __uint32 line)
{
    for(__uint32 i = 0; i < __column; i++)
        this->__data[line][i] *= arg;
}

#endif // MATRIX_H

