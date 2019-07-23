#ifndef MATRIX_H
#define MATRIX_H

#include "Version.h"
#include "Memory.h"
namespace rapid
{

template<typename _Tp>
class Matrix
{
private:
    using Type = _Tp;
    using Reference = Type&;
    using ConstReference = const Type &;
    using RvalueReference = Type &&;
    using SizeType = unsigned long;
    using DataType = NodeBase<Type>;

    using ConstMatrixRef = const Matrix<_Tp>&;
    using RvalueMatrixRef = Matrix<_Tp> &&;
protected:
    DataType **_Data;
    SizeType _Row;
    SizeType _Column;

    void _resize(SizeType r, SizeType c);
    void _construct_default(SizeType r, SizeType c, ConstReference v);
    void _set_value(SizeType r, SizeType c, ConstReference v)
    { _Data[r][c] = v; }
    void _multiply(ConstMatrixRef m);
    void _multiply(SizeType r, ConstReference n);
    void _add(ConstMatrixRef m);
    void _filter(ConstMatrixRef m);

    static Matrix<_Tp>* _multiply(ConstMatrixRef m1, ConstMatrixRef m2);
public:
    Matrix(SizeType r = 1, SizeType c = 1) : _Row(r), _Column(c)
    { _resize(r, c); }
    Matrix(SizeType r, SizeType c, ConstReference default_value) : _Row(r), _Column(c)
    { _construct_default(r, c, default_value); }
    Matrix(ConstMatrixRef m)
    { _copy(m); }
    Matrix(RvalueMatrixRef m)
    { _copy(std::forward<Matrix<_Tp>>(m)); }
    virtual ~Matrix() { }

    void set_value(SizeType r, SizeType c, ConstReference v)
    { _set_value(r, c, v); }
    void set_value(SizeType r, SizeType c, RvalueReference v)
    { _set_value(r, c, std::forward<Type>(v)); }

    Type get_value(SizeType r, SizeType c)
    { return _Data[r][c].content(); }

    SizeType row()
    { return _Row; }
    SizeType column()
    { return _Column; }

    void T();

    /* [r] line multiply [n]
     * param[r]: given line
     * param[n]: given value
     */
    void multiply(SizeType r, ConstReference n)
    { _multiply(r, n); }
    void multiply(SizeType r, RvalueReference n)
    { _multiply(r, std::forward<Type>(n)); }
    void multiply(ConstMatrixRef m)
    { _multiply(m); }
    void multiply(RvalueMatrixRef m)
    { _multiply(std::forward<Matrix<_Tp>>(m)); }

    void add(const Matrix &m)
    { _add(m); }
    void add(Matrix &&m)
    { _add(std::forward<Matrix<_Tp>>(m)); }

    void filter(ConstMatrixRef m)
    { _filter(m); }
    void filter(RvalueMatrixRef m)
    { _filter(std::forward<Matrix<_Tp>>(m)); }

    void power(SizeType p);

    // need call by manual
    void clear();

    static Matrix<_Tp>* multiply(ConstMatrixRef m1, ConstMatrixRef m2)
    { return _multiply(m1, m2); }
    static Matrix<_Tp>* multiply(ConstMatrixRef m1, RvalueMatrixRef m2)
    { return _multiply(m1, std::forward<Matrix<_Tp>>(m2)); }
    static Matrix<_Tp>* multiply(RvalueMatrixRef m1, ConstMatrixRef m2)
    { return _multiply(std::forward<Matrix<_Tp>>(m1), m2); }
    static Matrix<_Tp>* multiply(RvalueMatrixRef m1, RvalueMatrixRef m2)
    { return _multiply(std::forward<Matrix<_Tp>>(m1), std::forward<Matrix<_Tp>>(m2)); }

    // copy [m]'s data reference to [this]
    void copy_from(ConstMatrixRef m)
    { 
        _Data = m._Data;
        _Row = m._Row;
        _Column = m._Column;
    }
    void swap(ConstMatrixRef m)
    { 
        Matrix<_Tp> temp;
        temp.copy_from(m);
        m.copy_from(*this);
        this->copy_from(temp);
    }
    void clear_and_copy(ConstMatrixRef m)
    { 
        clear();
        copy_from(m);
    }

    Matrix<_Tp>* operator*(ConstMatrixRef m)
    { return _multiply(*this, m); }
    Matrix<_Tp>* operator*(RvalueMatrixRef m)
    { return _multiply(*this, std::forward<Matrix<_Tp>>(m)); }
};

}

#endif // MATRIX_H
