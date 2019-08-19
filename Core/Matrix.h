#ifndef MATRIX_H
#define MATRIX_H

#include "Version.h"
#include "Memory.h"
#include <initializer_list>
namespace rapid
{
/* type _Tp need implements:
 * _Tp operator+(_Tp)
 * _Tp operator*(_Tp)
 * _Tp operator=(_Tp)
 */
template<typename _Tp>
class Matrix
{
private:
    using Type = _Tp;
    using Reference = Type&;
    using ConstReference = const Type &;
    using RvalueReference = Type &&;
    using SizeType = long;
    using DataType = NodeBase<Type>;

    using MatrixRef = Matrix<_Tp>&;
    using RvalueMatrixRef = Matrix<_Tp> &&;
protected:
    DataType **_M_data = nullptr;
    SizeType _M_row = 0;
    SizeType _M_column = 0;

    void _F_copy(MatrixRef m);
    void _F_resize(SizeType r, SizeType c);
    void _F_construct_default(SizeType r, SizeType c, ConstReference v);
    void _F_set_value(SizeType r, SizeType c, ConstReference v)
    { _M_data[r][c].ref_content() = v; }
    void _F_multiply(MatrixRef m);
    void _F_multiply(SizeType r, ConstReference n);
    void _F_add(MatrixRef m);
    void _F_filter(MatrixRef m);

    static void _clear(DataType **mem, SizeType r);
    static Matrix<_Tp>* _F_multiply(MatrixRef m1, MatrixRef m2);
public:
    Matrix(SizeType r = 1, SizeType c = 1)
    { _F_resize(r, c); }
    Matrix(SizeType r, SizeType c, ConstReference default_value)
    { _F_construct_default(r, c, default_value); }
    Matrix(SizeType r, SizeType c, RvalueReference default_value)
    { _F_construct_default(r, c, std::forward<Type>(default_value)); }
    Matrix(MatrixRef m)
    { _F_copy(m); }
    Matrix(RvalueMatrixRef m)
    { _F_copy(std::forward<Matrix<_Tp>>(m)); }
    Matrix(std::initializer_list<std::initializer_list<Type>> m);
    virtual ~Matrix() { }

    void set_value(SizeType r, SizeType c, ConstReference v)
    { _F_set_value(r, c, v); }
    void set_value(SizeType r, SizeType c, RvalueReference v)
    { _F_set_value(r, c, std::forward<Type>(v)); }

    Type get_value(SizeType r, SizeType c)
    { return _M_data[r][c].content(); }

    SizeType row()
    { return _M_row; }
    SizeType column()
    { return _M_column; }

    void T();

    /* [r] line multiply [n]
     * param[r]: given line
     * param[n]: given value
     */
    void multiply(SizeType r, ConstReference n)
    { _F_multiply(r, n); }
    void multiply(SizeType r, RvalueReference n)
    { _F_multiply(r, std::forward<Type>(n)); }
    void multiply(MatrixRef m)
    { _F_multiply(m); }
    void multiply(RvalueMatrixRef m)
    { _F_multiply(std::forward<Matrix<_Tp>>(m)); }

    void add(const Matrix &m)
    { _F_add(m); }
    void add(Matrix &&m)
    { _F_add(std::forward<Matrix<_Tp>>(m)); }

    void filter(MatrixRef m)
    { _F_filter(m); }
    void filter(RvalueMatrixRef m)
    { _F_filter(std::forward<Matrix<_Tp>>(m)); }

    void power(SizeType p);

    // need call by manual
    void clear()
    { _clear(_M_data, row()); }

    static Matrix<_Tp>* multiply(MatrixRef m1, MatrixRef m2)
    { return _F_multiply(m1, m2); }
    static Matrix<_Tp>* multiply(MatrixRef m1, RvalueMatrixRef m2)
    { return _F_multiply(m1, std::forward<Matrix<_Tp>>(m2)); }
    static Matrix<_Tp>* multiply(RvalueMatrixRef m1, MatrixRef m2)
    { return _F_multiply(std::forward<Matrix<_Tp>>(m1), m2); }
    static Matrix<_Tp>* multiply(RvalueMatrixRef m1, RvalueMatrixRef m2)
    { return _F_multiply(std::forward<Matrix<_Tp>>(m1), std::forward<Matrix<_Tp>>(m2)); }

    // copy [m]'s data reference to [this]
    void copy_from(MatrixRef m)
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
    }
    void clear_and_copy(MatrixRef m)
    { 
        clear();
        copy_from(m);
    }

    Matrix<_Tp>* operator*(MatrixRef m)
    { return _F_multiply(*this, m); }
    Matrix<_Tp>* operator*(RvalueMatrixRef m)
    { return _F_multiply(*this, std::forward<Matrix<_Tp>>(m)); }

    Matrix<_Tp>* operator*=(MatrixRef m)
    {
        multiply(m);
        return this;
    }
    Matrix<_Tp>* operator*=(RvalueMatrixRef m)
    {
        multiply(std::forward<Matrix<_Tp>>(m));
        return this;
    }
    MatrixRef operator=(MatrixRef m)
    {
        _F_copy(m);
        return *this;
    }
    MatrixRef operator=(RvalueMatrixRef m)
    {
        _F_copy(std::forward<Matrix<_Tp>>(m));
        return *this;
    }
    Matrix<_Tp>* operator+(MatrixRef m)
    {
        Matrix<_Tp> *temp = new Matrix<_Tp>(*this);
        temp->_F_add(m);
        return temp;
    }
    Matrix<_Tp>* operator+(RvalueMatrixRef m)
    {
        Matrix<_Tp> *temp = new Matrix<_Tp>(*this);
        temp->_F_add(std::forward<Matrix<_Tp>>(m));
        return temp;
    }
    MatrixRef operator+=(MatrixRef m)
    {
        _F_add(m);
        return *this;
    }
    MatrixRef operator+=(RvalueMatrixRef m)
    {
        _F_add(std::forward<Matrix<_Tp>>(m));
        return *this;
    }

};

#ifndef NDEBUG
void test_Matrix_main();
#endif

};

#endif // MATRIX_H
