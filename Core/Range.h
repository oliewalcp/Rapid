#ifndef RANGE_H
#define RANGE_H

#include "Version.h"
#include "TypeTraits.h"
#include "Memory.h"

namespace rapid
{

template<typename T = int>
class Range
{
public:
    class iterator;
private:
    using ValueType = T;
    using ConstReference = const ValueType &;

    using const_iterator = const iterator;

    ValueType _M_start, _M_end;// not contain [__End]

public:
    class iterator
    {
    private:
        ValueType __Current;
        iterator(ConstReference c)
        { __Current = c; }

        void __init(const iterator &it)
        { __Current = it.__Current; }

        friend class Range;
    public:
        iterator(const iterator &it) : __Current(*it) { }
        iterator(iterator && it)
        { __init(forward<iterator>(it)); }

        ValueType operator+(const ValueType arg)
        { return __Current += arg; }
        ValueType operator-(const ValueType arg)
        { return __Current -= arg; }

        ValueType operator++()
        { return ++__Current; }
        ValueType operator++(int)
        { return __Current++; }

        ValueType operator--()
        { return --__Current; }
        ValueType operator--(int)
        { return __Current--; }

        ValueType operator*() const
        { return __Current; }

        const ValueType* operator->() const
        { return &__Current; }

        bool operator==(const iterator& arg) const
        { return __Current == arg.__Current; }
        bool operator!=(const iterator& arg) const
        { return __Current != arg.__Current; }
    };

    Range(ValueType s, ValueType e) : _M_start(s), _M_end(e) { }

    iterator begin() { return iterator(_M_start); }
    iterator end() { return iterator(_M_end); }
};

};

#endif // RANGE_H
