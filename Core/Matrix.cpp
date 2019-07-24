#include "Matrix.h"
#include "Exception.h"

template<typename _Tp>
void rapid::Matrix<_Tp>::_copy(MatrixRef m)
{
    _resize(m.row(), m.column());
    for(SizeType i = 0; i < row(); i++)
    {
        for(SizeType j = 0; j < column(); j++)
        { 
            set_value(i, j, m.get_value(i, j));
        }
    }
}

template<typename _Tp>
void rapid::Matrix<_Tp>::_resize(SizeType r, SizeType c)
{
    _clear(_Data, row());
    _Row = r;
    _Column = c;
    _Data = new DataType*[row()];
    for(SizeType i = 0; i < row(); i++)
    {
        _Data[i] = new DataType[column()];
    }
}

template<typename _Tp>
void rapid::Matrix<_Tp>::_construct_default(SizeType r, SizeType c, ConstReference value)
{
    _clear(_Data, row());
    _Row = r;
    _Column = c;
    _Data = new DataType*[row()];
    for(SizeType i = 0; i < row(); i++)
    {
        _Data[i] = new DataType[column()];
        for(SizeType j = 0; j < column(); j++)
        {
            _Data[i][j].construct(value);
        }
    }
}

template<typename _Tp>
void rapid::Matrix<_Tp>::_clear(DataType **mem, SizeType r)
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
void rapid::Matrix<_Tp>::_multiply(MatrixRef m)
{
    if(row() != m.column() || column() != m.row())
    { throw SizeDoesNotMatchException("exception: target size does not match from source size!"); }
    Matrix<_Tp> *temp = _multiply(*this, m);
    clear_and_copy(*temp);
    delete temp;
}

template<typename _Tp>
void rapid::Matrix<_Tp>::_multiply(SizeType r, ConstReference n)
{
    for(SizeType c = 0; c < column(); c++)
    { _Data[r][c] = _Data[r][c] * n; }
}

template<typename _Tp>
rapid::Matrix<_Tp>* rapid::Matrix<_Tp>::_multiply(MatrixRef m1, MatrixRef m2)
{ 
    if(m1.row() != m2.column() || m1.column() != m2.row())
    { throw SizeDoesNotMatchException("exception: target size does not match from source size!"); }
    Matrix<_Tp> *result = new Matrix<_Tp>(m1.row(), m2.column());
    for(SizeType i = 0; i < result->row(); i++)
    {
        for(SizeType j = 0; j < result->column(); j++)
        {
            Type temp = m1.get_value(i, 0) * m2.get_value(0, j);
            for(SizeType m = 1; m < m1.column(); m++)
            {
                temp = temp + m1.get_value(i, m) * m2.get_value(m, j);
            }
            result->set_value(i, j, temp); 
        }
    }
    return result;
}

template<typename _Tp>
void rapid::Matrix<_Tp>::_add(MatrixRef m)
{
    if(row() != m.row() || column() != m.column())
    { throw SizeDoesNotMatchException("exception: target size does not match from source size!"); }
    for(SizeType i = 0; i < row(); i++)
    {
        for(SizeType j = 0; j < column(); j++)
        {
            _Data[i][j] = _Data[i][j] + m._Data[i][j];
        }
    }
}

template<typename _Tp>
void rapid::Matrix<_Tp>::_filter(MatrixRef m)
{

}

template<typename _Tp>
void rapid::Matrix<_Tp>::T()
{
    Matrix<_Tp> temp(column(), row());
    for(SizeType i = 0; i < row(); i++)
    {
        for(SizeType j = 0; j < column(); j++)
        {
            temp.set_value(j, i, get_value(i, j));
        }
    }
    clear_and_copy(temp);
}

template<typename _Tp>
void rapid::Matrix<_Tp>::power(SizeType p)
{
    if(row() != column())
    { throw SizeDoesNotMatchException("exception: target size does not match from source size!"); }
    Matrix<_Tp> temp(*this);
    for(SizeType i = 0; i < p - 1; i++)
    {
        temp.multiply(*this);
    }
    clear_and_copy(temp);
}

#ifndef NDEBUG

#include <iomanip>
using SizeType = unsigned long;
template<typename T>
void print_matrix(rapid::Matrix<T> &m)
{
    std::cout << std::setiosflags(std::ios::left);
    for(SizeType i = 0; i < m.row(); i++)
    {
        for(SizeType j = 0; j < m.column(); j++)
        {
            std::cout << std::setw(4) << m.get_value(i, j);
        }
        std::cout << std::endl;
    }
    std::cout << std::resetiosflags(std::ios::showbase);
}

void rapid::test_Matrix_main()
{
    std::cout << "-----------test Matrix start----------" << std::endl;
    Matrix<int> m1(5, 5, 2);
    print_matrix(m1);
    std::cout << "---------------------" << std::endl;
    m1.set_value(0, 1, 0);
    m1.set_value(1, 2, 0);
    m1.set_value(2, 3, 0);
    m1.set_value(3, 4, 0);
    print_matrix(m1);
    std::cout << "---------------------" << std::endl;
    Matrix<int> m2(3, 4, 3);
    Matrix<int> m3(m1);
    Matrix<int> *m4;
    try
    {
        m4 = Matrix<int>::multiply(m2, m3);
    }
    catch(Exception e)
    {
        std::cout << e.what() << std::endl;
    }
    m3.multiply(m1);
    print_matrix(m3);
    std::cout << "---------------------" << std::endl;
    m2.T();
    print_matrix(m2);
    std::cout << "---------------------" << std::endl;
    m1.power(2);
    print_matrix(m1);
    std::cout << "---------------------" << std::endl;
    Matrix<int> m5(4, 3, 2);
    m2.T();
    m5 *= m2;
    print_matrix(m5);
    std::cout << "-----------test Matrix end----------" << std::endl;

}

#endif
