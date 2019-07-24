#ifndef MATRIX_H
#define MATRIX_H

#include "Version.h"
#include "Memory.h"
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
    using SizeType = unsigned long;
    using DataType = NodeBase<Type>;

    using MatrixRef = Matrix<_Tp>&;
    using RvalueMatrixRef = Matrix<_Tp> &&;
protected:
    DataType **_Data = nullptr;
    SizeType _Row = 0;
    SizeType _Column = 0;

    void _copy(MatrixRef m);
    void _resize(SizeType r, SizeType c);
    void _construct_default(SizeType r, SizeType c, ConstReference v);
    void _set_value(SizeType r, SizeType c, ConstReference v)
    { _Data[r][c].ref_content() = v; }
    void _multiply(MatrixRef m);
    void _multiply(SizeType r, ConstReference n);
    void _add(MatrixRef m);
    void _filter(MatrixRef m);

    static void _clear(DataType **mem, SizeType r);
    static Matrix<_Tp>* _multiply(MatrixRef m1, MatrixRef m2);
public:
    Matrix(SizeType r = 1, SizeType c = 1)
    { _resize(r, c); }
    Matrix(SizeType r, SizeType c, ConstReference default_value)
    { _construct_default(r, c, default_value); }
    Matrix(SizeType r, SizeType c, RvalueReference default_value)
    { _construct_default(r, c, std::forward<Type>(default_value)); }
    Matrix(MatrixRef m)
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
    void multiply(MatrixRef m)
    { _multiply(m); }
    void multiply(RvalueMatrixRef m)
    { _multiply(std::forward<Matrix<_Tp>>(m)); }

    void add(const Matrix &m)
    { _add(m); }
    void add(Matrix &&m)
    { _add(std::forward<Matrix<_Tp>>(m)); }

    void filter(MatrixRef m)
    { _filter(m); }
    void filter(RvalueMatrixRef m)
    { _filter(std::forward<Matrix<_Tp>>(m)); }

    void power(SizeType p);

    // need call by manual
    void clear()
    { _clear(_Data, row()); }

    static Matrix<_Tp>* multiply(MatrixRef m1, MatrixRef m2)
    { return _multiply(m1, m2); }
    static Matrix<_Tp>* multiply(MatrixRef m1, RvalueMatrixRef m2)
    { return _multiply(m1, std::forward<Matrix<_Tp>>(m2)); }
    static Matrix<_Tp>* multiply(RvalueMatrixRef m1, MatrixRef m2)
    { return _multiply(std::forward<Matrix<_Tp>>(m1), m2); }
    static Matrix<_Tp>* multiply(RvalueMatrixRef m1, RvalueMatrixRef m2)
    { return _multiply(std::forward<Matrix<_Tp>>(m1), std::forward<Matrix<_Tp>>(m2)); }

    // copy [m]'s data reference to [this]
    void copy_from(MatrixRef m)
    { 
        _Data = m._Data;
        _Row = m._Row;
        _Column = m._Column;
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
    { return _multiply(*this, m); }
    Matrix<_Tp>* operator*(RvalueMatrixRef m)
    { return _multiply(*this, std::forward<Matrix<_Tp>>(m)); }

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
        _copy(m);
        return *this;
    }
    MatrixRef operator=(RvalueMatrixRef m)
    {
        _copy(std::forward<Matrix<_Tp>>(m));
        return *this;
    }
    Matrix<_Tp>* operator+(MatrixRef m)
    {
        Matrix<_Tp> *temp = new Matrix<_Tp>(*this);
        temp->_add(m);
        return temp;
    }
    Matrix<_Tp>* operator+(RvalueMatrixRef m)
    {
        Matrix<_Tp> *temp = new Matrix<_Tp>(*this);
        temp->_add(std::forward<Matrix<_Tp>>(m));
        return temp;
    }
    MatrixRef operator+=(MatrixRef m)
    {
        _add(m);
        return *this;
    }
    MatrixRef operator+=(RvalueMatrixRef m)
    {
        _add(std::forward<Matrix<_Tp>>(m));
        return *this;
    }

};

#ifndef NDEBUG
void test_Matrix_main();
#endif

};

#endif // MATRIX_H
