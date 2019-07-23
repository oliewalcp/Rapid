#include "Matrix.h"
#include "Exception.h"

template<typename _Tp>
void rapid::Matrix<_Tp>::_resize(SizeType r, SizeType c)
{
    _Row = r;
    _Column = c;
    DataType **temp = _Data;
    _Data = new DataType*[row()];
    for(SizeType i = 0; i < row(); i++)
    {
        _Data[i] = new DataType[column()];
    }
}

template<typename _Tp>
void rapid::Matrix<_Tp>::_construct_default(SizeType r, SizeType c, ConstReference value)
{
    _Row = r;
    _Column = c;
    DataType **temp = _Data;
    _Data = new DataType*[row()];
    for(SizeType i = 0; i < row(); i++)
    {
        _Data[i] = new DataType[column()];
        for(SizeType j = 0; j < column(); j++)
        {
            _Data[i][j].contruct(value);
        }
    }
}

template<typename _Tp>
void rapid::Matrix<_Tp>::clear()
{
    if(_Data == nullptr) return;
    for(SizeType i = 0; i < row(); i++)
    {
        delete[] _Data[i];
    }
    delete[] _Data;
    _Data = nullptr;
}

template<typename _Tp>
void rapid::Matrix<_Tp>::_multiply(ConstMatrixRef m)
{
    if(row() != m.column() || column() != m.row())
    { throw SizeDoesNotMatchException("exception: target size does not match from source size!"); }
    Matrix<_Tp> *temp = _multiply(*this, m);
    clear_and_copy(&temp);
    delete temp;
}

template<typename _Tp>
void rapid::Matrix<_Tp>::_multiply(SizeType r, ConstReference n)
{
    for(SizeType c = 0; c < column(); c++)
    { _Data[r][c] = _Data[r][c] * n; }
}

template<typename _Tp>
rapid::Matrix<_Tp>* rapid::Matrix<_Tp>::_multiply(ConstMatrixRef m1, ConstMatrixRef m2)
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
                temp = m1.get_value(i, m) * m2.get_value(m, j);
            }
            result->set_value(i, j, temp); 
        }
    }
    return result;
}

template<typename _Tp>
void rapid::Matrix<_Tp>::_add(ConstMatrixRef m)
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
void rapid::Matrix<_Tp>::_filter(ConstMatrixRef m)
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
    clear_and_copy(m);
}

template<typename _Tp>
void rapid::Matrix<_Tp>::power(SizeType p)
{
    if(row() != column())
    { throw SizeDoesNotMatchException("exception: target size does not match from source size!"); }
    Matrix<_Tp> temp(*this);
    for(SizeType i = 0; i < row() - 1; i++)
    {
        temp.multiply(*this);
    }
    clear_and_copy(m);
}
