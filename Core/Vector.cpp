#include "Core/Vector.h"
#include "Core/Exception.h"

template<typename T>
rapid::Vector<T>::~Vector()
{
    if(_Data != nullptr)
    { delete[] _Data; }
}

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
void rapid::Vector<T>::_copy_data(Vector<T> &v)
{
    if(_Data != nullptr)
    {
        delete[] _Data;
        _Data = nullptr;
    }
    if(_Capacity > 0)
    {
        _Data = new NodeBase<Type>[_Capacity];
        mem_copy(_Data[0].address(), v._Data[0].address(), _Size * sizeof(Type));
    }
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
typename rapid::Vector<T>::ConstReference rapid::Vector<T>::at(const SizeType index)
{
    if(index < 0 || index >= size())
    { throw IndexOutOfArrayException("exception: index out of array"); }
    else
    { return (*this)[index]; }
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

template<typename T>
typename rapid::Vector<T>::iterator rapid::Vector<T>::find(Type arg)
{
    for(SizeType i = 0; i < size(); i++)
    {
        if(_Data[i].content() == arg)
            return iterator(i, size() - 1, _Data[0].address());
    }
    return end();
}

#ifndef NDEBUG
void rapid::test_Vector_main()
{
    std::cout << "-----------test_Vector_main-----------" << std::endl;
    Vector<long> v;
    v.push_back(10);
    v.push_back(30);
    v.push_back(50);
    v.push_back(70);
    v.push_back(20);
    v.push_back(40);
    std::cout << "size = " << v.size() << std::endl;
    for(long value : v)
    {
        std::cout << value << std::endl;
    }
    std::cout << "------------------------------" << std::endl;
    v.erase(v.find(10));
    v.pop_back();
    v.pop_front();
    std::cout << "size = " << v.size() << std::endl;
    for(long value : v)
    {
        std::cout << value << std::endl;
    }
    std::cout << "------------------------------" << std::endl;
    v.erase(v.find(10));
    v.push_back(100);
    v.push_back(200);
    v.push_front(0);
    v.insert(v.find(100), 90);
    std::cout << "size = " << v.size() << std::endl;
    for(long value : v)
    {
        std::cout << value << std::endl;
    }
    std::cout << "------------------------------" << std::endl;
    Vector<long> vec(v);
    std::cout << "size1 = " << vec.size() << std::endl;
    std::cout << "front: " << vec.front() << std::endl;
    std::cout << "back: " << vec.back() << std::endl;
    try
    {
        long temp = vec.at(3);
        std::cout << "the element at index 3 is: " << temp << std::endl;
        temp = vec.at(10);
        std::cout << "the element at index 10 is: " << temp << std::endl;
    }
    catch (IndexOutOfArrayException e)
    {
        std::cout << e.what() << std::endl;
    }
    std::cout << "size = " << vec.size() << std::endl;
    for(long value : vec)
    {
        std::cout << value << std::endl;
    }
    std::cout << "---------------test end---------------" << std::endl;
}
#endif
