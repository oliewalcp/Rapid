#ifndef RANGE_H
#define RANGE_H

#include "Core/Version.h"
#include "Core/TypeTraits.h"
#include "Core/Memory.h"

namespace rapid
{

template<typename T = int>
class Range
{
private:
    using ValueType = T;
    using Type = typename RemoveReference<T>::type;
    using ConstReference = const ValueType &;

    ValueType __Start, __End;// not contain [__End]

public:
    class iterator
    {
    private:
        ValueType __Current;
        iterator(ConstReference c)
        { __Current = c; }

        friend class Range;
    public:
        iterator(const iterator &it) : __Current(*it) { }
        iterator(iterator && it) : iterator(std::forward<iterator&&>(it)) { }

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

    Range(ValueType s, ValueType e) : __Start(s), __End(e) { }

    iterator begin() { return iterator(__Start); }
    iterator end() { return iterator(__End); }
};

#ifndef NDEBUG
void test_Range_main()
{
    for(int i : Range(1, 10))
    {
        std::cout << i << std::endl;
    }
}
#endif

};

#endif // RANGE_H
