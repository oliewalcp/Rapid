#ifndef RANGE_H
#define RANGE_H

#include "Core/Version.h"
#include "Core/TypeTraits.h"

namespace rapid
{

template<typename T = int>
class Range
{
public:
    using ValueType = T;
    using Reference = ValueType &;
    using ConstReference = const ValueType &;
    using Pointer = ValueType *;
    using RvalueReference = ValueType &&;

private:
    ValueType _M_start, _M_end;// contain [_M_end] and [_M_start]

public:
    class iterator
    {
    protected:
        friend class Range;

        ValueType _M_current;

        iterator(ConstReference c)
        { _M_current = c; }
    public:
        iterator(const iterator &it) : _M_current(*it) { }

        ValueType operator+(const ValueType arg)
        { return _M_current += arg; }
        ValueType operator-(const ValueType arg)
        { return _M_current -= arg; }

        ValueType operator++()
        { return ++_M_current; }
        ValueType operator++(int)
        { return _M_current++; }

        ValueType operator--()
        { return --_M_current; }
        ValueType operator--(int)
        { return _M_current--; }

        Reference operator*()
        { return _M_current; }

        Pointer operator->()
        { return &_M_current; }

        Reference operator*() const
        { return _M_current; }

        Pointer operator->() const
        { return &_M_current; }

        bool operator==(const iterator& arg) const
        { return _M_current > arg._M_current; }
        bool operator!=(const iterator& arg) const
        { return _M_current != arg._M_current; }
    };
    class reverse_iterator
    {
    protected:
        friend class Range;

        ValueType _M_current;

        reverse_iterator(ConstReference c)
        { _M_current = c; }
    public:
        reverse_iterator(const reverse_iterator &it) : _M_current(*it) { }

        ValueType operator+(const ValueType arg)
        { return _M_current += arg; }
        ValueType operator-(const ValueType arg)
        { return _M_current -= arg; }

        ValueType operator++()
        { return ++_M_current; }
        ValueType operator++(int)
        { return _M_current++; }

        ValueType operator--()
        { return --_M_current; }
        ValueType operator--(int)
        { return _M_current--; }

        Reference operator*()
        { return _M_current; }

        Pointer operator->()
        { return &_M_current; }

        Reference operator*() const
        { return _M_current; }

        Pointer operator->() const
        { return &_M_current; }

        bool operator==(const reverse_iterator& arg) const
        { return _M_current > arg._M_current; }
        bool operator!=(const reverse_iterator& arg) const
        { return _M_current != arg._M_current; }
    };

    Range(RvalueReference s, RvalueReference e)
        : _M_start(forward<ValueType>(s)), _M_end(forward<ValueType>(e)) { }
    Range(ConstReference s, ConstReference e) : _M_start(s), _M_end(e) { }

    iterator begin()
    { return iterator(_M_start); }
    iterator end()
    { return iterator(_M_end); }
    iterator begin() const
    { return iterator(_M_start); }
    iterator end() const
    { return iterator(_M_end); }

    reverse_iterator rbegin()
    { return reverse_iterator(_M_end); }
    reverse_iterator rend()
    { return reverse_iterator(_M_start); }
    reverse_iterator rbegin() const
    { return reverse_iterator(_M_end); }
    reverse_iterator rend() const
    { return reverse_iterator(_M_start); }

};

};

#endif // RANGE_H
