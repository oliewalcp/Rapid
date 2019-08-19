#ifndef VECTOR_H
#define VECTOR_H

#include "Version.h"
#include "Memory.h"
#include "TypeTraits.h"

namespace rapid
{
template <typename T>
class Vector
{
public:
    class iterator;
    class reverse_iterator;
    typedef const iterator const_iterator;
    typedef const reverse_iterator const_reverse_iterator;
protected:
    using Type = T;
    using Pointer = Type*;
    using Reference = Type&;
    using ConstReference = const Type &;
    using RvalueReference = Type&&;
    using SizeType = size_type;

    SizeType _M_size;
    SizeType _M_capacity;
    SizeType _M_growth;
    NodeBase<T> *_M_data;

    void _F_initialize(SizeType s);
    void _F_copy_data(Vector<T> &arg);
    void _F_insert(const iterator &it, ConstReference arg);
    void _F_erase(const iterator &it);
    void _F_growth();
    void _F_add_size(SizeType s)
    { _M_size += s;}
public:
    class iterator
    {
    private:
        SizeType _M_current_index;
        SizeType _M_max_index;// contained
        Type *_M_data;

        friend class Vector;

        inline void _F_init(const iterator &it)
        {
            _M_current_index = it._M_current_index;
            _M_max_index = it._M_max_index;
            _M_data = it._M_data;
        }
        void _F_next()
        {
            if(_M_current_index >= _M_max_index)
            { _M_current_index = _M_max_index + 1; }
            else
            { _M_current_index++; }
        }
        void _F_previous()
        {
            if(_M_current_index <= 0)
            { _M_current_index = _M_max_index + 1; }
            else
            { _M_current_index--; }
        }

        iterator(const SizeType n, const SizeType max, Type *start)
            : _M_current_index(n), _M_max_index(max), _M_data(start)
        { }
    public:
        iterator() : _M_current_index(SizeType(-1)), _M_max_index(SizeType(-1)), _M_data(nullptr) { }
        iterator(const iterator &it) { _F_init(it); }
        iterator(iterator && it) { _F_init(forward<iterator>(it)); }

        iterator operator=(const iterator &it)
        {
            _F_init(it);
            return *this;
        }
        iterator operator-(const SizeType s)
        {
            _M_current_index -= s;
            if(_M_current_index < 0 || _M_current_index > _M_max_index)
            { _M_current_index = _M_max_index + 1; }
            return *this;
        }
        iterator operator+(const SizeType s)
        {
            _M_current_index += s;
            if(_M_current_index < 0 || _M_current_index > _M_max_index)
            { _M_current_index = _M_max_index + 1; }
            return *this;
        }
        iterator operator++()
        {
            _F_next();
            return *this;
        }
        iterator operator++(int)
        {
            iterator it = *this;
            _F_next();
            return it;
        }
        iterator operator--()
        {
            _F_previous();
            return *this;
        }
        iterator operator--(int)
        {
            iterator it = *this;
            _F_previous();
            return it;
        }
        Type operator*() const
        { return _M_data[_M_current_index]; }

        Type* operator->() const
        { return &_M_data[_M_current_index]; }

        bool operator==(const iterator& arg) const
        { return _M_current_index == arg._M_current_index; }
        bool operator!=(const iterator& arg) const
        { return _M_current_index != arg._M_current_index; }
    };

    class reverse_iterator
    {
    private:
        SizeType _M_current_index;
        SizeType _M_max_index;
        Type *_M_data;

        friend class Vector;

        inline void _F_init(const iterator &it)
        {
            _M_current_index = it._M_current_index;
            _M_max_index = it._M_max_index;
            _M_data = it._M_data;
        }
        void _F_next()
        {
            if(_M_current_index <= 0)
            { _M_current_index = _M_max_index + 1; }
            else
            { _M_current_index--; }
        }
        void _F_previous()
        {
            if(_M_current_index >= _M_max_index)
            { _M_current_index = _M_max_index + 1; }
            else
            { _M_current_index++; }
        }

        reverse_iterator(const SizeType n, const SizeType max = 0, Type *start = nullptr)
            : _M_current_index(n), _M_max_index(max), _M_data(start) { }
    public:
        reverse_iterator() : _M_current_index(SizeType(-1)), _M_max_index(SizeType(-1)), _M_data(nullptr) { }
        reverse_iterator(const reverse_iterator &it) { _F_init(it); }
        reverse_iterator(reverse_iterator && it) { _F_init(forward<reverse_iterator>(it)); }

        reverse_iterator operator=(const reverse_iterator &it)
        {
            _F_init(it);
            return *this;
        }
        reverse_iterator operator++()
        {
            _F_next();
            return *this;
        }
        reverse_iterator operator++(int)
        {
            reverse_iterator it = *this;
            _F_next();
            return it;
        }
        reverse_iterator operator--()
        {
            _F_previous();
            return *this;
        }
        reverse_iterator operator--(int)
        {
            reverse_iterator it = *this;
            _F_previous();
            return it;
        }
        Type operator*() const
        { return _M_data[_M_current_index]; }

        Type* operator->() const
        { return &_M_data[_M_current_index]; }

        bool operator==(const iterator& arg) const
        { return _M_current_index == arg._M_current_index; }
        bool operator!=(const iterator& arg) const
        { return _M_current_index != arg._M_current_index; }
    };

    Vector(SizeType size = 1) : _M_size(0), _M_growth(-1), _M_data(nullptr)
    { _F_initialize(size); }
    Vector(Vector<T> &v) : _M_size(v.size()), _M_capacity(v.capacity()), _M_growth(v._M_growth), _M_data(nullptr)
    { _F_copy_data(v); }
    virtual ~Vector();

    void push_back(ConstReference arg)
    { _F_insert(end(), arg); }
    void push_back(RvalueReference arg)
    { _F_insert(end(), forward<Type>(arg)); }
    void push_front(ConstReference arg)
    { _F_insert(begin(), arg); }
    void push_front(RvalueReference arg)
    { _F_insert(begin(),  forward<Type>(arg)); }

    void pop_back()
    { _F_erase(end() - 1); }
    void pop_front()
    { _F_erase(begin()); }

    void clear();

    Vector<T>& operator=(const Vector<T> &v)
    { _F_copy_data(v); }
    ConstReference operator[](const SizeType index)
    { return _M_data[index].const_ref_content(); }

    iterator begin()
    { return iterator(0, _M_size - 1, _M_data[0].address()); }
    iterator end()
    { return iterator(_M_size, _M_size - 1, _M_data[0].address()); }
//    const iterator begin()
//    { return iterator(0, _Size - 1, _Data[0].address()); }

    reverse_iterator rbegin()
    { return reverse_iterator(_M_size - 1, _M_size - 1, _M_data[0].address()); }
    reverse_iterator rend()
    { return reverse_iterator(-1, _M_size - 1, _M_data[0].address()); }

    Reference back()
    { return _M_data[size() - 1].ref_content(); }
    Reference front()
    { return _M_data[0].ref_content(); }

    void insert(const iterator &it, ConstReference arg)
    { _F_insert(it, arg); }
    void insert(const iterator &it, RvalueReference arg)
    { _F_insert(it, forward<Type>(arg)); }
    void insert(iterator && it, ConstReference arg)
    { _F_insert(forward<iterator>(it), arg); }
    void insert(iterator && it, RvalueReference arg)
    { _F_insert(forward<iterator>(it), forward<Type>(arg)); }

    void erase(const iterator &it)
    { _F_erase(it); }
    void erase(iterator && it)
    { _F_erase(forward<iterator>(it)); }

    ConstReference at(const SizeType index);

    void resize(SizeType s);

    SizeType size()
    { return _M_size; }
    SizeType capacity()
    { return _M_capacity; }
    bool empty()
    { return size() == 0; }
    void set_growth(SizeType s)
    { _M_growth = s; }

    iterator find(Type arg);
};

};

#endif // VECTOR_H
