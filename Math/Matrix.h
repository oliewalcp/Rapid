#ifndef MATRIX_H
#define MATRIX_H
#include "MathBase.h"

class Matrix : public _M_Base<double>
{
public:
    Matrix(const __uint32 line, const __uint32 column, const double default_value = 0);

    Matrix operator+(Matrix &arg);
    Matrix operator*(Matrix &arg);
    Matrix operator+=(Matrix &arg);
    Matrix operator=(Matrix &arg);
    /* let each of the value of the _line_ line multiply arg
     * param[arg]: factor
     * param[line]: the value of the _line_ line will change
     */
    void line_multiply(const double value, const __uint32 line);

    Matrix *T();//Transpose
};

Matrix* Matrix::T()
{
    __uint32 line = __data->size();
    Matrix *result = new Matrix(__column, line);
    for(__uint32 i = 0; i < line; i++)
    {
        for(__uint32 j = 0; j < __column; j++)
        {
            result->set_value(j, i, get_value(i, j));
        }
    }
    return result;
}

Matrix::Matrix(const __uint32 line, const __uint32 column, const double default_value)
{
    __column = column;
    __data = new std::vector<double *>(line);
    for(__uint32 i = 0; i < line; i++)
    {
        double *temp = new double[column];
        for(__uint32 j = 0; j < column; j++)
            temp[j] = default_value;
        __data->push_back(temp);
    }
}

Matrix Matrix::operator+(Matrix &arg)
{
    if(line() != arg.line())
        throw Exception("the number of current line is different from arg");
    if(column() != arg.column())
        throw Exception("the number of current column is different from arg");
    __uint32 row = line();
    Matrix result(row, __column);
    for(__uint32 i = 0; i < row; i++)
    {
        double *temp = __data->at(i);
        double *arg_temp = arg.__data->at(i);
        double *result_temp = result.__data->at(i);
        for(__uint32 j = 0; j < __column; j++)
            result_temp[j] = temp[j] + arg_temp[j];
    }
    return result;
}

Matrix Matrix::operator*(Matrix &arg)
{
    if(column() != arg.line())
        throw Exception("the number of current column is different from the number of arg line");
    __uint32 row = line();
    Matrix result(row, arg.__column);
    for(__uint32 i = 0; i < row; i++)
    {
        for(__uint32 j = 0; j < arg.__column; j++)
        {
            double temp = 0;
            for(__uint32 index = 0; index < __column; index++)
                temp += get_value(i, index) * arg.get_value(index, i);
            result.set_value(i, j, temp);
        }
    }
    return result;
}

Matrix Matrix::operator+=(Matrix &arg)
{
    if(line() != arg.line())
        throw Exception("the number of current line is different from arg");
    if(column() != arg.column())
        throw Exception("the number of current column is different from arg");
    __uint32 row = line();
    for(__uint32 i = 0; i < row; i++)
    {
        double *temp = __data->at(i);
        double *arg_temp = arg.__data->at(i);
        for(__uint32 j = 0; j < __column; j++)
            temp[j] += arg_temp[j];
    }
    return *this;
}

Matrix Matrix::operator=(Matrix &src)
{
    if(line() != src.line())
        throw Exception("the number of current line is different from arg");
    if(column() != src.column())
        throw Exception("the number of current column is different from arg");
    __uint32 row = line();
    for(__uint32 i = 0; i < row; i++)
    {
        double *temp = __data->at(i);
        double *src_temp = src.__data->at(i);
        memcpy(temp, src_temp, 8 * __column);
    }
    return *this;
}

void Matrix::line_multiply(const double value, const __uint32 line)
{
    double *temp = __data->at(line);
    for(__uint32 i = 0; i < __column; i++)
        temp[i] = value;
}

#endif // MATRIX_H

