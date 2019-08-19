#include "Matrix.h"
#include "Exception.h"

template<typename _Tp>
rapid::Matrix<_Tp>::Matrix(std::initializer_list<std::initializer_list<Type>> m)
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
        for(Type t : *it)
        {
            set_value(i, j++, t);
        }
    }
}

template<typename _Tp>
void rapid::Matrix<_Tp>::_F_copy(MatrixRef m)
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
void rapid::Matrix<_Tp>::_F_resize(SizeType r, SizeType c)
{
    _clear(_M_data, row());
    _M_row = r;
    _M_column = c;
    _M_data = new DataType*[static_cast<unsigned long long>(row())];
    for(SizeType i = 0; i < row(); i++)
    {
        _M_data[i] = new DataType[static_cast<unsigned long long>(column())];
    }
}

template<typename _Tp>
void rapid::Matrix<_Tp>::_F_construct_default(SizeType r, SizeType c, ConstReference value)
{
    _clear(_M_data, row());
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
void rapid::Matrix<_Tp>::_F_multiply(MatrixRef m)
{
    if(row() != m.column() || column() != m.row())
    { throw SizeDoesNotMatchException("exception: target size does not match from source size!"); }
    Matrix<_Tp> *temp = _F_multiply(*this, m);
    clear_and_copy(*temp);
    delete temp;
}

template<typename _Tp>
void rapid::Matrix<_Tp>::_F_multiply(SizeType r, ConstReference n)
{
    for(SizeType c = 0; c < column(); c++)
    { _M_data[r][c] = _M_data[r][c] * n; }
}

template<typename _Tp>
rapid::Matrix<_Tp>* rapid::Matrix<_Tp>::_F_multiply(MatrixRef m1, MatrixRef m2)
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
void rapid::Matrix<_Tp>::_F_add(MatrixRef m)
{
    if(row() != m.row() || column() != m.column())
    { throw SizeDoesNotMatchException("exception: target size does not match from source size!"); }
    for(SizeType i = 0; i < row(); i++)
    {
        for(SizeType j = 0; j < column(); j++)
        {
            _M_data[i][j] = _M_data[i][j] + m._M_data[i][j];
        }
    }
}

template<typename _Tp>
void rapid::Matrix<_Tp>::_F_filter(MatrixRef m)
{
    Matrix<_Tp> result(row(), column());
    SizeType center_row = (m.row() - 1) / 2, center_column = (m.column() - 1) / 2;
    for(SizeType i = 0; i < row(); i++)
    {
        for(SizeType j = 0; j < column(); j++)
        {
            DataType dt;
            dt.clear();
            for(SizeType x = 0; x < m.row(); x++)
            {
                for(SizeType y = 0; y < m.column(); y++)
                {
                    SizeType tempx = i - center_row + x, tempy = j - center_column + y;
                    if(tempx < 0 || tempy < 0 || tempx >= row() || tempy >= column()) continue;
                    dt.construct(dt.content() + m.get_value(x, y) * get_value(tempx, tempy));
                }
            }
            result.set_value(i, j, dt.content());
        }
    }
    clear_and_copy(result);
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

