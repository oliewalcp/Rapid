#ifndef MATRIX_H
#define MATRIX_H

#include "Core/TLNode.h"
#include "Core/TypeTraits.h"
#include "Core/Version.h"
#include "Core/Exception.h"
#include "Core/Memory.h"
#include <initializer_list>
#include <iostream>

namespace rapid
{
/* type _Tp need implements:
 * _Tp operator+(_Tp)
 * _Tp operator*(_Tp)
 * _Tp operator=(_Tp)
 */
template<typename _Tp>
class Matrix final
{
public:
    using ValueType = _Tp;
    using Reference = ValueType&;
    using ConstReference = const ValueType &;
    using RvalueReference = ValueType &&;
    using SizeType = long;

    using MatrixRef = Matrix<_Tp>&;
    using RvalueMatrixRef = Matrix<_Tp> &&;
    using ConstMatrixRef = const Matrix<_Tp>&;
protected:
    using DataType = NodeBase<ValueType>;

    DataType **_M_data = nullptr;
    SizeType _M_row = 0;
    SizeType _M_column = 0;

    Reference _F_data(SizeType r, SizeType c)
    { return _M_data[r][c].ref_content(); }
    ConstReference _F_data(SizeType r, SizeType c) const
    { return _M_data[r][c].const_ref_content(); }
    void _F_copy(ConstMatrixRef m);
    void _F_resize(SizeType r, SizeType c);
    void _F_construct_default(SizeType r, SizeType c, ConstReference v);
    void _F_set_value(Reference v, ConstReference value)
    { v = value; }
    void _F_set_value(SizeType r, SizeType c, ConstReference v)
    { _F_set_value(_M_data[r][c].ref_content(), v); }
    void _F_multiply(ConstMatrixRef m);
    void _F_multiply(SizeType r, ConstReference n);
    void _F_add(ConstMatrixRef m);
    template<typename _FilterType>
    void _F_filter(const Matrix<_FilterType> m);

    static void _SF_clear(DataType **mem, SizeType r);
    static Matrix<_Tp> _SF_multiply(ConstMatrixRef m1, ConstMatrixRef m2);
public:
    Matrix(SizeType r = 1, SizeType c = 1)
    { _F_resize(r, c); }
    Matrix(SizeType r, SizeType c, ConstReference default_value)
    { _F_construct_default(r, c, default_value); }
    Matrix(SizeType r, SizeType c, RvalueReference default_value)
    { _F_construct_default(r, c, forward<ValueType>(default_value)); }
    Matrix(ConstMatrixRef m)
    { _F_copy(m); }
    Matrix(RvalueMatrixRef m)
        : _M_data(m._M_data), _M_row(m._M_row), _M_column(m._M_column)
    {
        m._M_data = nullptr;
        m._M_row = 0;
        m._M_column = 0;
    }
    Matrix(std::initializer_list<std::initializer_list<ValueType>> m);
    ~Matrix()
    { clear(); }

    void set_value(SizeType r, SizeType c, ConstReference v)
    { _F_set_value(r, c, v); }
    void set_value(SizeType r, SizeType c, RvalueReference v)
    { _F_set_value(r, c, forward<ValueType>(v)); }

    ValueType get_value(SizeType r, SizeType c) const
    { return _F_data(r, c); }

    SizeType row() const
    { return _M_row; }
    SizeType column() const
    { return _M_column; }

    void T();

    /* [r] line multiply [n]
     * param[r]: given line
     * param[n]: given value
     */
    void multiply(SizeType r, ConstReference n)
    { _F_multiply(r, n); }
    void multiply(SizeType r, RvalueReference n)
    { _F_multiply(r, forward<ValueType>(n)); }
    void multiply(ConstMatrixRef m)
    { _F_multiply(m); }
    void multiply(RvalueMatrixRef m)
    { _F_multiply(forward<Matrix<_Tp>>(m)); }

    void add(ConstMatrixRef m)
    { _F_add(m); }
    void add(RvalueMatrixRef m)
    { _F_add(forward<Matrix<_Tp>>(m)); }

    void filter(ConstMatrixRef m)
    { _F_filter(m); }
    void filter(RvalueMatrixRef m)
    { _F_filter(forward<Matrix<_Tp>>(m)); }
    void filter(std::initializer_list<std::initializer_list<double>> m)
    { _F_filter(Matrix<double>(m)); }

    void power(SizeType p);

    // need call by manual
    void clear()
    {
        _SF_clear(_M_data, row());
        _M_data = nullptr;
    }

    static Matrix<_Tp> multiply(ConstMatrixRef m1, ConstMatrixRef m2)
    { return _SF_multiply(m1, m2); }
    static Matrix<_Tp> multiply(ConstMatrixRef m1, RvalueMatrixRef m2)
    { return _SF_multiply(m1, forward<Matrix<_Tp>>(m2)); }
    static Matrix<_Tp> multiply(RvalueMatrixRef m1, ConstMatrixRef m2)
    { return _SF_multiply(forward<Matrix<_Tp>>(m1), m2); }
    static Matrix<_Tp> multiply(RvalueMatrixRef m1, RvalueMatrixRef m2)
    { return _SF_multiply(forward<Matrix<_Tp>>(m1), forward<Matrix<_Tp>>(m2)); }

    // copy [m]'s data reference to [this]
    void copy_from(ConstMatrixRef m)
    { 
        _M_data = m._M_data;
        _M_row = m._M_row;
        _M_column = m._M_column;
    }
    void swap(MatrixRef m)
    { 
        Matrix<_Tp> temp;
        temp.copy_from(m);
        m.copy_from(*this);
        copy_from(temp);
        temp._M_data = nullptr;
        temp._M_row = temp._M_column = 0;
    }

    Matrix<_Tp> operator*(ConstMatrixRef m)
    { return _SF_multiply(*this, m); }
    MatrixRef operator*=(ConstMatrixRef m)
    {
        multiply(m);
        return *this;
    }
    MatrixRef operator=(ConstMatrixRef m)
    {
        _F_copy(m);
        return *this;
    }
    Matrix<_Tp> operator+(ConstMatrixRef m)
    {
        Matrix<_Tp> temp(*this);
        temp._F_add(m);
        return temp;
    }
    MatrixRef operator+=(ConstMatrixRef m)
    {
        _F_add(m);
        return *this;
    }

};

//-----------------------impl-----------------------//
//-----------------------impl-----------------------//
//-----------------------impl-----------------------//
//-----------------------impl-----------------------//
//-----------------------impl-----------------------//
template<typename _Tp>
Matrix<_Tp>::Matrix(std::initializer_list<std::initializer_list<ValueType>> m)
{
    _M_row = static_cast<SizeType>(m.size());
    SizeType i = 0, j = 0;
    for(auto it = m.begin(); it != m.end(); it++, i++)
    {
        if(column() == 0)
        {
            _M_column = static_cast<SizeType>(it->size());
            _F_resize(row(), column());
        }
        else if(static_cast<SizeType>(it->size()) != column())
        { throw SizeDoesNotMatchException("exception: target size does not match from source size!"); }
        j = 0;
        for(ValueType t : *it)
        {
            set_value(i, j++, t);
        }
    }
}

template<typename _Tp>
void Matrix<_Tp>::_F_copy(ConstMatrixRef m)
{
    _F_resize(m.row(), m.column());
    for(SizeType i = 0; i < row(); i++)
    {
        for(SizeType j = 0; j < column(); j++)
        {
            set_value(i, j, m.get_value(i, j));
        }
    }
}

template<typename _Tp>
void Matrix<_Tp>::_F_resize(SizeType r, SizeType c)
{
    _SF_clear(_M_data, row());
    _M_row = r;
    _M_column = c;
    _M_data = new DataType*[static_cast<unsigned long long>(row())];
    for(SizeType i = 0; i < row(); i++)
    {
        _M_data[i] = new DataType[static_cast<unsigned long long>(column())];
    }
}

template<typename _Tp>
void Matrix<_Tp>::_F_construct_default(SizeType r, SizeType c, ConstReference value)
{
    _SF_clear(_M_data, row());
    _M_row = r;
    _M_column = c;
    _M_data = new DataType*[row()];
    for(SizeType i = 0; i < row(); i++)
    {
        _M_data[i] = new DataType[column()];
        for(SizeType j = 0; j < column(); j++)
        {
            _M_data[i][j].construct(value);
        }
    }
}

template<typename _Tp>
void Matrix<_Tp>::_SF_clear(DataType **mem, SizeType r)
{
    if(mem == nullptr) return;
    for(SizeType i = 0; i < r; i++)
    {
        delete[] mem[i];
    }
    delete[] mem;
    mem = nullptr;
}

template<typename _Tp>
void Matrix<_Tp>::_F_multiply(ConstMatrixRef m)
{
    if(row() != m.column() || column() != m.row())
    { throw SizeDoesNotMatchException("exception: target size does not match from source size!"); }
    Matrix<_Tp> temp = _SF_multiply(*this, m);
    swap(temp);
}

template<typename _Tp>
void Matrix<_Tp>::_F_multiply(SizeType r, ConstReference n)
{
    for(SizeType c = 0; c < column(); c++)
    { _M_data[r][c] = _M_data[r][c] * n; }
}

template<typename _Tp>
Matrix<_Tp> Matrix<_Tp>::_SF_multiply(ConstMatrixRef m1, ConstMatrixRef m2)
{
    if(m1.row() != m2.column() || m1.column() != m2.row())
    { throw SizeDoesNotMatchException("exception: target size does not match from source size!"); }
    Matrix<_Tp> result(m1.row(), m2.column());
    for(SizeType i = 0; i < result.row(); i++)
    {
        for(SizeType j = 0; j < result.column(); j++)
        {
            ValueType temp = m1.get_value(i, 0) * m2.get_value(0, j);
            for(SizeType m = 1; m < m1.column(); m++)
            {
                temp = temp + m1.get_value(i, m) * m2.get_value(m, j);
            }
            result.set_value(i, j, temp);
        }
    }
    return result;
}

template<typename _Tp>
void Matrix<_Tp>::_F_add(ConstMatrixRef m)
{
    if(row() != m.row() || column() != m.column())
    { throw SizeDoesNotMatchException("exception: target size does not match from source size!"); }
    for(SizeType i = 0; i < row(); i++)
    {
        for(SizeType j = 0; j < column(); j++)
        {
            _F_data(i, j) = _F_data(i, j) + m._F_data(i, j);
        }
    }
}

template<typename _Tp>
template<typename _FilterType>
void Matrix<_Tp>::_F_filter(const Matrix<_FilterType> m)
{
    Matrix<_Tp> result(row(), column());
    SizeType center_row = (m.row() - 1) / 2, center_column = (m.column() - 1) / 2;
    for(SizeType i = 0; i < row(); i++)
    {
        for(SizeType j = 0; j < column(); j++)
        {
            DataType dt;
            mem_clear(&dt, sizeof(dt));
            for(SizeType x = 0; x < m.row(); x++)
            {
                for(SizeType y = 0; y < m.column(); y++)
                {
                    SizeType tempx = i - center_row + x, tempy = j - center_column + y;
                    if(tempx < 0 || tempy < 0 || tempx >= row() || tempy >= column()) continue;
                    dt.construct(dt.content() + get_value(tempx, tempy) * m.get_value(x, y));
                }
            }
            result.set_value(i, j, dt.ref_content());
        }
    }
    swap(result);
}

template<typename _Tp>
void Matrix<_Tp>::T()
{
    Matrix<_Tp> temp(column(), row());
    for(SizeType i = 0; i < row(); i++)
    {
        for(SizeType j = 0; j < column(); j++)
        {
            temp.set_value(j, i, get_value(i, j));
        }
    }
    swap(temp);
}

template<typename _Tp>
void Matrix<_Tp>::power(SizeType p)
{
    if(row() != column())
    { throw SizeDoesNotMatchException("exception: target size does not match from source size!"); }
    Matrix<_Tp> temp(*this);
    for(SizeType i = 1; i < p; i++)
    {
        temp.multiply(*this);
    }
    swap(temp);
}


};

#endif // MATRIX_H
