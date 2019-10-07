#ifndef VECTOR_H
#define VECTOR_H

#include "Core/TLNode.h"
#include "Core/TypeTraits.h"
#include "Core/Version.h"
#include "Core/Exception.h"
#include "Core/Memory.h"
#include <initializer_list>

namespace rapid
{
template <typename T>
class Vector
{
public:
    class iterator;
    class reverse_iterator;
    class const_iterator;
    class const_reverse_iterator;

    using ValueType = T;
    using Pointer = ValueType*;
    using Reference = ValueType&;
    using ConstReference = const ValueType &;
    using RvalueReference = ValueType&&;
    using SizeType = size_type;
private:
    SizeType _M_size = 0;
    SizeType _M_capacity = 1;
    SizeType _M_growth = 0;
    NodeBase<ValueType> *_M_data = nullptr;

    void _F_initialize(SizeType s);
    void _F_copy_data(const Vector &arg);
    template<typename ... Args>
    void _F_insert(const iterator &it, const Args & ... arg);
    iterator _F_find(ConstReference arg) const;
    void _F_erase(const iterator &it)
    {
        if(it == end()) return;
        mem_forward(_M_data[it._M_current_index + 1].address(), (size() - it._M_current_index) * sizeof(ValueType), sizeof(ValueType));
        _F_add_size(-1);
    }
    void _F_growth()
    {
        if(_M_growth < 1)
        { _F_initialize(capacity() * 2); }
        else
        { _F_initialize(capacity() + _M_growth); }
    }
    void _F_add_size(SizeType s)
    { _M_size += s;}
public:
    class iterator
    {
    private:
        SizeType _M_current_index;
        SizeType _M_max_index;// not contained
        ValueType *_M_data;

        friend class Vector;

        inline void _F_init(const iterator &it)
        {
            _M_current_index = it._M_current_index;
            _M_max_index = it._M_max_index;
            _M_data = it._M_data;
        }
        void _F_next()
        {
            if(_M_current_index >= 0 && _M_current_index < _M_max_index)
            { _M_current_index++; }
            else
            { _M_current_index = _M_max_index; }
        }
        void _F_previous()
        {
            if(_M_current_index >= 0 && _M_current_index < _M_max_index)
            { _M_current_index--; }
            else
            { _M_current_index = _M_max_index; }
        }

        iterator(const SizeType n, const SizeType max, ValueType *start)
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
        SizeType operator-(const iterator &it) const
        { return _M_current_index - it._M_current_index; }
        iterator operator-(const SizeType s) const
        {
            iterator it = *this;
            it._M_current_index -= s;
            if(it._M_current_index < 0 || it._M_current_index > it._M_max_index)
            { it._M_current_index = it._M_max_index + 1; }
            return it;
        }
        iterator operator+(const SizeType s) const
        {
            iterator it = *this;
            it._M_current_index += s;
            if(it._M_current_index < 0 || it._M_current_index > it._M_max_index)
            { it._M_current_index = it._M_max_index + 1; }
            return it;
        }
        iterator operator-=(const SizeType s)
        {
            _M_current_index -= s;
            if(_M_current_index < 0 || _M_current_index > _M_max_index)
            { _M_current_index = _M_max_index + 1; }
            return *this;
        }
        iterator operator+=(const SizeType s)
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
        bool operator<(const iterator &it) const
        { return _M_current_index < it._M_current_index; }
        Reference operator*()
        { return _M_data[_M_current_index]; }

        Pointer operator->()
        { return _M_data[_M_current_index].address(); }

        bool operator==(const iterator& arg) const
        { return _M_current_index == arg._M_current_index; }
        bool operator!=(const iterator& arg) const
        { return _M_current_index != arg._M_current_index; }
    };

    class const_iterator
    {
    private:
        SizeType _M_current_index;
        SizeType _M_max_index;// not contained
        const ValueType *_M_data;

        friend class Vector;

        inline void _F_init(const const_iterator &it)
        {
            _M_current_index = it._M_current_index;
            _M_max_index = it._M_max_index;
            _M_data = it._M_data;
        }
        void _F_next()
        {
            if(_M_current_index >= 0 && _M_current_index < _M_max_index)
            { _M_current_index++; }
            else
            { _M_current_index = _M_max_index; }
        }
        void _F_previous()
        {
            if(_M_current_index >= 0 && _M_current_index < _M_max_index)
            { _M_current_index--; }
            else
            { _M_current_index = _M_max_index; }
        }
        iterator _F_const_cast()
        { return iterator(_M_current_index, _M_max_index, const_cast<ValueType *>(_M_data)); }

        const_iterator(const SizeType n, const SizeType max, const ValueType *start)
            : _M_current_index(n), _M_max_index(max), _M_data(start)
        { }
    public:
        const_iterator()
            : _M_current_index(SizeType(-1)), _M_max_index(SizeType(-1)), _M_data(nullptr)
        { }
        const_iterator(const const_iterator &it)
        { _F_init(it); }
        const_iterator(const_iterator && it)
        { _F_init(forward<const_iterator>(it)); }

        const_iterator operator=(const const_iterator &it)
        {
            _F_init(it);
            return *this;
        }
        SizeType operator-(const const_iterator &it) const
        { return _M_current_index - it._M_current_index; }
        const_iterator operator-(const SizeType s) const
        {
            const_iterator it = *this;
            it._M_current_index -= s;
            if(it._M_current_index < 0 || it._M_current_index > it._M_max_index)
            { it._M_current_index = it._M_max_index + 1; }
            return it;
        }
        const_iterator operator+(const SizeType s) const
        {
            const_iterator it = *this;
            it._M_current_index += s;
            if(it._M_current_index < 0 || it._M_current_index > it._M_max_index)
            { it._M_current_index = it._M_max_index + 1; }
            return it;
        }
        const_iterator operator-=(const SizeType s)
        {
            _M_current_index -= s;
            if(_M_current_index < 0 || _M_current_index > _M_max_index)
            { _M_current_index = _M_max_index + 1; }
            return *this;
        }
        const_iterator operator+=(const SizeType s)
        {
            _M_current_index += s;
            if(_M_current_index < 0 || _M_current_index > _M_max_index)
            { _M_current_index = _M_max_index + 1; }
            return *this;
        }
        const_iterator operator++()
        {
            _F_next();
            return *this;
        }
        const_iterator operator++(int)
        {
            const_iterator it = *this;
            _F_next();
            return it;
        }
        const_iterator operator--()
        {
            _F_previous();
            return *this;
        }
        const_iterator operator--(int)
        {
            const_iterator it = *this;
            _F_previous();
            return it;
        }
        bool operator<(const const_iterator &it) const
        { return _M_current_index < it._M_current_index; }
        Reference operator*() const
        { return _M_data[_M_current_index]; }

        Pointer operator->() const
        { return _M_data[_M_current_index].address(); }

        bool operator==(const const_iterator& arg) const
        { return _M_current_index == arg._M_current_index; }
        bool operator!=(const const_iterator& arg) const
        { return _M_current_index != arg._M_current_index; }
    };

    class reverse_iterator
    {
    private:
        SizeType _M_current_index;
        SizeType _M_max_index;
        ValueType *_M_data;

        friend class Vector;

        inline void _F_init(const iterator &it)
        {
            _M_current_index = it._M_current_index;
            _M_max_index = it._M_max_index;
            _M_data = it._M_data;
        }
        void _F_next()
        {
            if(_M_current_index >= 0 && _M_current_index < _M_max_index)
            { _M_current_index--; }
            else
            { _M_current_index = _M_max_index; }
        }
        void _F_previous()
        {
            if(_M_current_index >= 0 && _M_current_index < _M_max_index)
            { _M_current_index++; }
            else
            { _M_current_index = _M_max_index; }
        }

        reverse_iterator(const SizeType n, const SizeType max = 0, ValueType *start = nullptr)
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
        SizeType operator-(const reverse_iterator &it) const
        { return _M_current_index + it._M_current_index; }
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
        bool operator<(const reverse_iterator &it) const
        { return _M_current_index > it._M_current_index; }
        Reference operator*()
        { return _M_data[_M_current_index]; }

        Pointer operator->()
        { return &_M_data[_M_current_index]; }

        bool operator==(const iterator& arg) const
        { return _M_current_index == arg._M_current_index; }
        bool operator!=(const iterator& arg) const
        { return _M_current_index != arg._M_current_index; }
    };

    class const_reverse_iterator
    {
    private:
        SizeType _M_current_index;
        SizeType _M_max_index;
        const ValueType *_M_data;

        friend class Vector;

        inline void _F_init(const iterator &it)
        {
            _M_current_index = it._M_current_index;
            _M_max_index = it._M_max_index;
            _M_data = it._M_data;
        }
        void _F_next()
        {
            if(_M_current_index >= 0 && _M_current_index < _M_max_index)
            { _M_current_index--; }
            else
            { _M_current_index = _M_max_index; }
        }
        void _F_previous()
        {
            if(_M_current_index >= 0 && _M_current_index < _M_max_index)
            { _M_current_index++; }
            else
            { _M_current_index = _M_max_index; }
        }

        const_reverse_iterator(const SizeType n, const SizeType max = 0, const ValueType *start = nullptr)
            : _M_current_index(n), _M_max_index(max), _M_data(start) { }
    public:
        const_reverse_iterator() : _M_current_index(SizeType(-1)), _M_max_index(SizeType(-1)), _M_data(nullptr) { }
        const_reverse_iterator(const const_reverse_iterator &it) { _F_init(it); }
        const_reverse_iterator(const_reverse_iterator && it) { _F_init(forward<const_reverse_iterator>(it)); }

        const_reverse_iterator operator=(const const_reverse_iterator &it)
        {
            _F_init(it);
            return *this;
        }
        SizeType operator-(const const_reverse_iterator &it) const
        { return _M_current_index + it._M_current_index; }
        const_reverse_iterator operator++()
        {
            _F_next();
            return *this;
        }
        const_reverse_iterator operator++(int)
        {
            const_reverse_iterator it = *this;
            _F_next();
            return it;
        }
        const_reverse_iterator operator--()
        {
            _F_previous();
            return *this;
        }
        const_reverse_iterator operator--(int)
        {
            const_reverse_iterator it = *this;
            _F_previous();
            return it;
        }
        bool operator<(const const_reverse_iterator &it) const
        { return _M_current_index > it._M_current_index; }
        Reference operator*() const
        { return _M_data[_M_current_index]; }

        Pointer operator->() const
        { return &_M_data[_M_current_index]; }

        bool operator==(const iterator& arg) const
        { return _M_current_index == arg._M_current_index; }
        bool operator!=(const iterator& arg) const
        { return _M_current_index != arg._M_current_index; }
    };

    Vector(SizeType size = 1)
    { _F_initialize(size); }
    Vector(const Vector &v)
        : _M_size(v.size()), _M_capacity(v.capacity()), _M_growth(v._M_growth)
    { _F_copy_data(v); }
    Vector(std::initializer_list<ValueType> arg)
    {
        _F_initialize(arg.size());
        for(auto it = arg.begin(); it != arg.end(); ++it)
        { push_back(*it); }
    }
    ~Vector()
    { clear(); }

    void push_back(ConstReference arg)
    { _F_insert(end(), arg); }
    void push_back(RvalueReference arg)
    { _F_insert(end(), forward<ValueType>(arg)); }
    void push_front(ConstReference arg)
    { _F_insert(begin(), arg); }
    void push_front(RvalueReference arg)
    { _F_insert(begin(),  forward<ValueType>(arg)); }

    void pop_back()
    { _F_erase(end() - 1); }
    void pop_front()
    { _F_erase(begin()); }

    void clear()
    {
        if(_M_data != nullptr)
        { delete[] _M_data; }
        _M_data = nullptr;
        _M_size = 0;
    }

    Vector& operator=(const Vector &v)
    { _F_copy_data(v); }
    Reference operator[](const SizeType index)
    { return _M_data[index].ref_content(); }

    iterator begin()
    { return iterator(0, _M_size, _M_data[0].address()); }
    iterator end()
    { return iterator(_M_size, _M_size, _M_data[0].address()); }
    const_iterator begin() const
    { return const_iterator(0, _M_size, _M_data[0].address()); }
    const_iterator end() const
    { return const_iterator(_M_size, _M_size, _M_data[0].address()); }
    const_iterator cbegin() const
    { return const_iterator(0, _M_size, _M_data[0].address()); }
    const_iterator cend() const
    { return const_iterator(_M_size, _M_size, _M_data[0].address()); }

    reverse_iterator rbegin()
    { return reverse_iterator(_M_size - 1, _M_size, _M_data[0].address()); }
    reverse_iterator rend()
    { return reverse_iterator(-1, _M_size, _M_data[0].address()); }
    const_reverse_iterator rbegin() const
    { return const_reverse_iterator(_M_size - 1, _M_size, _M_data[0].address()); }
    const_reverse_iterator rend() const
    { return const_reverse_iterator(-1, _M_size, _M_data[0].address()); }
    const_reverse_iterator crbegin() const
    { return const_reverse_iterator(_M_size - 1, _M_size, _M_data[0].address()); }
    const_reverse_iterator crend() const
    { return const_reverse_iterator(-1, _M_size, _M_data[0].address()); }

    Reference back() const
    { return _M_data[size() - 1].ref_content(); }
    Reference front() const
    { return _M_data[0].ref_content(); }

    void insert(const iterator &it, ConstReference arg)
    { _F_insert(it, arg); }
    void insert(const iterator &it, RvalueReference arg)
    { _F_insert(it, forward<ValueType>(arg)); }
    void insert(iterator && it, ConstReference arg)
    { _F_insert(forward<iterator>(it), arg); }
    void insert(iterator && it, RvalueReference arg)
    { _F_insert(forward<iterator>(it), forward<ValueType>(arg)); }

    void erase(const iterator &it)
    { _F_erase(it); }
    void erase(iterator && it)
    { _F_erase(forward<iterator>(it)); }

    Reference at(const SizeType index)
    {
        if(index < 0 || index >= size())
        { throw IndexOutOfArrayException("exception: index out of array !"); }
        else
        { return (*this)[index]; }
    }

    void resize(SizeType s);

    SizeType size() const
    { return _M_size; }
    SizeType capacity() const
    { return _M_capacity; }
    bool empty() const
    { return size() == 0; }
    void set_growth(SizeType s)
    { _M_growth = s; }

    iterator find(Reference arg) const
    { return _F_find(arg); }
    iterator find(RvalueReference arg) const
    { return _F_find(forward<ValueType>(arg)); }

    template<typename ... Args>
    iterator emplace_front(const Args & ... args)
    { return _F_insert(begin(), args...); }
    template<typename ... Args>
    iterator emplace_front(Args && ... args)
    { return _F_insert(begin(), forward<ValueType>(args)...); }
    template<typename ... Args>
    iterator emplace_back(const Args & ... args)
    { return _F_insert(end(), args...); }
    template<typename ... Args>
    iterator emplace_back(Args && ... args)
    { return _F_insert(end(), forward<ValueType>(args)...); }

    template<typename ... Args>
    iterator emplace(const_iterator it, const Args & ... args)
    { return _F_insert(it._F_const_cast(), args...); }
    template<typename ... Args>
    iterator emplace(iterator it, const Args & ... args)
    { return _F_insert(it, args...); }
    template<typename ... Args>
    iterator emplace(const_iterator it, Args && ... args)
    { return _F_insert(it._F_const_cast(), forward<Args>(args)...); }
    template<typename ... Args>
    iterator emplace(iterator it, Args && ... args)
    { return _F_insert(it, forward<Args>(args)...); }
};

//-----------------------impl-----------------------//
//-----------------------impl-----------------------//
//-----------------------impl-----------------------//
//-----------------------impl-----------------------//
//-----------------------impl-----------------------//
template<typename T>
void Vector<T>::_F_initialize(SizeType s)
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
void Vector<T>::_F_copy_data(const Vector &v)
{
    clear();
    if(capacity() > 0)
    {
        _M_data = new NodeBase<ValueType>[capacity()];
        mem_copy(_M_data[0].address(), v._M_data[0].address(), _M_size * sizeof(ValueType));
    }
}

template<typename T>
template<typename ... Args>
void Vector<T>::_F_insert(const iterator &it, const Args & ... args)
{
    if(size() >= capacity())
    { _F_growth(); }
    if(it == end())
    { _M_data[size()].construct(args...); }
    else
    {
        mem_backward(_M_data[it._M_current_index].address(), (size() - it._M_current_index) * sizeof(ValueType), sizeof(ValueType));
        _M_data[it._M_current_index].construct(args...);
    }
    _F_add_size(1);
}

template<typename T>
void Vector<T>::resize(SizeType s)
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
typename Vector<T>::iterator Vector<T>::_F_find(ConstReference arg) const
{
    for(SizeType i = 0; i < size(); i++)
    {
        if(_M_data[i].content() == arg)
            return iterator(i, size() - 1, _M_data[0].address());
    }
    return end()._F_const_cast();
}

};

#endif // VECTOR_H
