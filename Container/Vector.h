#ifndef VECTOR_H
#define VECTOR_H
#include "C_Base.h"
#include "../Exception.h"

template<typename _Value>
class Vector
{
private:
    typedef unsigned int __uint32;

    _C_Base<_Value> *__Data;
    __uint32 __Size;
    __uint32 __Capacity;

    inline _C_Base<_Value>* __get_data(const _Value& arg)
    { return (_C_Base<_Value>*)(&arg);}
    inline _C_Base<_Value>* __create_array()
    { return new _C_Base<_Value>[__Capacity]; }

    _Value& operator[](__uint32 index)
    {
        if(index >= __Size) throw Exception("IndexOutOfArray");
        return __Data[index].Data;
    }

    void __push_back(const _Value& arg)
    {
        if(__Data == nullptr)
        {
            __create_array();
            __Data[__Size] = __get_data(arg);
        }
        else if(__Size == __Capacity)
        {
            __Capacity *= 2;
            _C_Base<_Value> *temp = __create_array();
            memcpy(temp, __Data, sizeof(_Value) * __Capacity / 2);
            temp[__Size] = __get_data(arg);
            delete[] __Data;
            __Data = temp;
        }
        __Size++;
    }
public:
    Vector() : __Data(nullptr), __Size(0), __Capacity(1) {}

    void push_back(const _Value& arg)
    { __push_back(arg); }

    void push_back(_Value && arg)
    { __push_back(std::forward<_Value&&>(arg); }

    inline void pop_back()
    { __Size--; }

    class iterator
    {
    private:
        __uint32 __Current;
        _Value *__Data;
    public:
        iterator(_Value *data = nullptr, __uint32 index = 0) : __Current(index), __Data(data) {}
        T& operator*() const
        { return __Data[__Current]; }
        iterator& operator++() {
            __Current++;
            return *this;
        }
        iterator operator++(int) {
            return iterator(__Data, __Current++);
        }
        T* operator->() const {
            return &(__Data[__Current]);
        }
        bool operator==(const iterator &arg) const {
            return arg.__Current == this->__Current;
        }

        bool operator!=(const iterator &arg) const {
            return arg.__Current != this->__Current;
        }
    };

    iterator begin() const
    {
        return iterator(__Data, 0);
    }
    iterator end() const
    {
        return iterator(nullptr);
    }
};

#endif // VECTOR_H
