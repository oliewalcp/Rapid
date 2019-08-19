#include "Core/Vector.h"
#include "Core/Exception.h"

template<typename T>
rapid::Vector<T>::~Vector()
{
    if(_M_data != nullptr)
    { delete[] _M_data; }
}

template<typename T>
void rapid::Vector<T>::clear()
{
    if(_M_data != nullptr)
        delete[] _M_data;
}

template<typename T>
void rapid::Vector<T>::_F_initialize(SizeType s)
{
    NodeBase<Type> *temp = _M_data;
    _M_data = new NodeBase<Type>[s];
    if(temp != nullptr)
    {
        mem_copy(_M_data, temp, size() * sizeof(Type));
        delete[] temp;
    }
    _M_capacity = s;
}

template<typename T>
void rapid::Vector<T>::_F_copy_data(Vector<T> &v)
{
    if(_M_data != nullptr)
    {
        delete[] _M_data;
        _M_data = nullptr;
    }
    if(capacity() > 0)
    {
        _M_data = new NodeBase<Type>[capacity()];
        mem_copy(_M_data[0].address(), v._M_data[0].address(), _M_size * sizeof(Type));
    }
}

template<typename T>
void rapid::Vector<T>::_F_insert(const iterator &it, ConstReference arg)
{
    if(size() >= capacity())
    { _F_growth(); }
    if(it == end())
    { _M_data[size()].construct(arg); }
    else
    {
        mem_backward(_M_data[it._M_current_index].address(), (size() - it._M_current_index) * sizeof(Type), sizeof(Type));
        _M_data[it._M_current_index].construct(arg);
    }
    _F_add_size(1);
}

template<typename T>
void rapid::Vector<T>::_F_erase(const iterator &it)
{
    if(it == end()) return;
    mem_forward(_M_data[it._M_current_index + 1].address(), (size() - it._M_current_index) * sizeof(Type), sizeof(Type));
    _F_add_size(-1);
}

template<typename T>
void rapid::Vector<T>::_F_growth()
{
    if(_M_growth < 1)
    { _F_initialize(capacity() * 2); }
    else
    { _F_initialize(capacity() + _M_growth); }
}

template<typename T>
typename rapid::Vector<T>::ConstReference rapid::Vector<T>::at(const SizeType index)
{
    if(index < 0 || index >= size())
    { throw IndexOutOfArrayException("exception: index out of array !"); }
    else
    { return (*this)[index]; }
}

template<typename T>
void rapid::Vector<T>::resize(SizeType s)
{
    if(s > capacity())
    { _F_initialize(s); }
    else if(s > size())
    { mem_clear(_M_data[0].address() + size(), (s - size()) * sizeof(Type)); }
    else
    { mem_clear(_M_data[0].address() + s, (size() - s) * sizeof(Type)); }
    _M_size = s;
}

template<typename T>
typename rapid::Vector<T>::iterator rapid::Vector<T>::find(Type arg)
{
    for(SizeType i = 0; i < size(); i++)
    {
        if(_M_data[i].content() == arg)
            return iterator(i, size() - 1, _M_data[0].address());
    }
    return end();
}
