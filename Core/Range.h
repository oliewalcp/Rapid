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
    class IteratorImpl;
    class ConstIteratorImpl;
    class ReverseIteratorImpl;
    class ConstReverseIteratorImpl;
private:
    using ValueType = T;
    using ConstReference = const ValueType &;

    using iterator = IteratorImpl;
    using const_iterator = ConstIteratorImpl;
    using reverse_iterator = ReverseIteratorImpl;
    using const_reverse_iterator = ConstReverseIteratorImpl;

    ValueType _M_start, _M_end;// contain [_M_end] and [_M_start]

public:
    class RangeIteratorBase
    {
    protected:
        ValueType _M_current;
        RangeIteratorBase(ConstReference c)
        { _M_current = c; }

    public:
        RangeIteratorBase(const RangeIteratorBase &it) : _M_current(*it) { }

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

        ValueType operator*() const
        { return _M_current; }

        ValueType* operator->() const
        { return &_M_current; }

        bool operator==(const RangeIteratorBase& arg) const
        { return _M_current > arg._M_current; }
        bool operator!=(const RangeIteratorBase& arg) const
        { return _M_current != arg._M_current; }
    };
    class RangeReverseIteratorBase
    {
    protected:
        ValueType _M_current;
        RangeReverseIteratorBase(ConstReference c)
        { _M_current = c; }

    public:
        RangeReverseIteratorBase(const RangeReverseIteratorBase &it)
            : _M_current(*it) { }

        ValueType operator+(const ValueType arg)
        { return _M_current += arg; }
        ValueType operator-(const ValueType arg)
        { return _M_current -= arg; }

        ValueType operator++()
        { return --_M_current; }
        ValueType operator++(int)
        { return _M_current--; }

        ValueType operator--()
        { return ++_M_current; }
        ValueType operator--(int)
        { return _M_current++; }

        ValueType operator*() const
        { return _M_current; }

        ValueType* operator->() const
        { return &_M_current; }

        bool operator==(const RangeReverseIteratorBase& arg) const
        { return _M_current < arg._M_current; }
        bool operator!=(const RangeReverseIteratorBase& arg) const
        { return _M_current != arg._M_current; }
    };
    class IteratorImpl : public RangeIteratorBase
    {
    protected:
        IteratorImpl(ConstReference n) : RangeIteratorBase(n) { }
    public:
        IteratorImpl(const IteratorImpl &it) : RangeIteratorBase(it) { }
        IteratorImpl(IteratorImpl && it)
            : RangeIteratorBase(forward<IteratorImpl>(it)) { }
    };
    class ConstIteratorImpl : public RangeIteratorBase
    {
    protected:
        ConstIteratorImpl(ConstReference n) : RangeIteratorBase(n) { }
    public:
        ConstIteratorImpl(const ConstIteratorImpl &it) : RangeIteratorBase(it) { }
        ConstIteratorImpl(ConstIteratorImpl && it)
            : RangeIteratorBase(forward<ConstIteratorImpl>(it)) { }
    };
    class ReverseIteratorImpl : public RangeReverseIteratorBase
    {
    protected:
        ReverseIteratorImpl(ConstReference n) : RangeReverseIteratorBase(n) { }
    public:
        ReverseIteratorImpl(const ReverseIteratorImpl &it) : RangeReverseIteratorBase(it) { }
        ReverseIteratorImpl(ReverseIteratorImpl && it)
            : RangeReverseIteratorBase(forward<ReverseIteratorImpl>(it)) { }
    };
    class ConstReverseIteratorImpl : public RangeReverseIteratorBase
    {
    protected:
        ConstReverseIteratorImpl(ConstReference n) : RangeReverseIteratorBase(n) { }
    public:
        ConstReverseIteratorImpl(const ConstReverseIteratorImpl &it) : RangeReverseIteratorBase(it) { }
        ConstReverseIteratorImpl(ConstReverseIteratorImpl && it)
            : RangeReverseIteratorBase(forward<ConstReverseIteratorImpl>(it)) { }
    };

    Range(ValueType s, ValueType e) : _M_start(s), _M_end(e) { }

    iterator begin()
    { return iterator(_M_start); }
    iterator end()
    { return iterator(_M_end); }
    const_iterator begin() const
    { return const_iterator(_M_start); }
    const_iterator end() const
    { return const_iterator(_M_end); }
    const_iterator cbegin() const
    { return const_iterator(_M_start); }
    const_iterator cend() const
    { return const_iterator(_M_end); }

    reverse_iterator rbegin()
    { return reverse_iterator(_M_end); }
    reverse_iterator rend()
    { return reverse_iterator(_M_start); }
    const_reverse_iterator rbegin() const
    { return const_reverse_iterator(_M_end); }
    const_reverse_iterator rend() const
    { return const_reverse_iterator(_M_start); }
    const_reverse_iterator crbegin() const
    { return const_reverse_iterator(_M_end); }
    const_reverse_iterator crend() const
    { return const_reverse_iterator(_M_start); }
};

};

#endif // RANGE_H
