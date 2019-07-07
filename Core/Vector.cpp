#include "Core/Vector.h"
#include "Core/Exception.h"

template<typename T>
void rapid::Vector<T>::_initialize(SizeType s)
{
    NodeBase<Type> *temp = _Data;
    _Data = new NodeBase<Type>[s];
    if(temp != nullptr)
    { mem_copy(_Data, temp, size() * sizeof(Type)); }
    _Capacity = s;
}

template<typename T>
void rapid::Vector<T>::_copy_data(const Vector<T> &v)
{
    _Data = new NodeBase<Type>[v.capacity()];
    { mem_copy(_Data, v._Data, v.size() * sizeof(Type)); }
    _Size = v.size();
    _Capacity = v.capacity();
}

template<typename T>
void rapid::Vector<T>::_insert(const iterator &it, ConstReference arg)
{
    if(size() >= capacity())
    { _growth(); }
    if(it == end())
    { _Data[size()].construct(arg); }
    else
    {
        mem_backward(_Data[it.__CurrentIndex].address(), (size() - it.__CurrentIndex) * sizeof(Type), sizeof(Type));
        _Data[it.__CurrentIndex].construct(arg);
    }
    _add_size(1);
}

template<typename T>
void rapid::Vector<T>::_erase(const iterator &it)
{
    if(it == end()) return;
    mem_forward(_Data[it.__CurrentIndex + 1].address(), (size() - it.__CurrentIndex) * sizeof(Type), sizeof(Type));
    _add_size(-1);
}

template<typename T>
void rapid::Vector<T>::_growth()
{
    if(_Growth < 1)
    { _Capacity *= 2; }
    else
    { _Capacity += _Growth; }
}

template<typename T>
typename rapid::Vector<T>::Reference rapid::Vector<T>::at(const SizeType index)
{
    if(index < 0 || index >= size())
    { throw Exception("index out of array"); }
    else
    { return this->operator[](index); }
}

template<typename T>
void rapid::Vector<T>::resize(SizeType s)
{
    if(s > capacity())
    { _initialize(s); }
    else if(s > size())
    { mem_clear(_Data[0].address() + size(), (s - size()) * sizeof(Type)); }
    else
    { mem_clear(_Data[0].address() + s, (size() - s) * sizeof(Type)); }
    _Size = s;
}

#ifndef NDEBUG
void rapid::test_Vector_main()
{
    Vector<long> v;
    v.push_back(10);
    v.push_back(30);
    v.push_back(50);
    v.push_back(70);
    v.push_back(20);
    v.push_back(40);
    for(long value : v)
    {
        std::cout << value << std::endl;
    }
}
#endif
