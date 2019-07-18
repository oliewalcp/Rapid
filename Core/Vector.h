#ifndef VECTOR_H
#define VECTOR_H

#include "Core/Version.h"
#include "Core/Memory.h"

namespace rapid
{
template <typename T>
class Vector
{
public:
    class iterator;
    class reverse_iterator;
protected:
    using Type = T;
    using Pointer = Type*;
    using Reference = Type&;
    using ConstReference = const Type &;
    using RvalueReference = Type&&;
    using SizeType = long;

    SizeType _Size;
    SizeType _Capacity;
    SizeType _Growth;
    NodeBase<Type> *_Data;

    void _initialize(SizeType s);
    void _copy_data(Vector<T> &arg);
    void _insert(const iterator &it, ConstReference arg);
    void _erase(const iterator &it);
    void _growth();
    void _add_size(SizeType s)
    { _Size += s;}
public:
    class iterator
    {
    private:
        SizeType __CurrentIndex;
        SizeType __MaxIndex;// contained
        Type *__Data;

        friend class Vector;

        inline void __init(const iterator &it)
        {
            __CurrentIndex = it.__CurrentIndex;
            __MaxIndex = it.__MaxIndex;
            __Data = it.__Data;
        }
        void __next()
        {
            if(__CurrentIndex >= __MaxIndex)
            { __CurrentIndex = __MaxIndex + 1; }
            else
            { __CurrentIndex++; }
        }
        void __previous()
        {
            if(__CurrentIndex <= 0)
            { __CurrentIndex = __MaxIndex + 1; }
            else
            { __CurrentIndex--; }
        }

        iterator(const SizeType n, const SizeType max, Type *start)
            : __CurrentIndex(n), __MaxIndex(max), __Data(start) { }
    public:
        iterator() : __CurrentIndex(-1), __MaxIndex(-1), __Data(nullptr) { }
        iterator(const iterator &it) { __init(it); }
        iterator(iterator && it) { __init(std::forward<iterator>(it)); }

        iterator operator=(const iterator &it)
        {
            __init(it);
            return *this;
        }
        iterator operator-(const SizeType s)
        {
            __CurrentIndex -= s;
            if(__CurrentIndex < 0 || __CurrentIndex > __MaxIndex)
            { __CurrentIndex = __MaxIndex + 1; }
            return *this;
        }
        iterator operator+(const SizeType s)
        {
            __CurrentIndex += s;
            if(__CurrentIndex < 0 || __CurrentIndex > __MaxIndex)
            { __CurrentIndex = __MaxIndex + 1; }
            return *this;
        }
        iterator operator++()
        {
            __next();
            return *this;
        }
        iterator operator++(int)
        {
            iterator it = *this;
            __next();
            return it;
        }
        iterator operator--()
        {
            __previous();
            return *this;
        }
        iterator operator--(int)
        {
            iterator it = *this;
            __previous();
            return it;
        }
        Type operator*() const
        { return __Data[__CurrentIndex]; }

        Type* operator->() const
        { return &__Data[__CurrentIndex]; }

        bool operator==(const iterator& arg) const
        { return __CurrentIndex == arg.__CurrentIndex; }
        bool operator!=(const iterator& arg) const
        { return __CurrentIndex != arg.__CurrentIndex; }
    };

    class reverse_iterator
    {
    private:
        SizeType __CurrentIndex;
        SizeType __MaxIndex;
        Type *__Data;

        friend class Vector;

        inline void __init(const iterator &it)
        {
            __CurrentIndex = it.__CurrentIndex;
            __MaxIndex = it.__MaxIndex;
            __Data = it.__Data;
        }
        void __next()
        {
            if(__CurrentIndex <= 0)
            { __CurrentIndex = __MaxIndex + 1; }
            else
            { __CurrentIndex--; }
        }
        void __previous()
        {
            if(__CurrentIndex >= __MaxIndex)
            { __CurrentIndex = __MaxIndex + 1; }
            else
            { __CurrentIndex++; }
        }

        reverse_iterator(const SizeType n, const SizeType max = 0, Type *start = nullptr)
            : __CurrentIndex(n), __MaxIndex(max), __Data(start) { }
    public:
        reverse_iterator() : __CurrentIndex(-1), __MaxIndex(-1), __Data(nullptr) { }
        reverse_iterator(const reverse_iterator &it) { __init(it); }
        reverse_iterator(reverse_iterator && it) { __init(std::forward<reverse_iterator>(it)); }

        reverse_iterator operator=(const reverse_iterator &it)
        {
            __init(it);
            return *this;
        }
        reverse_iterator operator++()
        {
            __next();
            return *this;
        }
        reverse_iterator operator++(int)
        {
            reverse_iterator it = *this;
            __next();
            return it;
        }
        reverse_iterator operator--()
        {
            __previous();
            return *this;
        }
        reverse_iterator operator--(int)
        {
            reverse_iterator it = *this;
            __previous();
            return it;
        }
        Type operator*() const
        { return __Data[__CurrentIndex]; }

        Type* operator->() const
        { return &__Data[__CurrentIndex]; }

        bool operator==(const iterator& arg) const
        { return __CurrentIndex == arg.__CurrentIndex; }
        bool operator!=(const iterator& arg) const
        { return __CurrentIndex != arg.__CurrentIndex; }
    };

    Vector(SizeType size = 1) : _Size(0), _Growth(-1), _Data(nullptr)
    { _initialize(size); }
    Vector(Vector<T> &v) : _Size(v.size()), _Capacity(v.capacity()), _Growth(v._Growth), _Data(nullptr)
    { _copy_data(v); }
    virtual ~Vector();

    void push_back(ConstReference arg)
    { _insert(end(), arg); }
    void push_back(RvalueReference arg)
    { _insert(end(), std::forward<Type>(arg)); }
    void push_front(ConstReference arg)
    { _insert(begin(), arg); }
    void push_front(RvalueReference arg)
    { _insert(begin(),  std::forward<Type>(arg)); }

    void pop_back()
    { _erase(end() - 1); }
    void pop_front()
    { _erase(begin()); }

    void clear()
    { delete[] _Data; }

    Vector<T>& operator=(const Vector<T> &v)
    { _copy_data(v); }
    ConstReference operator[](const SizeType index)
    { return _Data[index].const_ref_content(); }

    iterator begin()
    { return iterator(0, _Size - 1, _Data[0].address()); }
    iterator end()
    { return iterator(_Size, _Size - 1, _Data[0].address()); }

    reverse_iterator rbegin()
    { return reverse_iterator(_Size - 1, _Size - 1, _Data[0].address()); }
    reverse_iterator rend()
    { return reverse_iterator(-1, _Size - 1, _Data[0].address()); }

    Reference back()
    { return _Data[size() - 1].ref_content(); }
    Reference front()
    { return _Data[0].ref_content(); }

    void insert(const iterator &it, ConstReference arg)
    { _insert(it, arg); }
    void insert(const iterator &it, RvalueReference arg)
    { _insert(it, std::forward<Type>(arg)); }
    void insert(iterator && it, ConstReference arg)
    { _insert(std::forward<iterator>(it), arg); }
    void insert(iterator && it, RvalueReference arg)
    { _insert(std::forward<iterator>(it), std::forward<Type>(arg)); }

    void erase(const iterator &it)
    { _erase(it); }
    void erase(iterator && it)
    { _erase(std::forward<iterator>(it)); }

    ConstReference at(const SizeType index);

    void resize(SizeType s);

    inline SizeType size()
    { return _Size; }
    inline SizeType capacity()
    { return _Capacity; }
    inline bool empty()
    { return size() == 0; }
    inline void set_growth(SizeType s)
    { _Growth = s; }

    iterator find(Type arg);
};


#ifndef NDEBUG
void test_Vector_main();
#endif

};

#endif // VECTOR_H
