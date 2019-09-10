#ifndef VECTOR_CPP
#define VECTOR_CPP

#include "Vector.h"
#include "Core/Exception.h"
#include <iostream>

template<typename T>
rapid::Vector<T>::~Vector()
{
    clear();
}

template<typename T>
void rapid::Vector<T>::clear()
{
    if(_M_data != nullptr)
    {
        delete[] _M_data;
        _M_data = nullptr;
    }
}

template<typename T>
void rapid::Vector<T>::_F_initialize(SizeType s)
{
    NodeBase<ValueType> *temp = _M_data;
    _M_data = new NodeBase<ValueType>[s];
    if(temp != nullptr)
    {
        mem_copy(_M_data, temp, size() > s ? s : size() * sizeof(ValueType));
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
        _M_data = new NodeBase<ValueType>[capacity()];
        mem_copy(_M_data[0].address(), v._M_data[0].address(), _M_size * sizeof(ValueType));
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
        mem_backward(_M_data[it._M_current_index].address(), (size() - it._M_current_index) * sizeof(ValueType), sizeof(ValueType));
        _M_data[it._M_current_index].construct(arg);
    }
    _F_add_size(1);
}

template<typename T>
void rapid::Vector<T>::_F_erase(const iterator &it)
{
    if(it == end()) return;
    mem_forward(_M_data[it._M_current_index + 1].address(), (size() - it._M_current_index) * sizeof(ValueType), sizeof(ValueType));
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
    { mem_clear(_M_data[0].address() + size(), (s - size()) * sizeof(ValueType)); }
    else
    {
        mem_clear(_M_data[0].address() + s, (size() - s) * sizeof(ValueType));
        _M_size = s;
    }
}

template<typename T>
typename rapid::Vector<T>::iterator rapid::Vector<T>::find(ValueType arg)
{
    for(SizeType i = 0; i < size(); i++)
    {
        if(_M_data[i].content() == arg)
            return iterator(i, size() - 1, _M_data[0].address());
    }
    return end();
}

template<typename T>
static void print_vector(rapid::Vector<T> &v)
{
    for(T value : v)
        std::cout << value << " ";
    std::cout << std::endl;
}
void rapid::test_Vector_main()
{
    std::cout << "-----------test_Vector_main-----------" << std::endl;
    Vector<long> v;
    v.push_back(10);
    v.push_back(30);
    print_vector(v);
    v.push_back(50);
    v.push_back(70);
    print_vector(v);
    v.push_back(20);
    v.push_back(40);
    std::cout << "size = " << v.size() << std::endl;
    print_vector(v);
    std::cout << "------------------------------" << std::endl;
    v.erase(v.find(10));
    v.pop_back();
    v.pop_front();
    std::cout << "size = " << v.size() << std::endl;
    print_vector(v);
    std::cout << "------------------------------" << std::endl;
    v.erase(v.find(10));
    v.push_back(100);
    v.push_back(200);
    v.push_front(0);
    v.insert(v.find(100), 90);
    std::cout << "size = " << v.size() << std::endl;
    print_vector(v);
    std::cout << "------------------------------" << std::endl;
    Vector<long> vec(v);
    std::cout << "size = " << vec.size() << std::endl;
    std::cout << "front: " << vec.front() << std::endl;
    std::cout << "back: " << vec.back() << std::endl;
    try
    {
        std::cout << "the element at index 3 is: " << vec.at(3) << std::endl;
        std::cout << "the element at index 10 is: " << vec.at(10) << std::endl;
    }
    catch (IndexOutOfArrayException &e)
    {
        std::cout << e.what() << std::endl;
    }
    std::cout << "size = " << vec.size() << std::endl;
    print_vector(vec);
    std::cout << "---------------test end---------------" << std::endl;
}

#endif // end VECTOR_CPP
