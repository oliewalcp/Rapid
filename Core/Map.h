#ifndef MAP_H
#define MAP_H

#include "AVLTree.h"
#include "RedBlackTree.h"
#include <iostream>

namespace rapid
{

template<typename _First, typename _Second>
struct Pair
{
    _First First;
    _Second Second;
    Pair() { }
    Pair(const _First &f) : First(f) { }
    Pair(_First &&f) : First(forward<_First>(f)) { }
    Pair(const Pair &p)
        : First(p.First), Second(p.Second) { }
    Pair(Pair &&p)
        : First(forward<Pair>(p).First), Second(forward<Pair>(p).Second) { }

    template<typename ... Args>
    Pair(const _First &f, const Args & ... args)
        : First(f), Second(args...) { }
    template<typename ... Args>
    Pair(const _First &f, Args && ... args)
        : First(f), Second(forward<Args>(args)...) { }
    template<typename ... Args>
    Pair(_First &&f, const Args & ... args)
        : First(forward<_First>(f)), Second(args...) { }
    template<typename ... Args>
    Pair(_First &&f, Args && ... args)
        : First(forward<_First>(f)), Second(forward<Args>(args)...) { }

    bool operator<(const Pair &f) const
    { return First < f.First; }
    Pair& operator=(const Pair &p)
    {
        First = p.First;
        Second = p.Second;
        return *this;
    }
};

template<typename _First, typename _Second>
struct NodeCompare
{
    int operator()(const Pair<_First, _Second> &p, const _First &f)
    {
        if(p.First < f) return 1;
        if(f < p.First) return -1;
        return 0;
    }
    int operator()(const _First &f, const Pair<_First, _Second> &p)
    {
        if(f < p.First) return 1;
        if(p.First < f) return -1;
        return 0;
    }
};

template<typename _Key, typename _Value,
         typename _TreeType>
class MapBase
{
public:
    using KeyType = _Key;
    using ValueType = _Value;
    using DataType = Pair<KeyType, ValueType>;
    using SizeType = size_type;
    using CompareType = NodeCompare<KeyType, ValueType>;

private:
    using TreeType = _TreeType;

    using IteratorImpl = typename TreeType::iterator;
    using ConstIteratorImpl = typename TreeType::const_iterator;
    using ReverseIteratorImpl = typename TreeType::reverse_iterator;
    using ConstReverseIteratorImpl = typename TreeType::const_reverse_iterator;

public:
    class iterator;
    class const_iterator;
    class reverse_iterator;
    class const_reverse_iterator;

    class iterator
    {
    private:
        IteratorImpl _M_it;

        friend class MapBase;

        iterator(IteratorImpl it) : _M_it(it) { }
    public:
        iterator() { }
        iterator(const iterator &it)
            : _M_it(it._M_it) { }
        iterator(iterator &&it)
            : _M_it(forward<iterator>(it)._M_it) { }

        iterator operator++()
        {
            ++_M_it;
            return *this;
        }
        iterator operator++(int)
        {
            iterator it = *this;
            ++_M_it;
            return it;
        }
        iterator operator--()
        {
            --_M_it;
            return *this;
        }
        iterator operator--(int)
        {
            iterator it = *this;
            --_M_it;
            return it;
        }
        iterator operator=(const iterator &it)
        { return _M_it = it._M_it; }
        DataType& operator*()
        { return *_M_it; }
        DataType* operator->()
        { return _M_it.operator->(); }
        bool operator==(const iterator &it) const
        { return _M_it == it._M_it; }
        bool operator!=(const iterator &it) const
        { return _M_it != it._M_it; }
    };
    class const_iterator
    {
    private:
        ConstIteratorImpl _M_it;

        friend class MapBase;

        const_iterator(ConstIteratorImpl it) : _M_it(it) { }
    public:
        const_iterator() { }
        const_iterator(const const_iterator &it)
            : _M_it(it._M_it) { }
        const_iterator(const_iterator &&it)
            : _M_it(forward<const_iterator>(it)._M_it) { }

        const_iterator operator++()
        {
            ++_M_it;
            return *this;
        }
        const_iterator operator++(int)
        {
            const_iterator it = *this;
            ++_M_it;
            return it;
        }
        const_iterator operator--()
        {
            --_M_it;
            return *this;
        }
        const_iterator operator--(int)
        {
            const_iterator it = *this;
            --_M_it;
            return it;
        }
        const_iterator operator=(const const_iterator &it)
        { return _M_it = it._M_it; }
        DataType& operator*() const
        { return *_M_it; }
        DataType* operator->() const
        { return _M_it.operator->(); }
        bool operator==(const const_iterator &it) const
        { return _M_it == it._M_it; }
        bool operator!=(const const_iterator &it) const
        { return _M_it != it._M_it; }
    };
    class reverse_iterator
    {
    private:
        ReverseIteratorImpl _M_it;

        friend class MapBase;

        reverse_iterator(ReverseIteratorImpl it) : _M_it(it) { }
    public:
        reverse_iterator() { }
        reverse_iterator(const reverse_iterator &it)
            : _M_it(it._M_it) { }
        reverse_iterator(reverse_iterator &&it)
            : _M_it(forward<reverse_iterator>(it)._M_it) { }

        reverse_iterator operator++()
        {
            ++_M_it;
            return *this;
        }
        reverse_iterator operator++(int)
        {
            reverse_iterator it = *this;
            ++_M_it;
            return it;
        }
        reverse_iterator operator--()
        {
            --_M_it;
            return *this;
        }
        reverse_iterator operator--(int)
        {
            reverse_iterator it = *this;
            --_M_it;
            return it;
        }
        reverse_iterator operator=(const reverse_iterator &it)
        { return _M_it = it._M_it; }
        DataType& operator*()
        { return *_M_it; }
        DataType* operator->()
        { return _M_it.operator->(); }
        bool operator==(const reverse_iterator &it) const
        { return _M_it == it._M_it; }
        bool operator!=(const reverse_iterator &it) const
        { return _M_it != it._M_it; }
    };
    class const_reverse_iterator
    {
    private:
        ConstReverseIteratorImpl _M_it;

        friend class MapBase;

        const_reverse_iterator(ConstReverseIteratorImpl it) : _M_it(it) { }
    public:
        const_reverse_iterator() { }
        const_reverse_iterator(const const_reverse_iterator &it)
            : _M_it(it._M_it) { }
        const_reverse_iterator(const_reverse_iterator &&it)
            : _M_it(forward<const_reverse_iterator>(it)._M_it) { }

        const_reverse_iterator operator++()
        {
            ++_M_it;
            return *this;
        }
        const_reverse_iterator operator++(int)
        {
            const_reverse_iterator it = *this;
            ++_M_it;
            return it;
        }
        const_reverse_iterator operator--()
        {
            --_M_it;
            return *this;
        }
        const_reverse_iterator operator--(int)
        {
            const_reverse_iterator it = *this;
            --_M_it;
            return it;
        }
        const_reverse_iterator operator=(const const_reverse_iterator &it)
        { return _M_it = it._M_it; }
        DataType& operator*() const
        { return *_M_it; }
        DataType* operator->() const
        { return _M_it.operator->(); }
        bool operator==(const const_reverse_iterator &it) const
        { return _M_it == it._M_it; }
        bool operator!=(const const_reverse_iterator &it) const
        { return _M_it != it._M_it; }
    };

private:
    TreeType _M_tree;

public:
    MapBase() { }
    MapBase(const MapBase &m) : _M_tree(m._M_tree) { }
    MapBase(MapBase &&m) : _M_tree(forward<MapBase>(m)._M_tree) { }

    bool empty() const
    { return _M_tree.empty(); }
    SizeType size() const
    { return _M_tree.size(); }

    iterator insert(const DataType &data)
    { return _M_tree.insert(data); }
    iterator insert(DataType &&data)
    { return _M_tree.insert(data); }

//    iterator insert(const KeyType &key, const ValueType &value)
//    { return insert(DataType(key, value)); }
//    iterator insert(KeyType &&key, const ValueType &value)
//    { return insert(DataType(forward<KeyType>(key), value)); }
//    iterator insert(const KeyType &key, ValueType &&value)
//    { return insert(DataType(key, forward<ValueType>(value))); }
//    iterator insert(KeyType &&key, ValueType &&value)
//    { return insert(DataType(forward<KeyType>(key), forward<ValueType>(value))); }

    template<typename ... Args>
    iterator insert(KeyType &&key, Args && ... value)
    { return insert(DataType(forward<KeyType>(key), forward<Args>(value)...)); }
    template<typename ... Args>
    iterator insert(KeyType &&key, const Args & ... value)
    { return insert(DataType(forward<KeyType>(key), value...)); }
    template<typename ... Args>
    iterator insert(const KeyType &key, Args && ... value)
    { return insert(DataType(key, forward<Args>(value)...)); }
    template<typename ... Args>
    iterator insert(const KeyType &key, const Args & ... value)
    { return insert(DataType(key, value...)); }

    template<typename ... Args>
    iterator emplace(const KeyType &key, const Args & ... args)
    { return insert(DataType(key, args...)); }
    template<typename ... Args>
    iterator emplace(const KeyType &key, Args && ... args)
    { return insert(DataType(key, forward<Args>(args)...)); }
    template<typename ... Args>
    iterator emplace(KeyType &&key, const Args & ... args)
    { return insert(DataType(forward<KeyType>(key), args...)); }
    template<typename ... Args>
    iterator emplace(KeyType &&key, Args && ... args)
    { return insert(DataType(forward<KeyType>(key), forward<Args>(args)...)); }

    void erase(iterator it)
    { _M_tree.erase(*it); }

    iterator find(const KeyType &key) const
    { return iterator(_M_tree.template find<KeyType, CompareType>(key)); }
    iterator find(KeyType &&key) const
    { return iterator(_M_tree.template find<KeyType, CompareType>(forward<KeyType>(key))); }

    ValueType& operator[](const KeyType &key)
    {
        IteratorImpl it = _M_tree.template find_and_insert<KeyType, CompareType>(key);
        return it->Second;
    }
    ValueType& operator[](KeyType &&key)
    {
        IteratorImpl it = _M_tree.template find_and_insert(forward<KeyType>(key));
        return it->Second;
    }

    iterator begin()
    { return iterator(_M_tree.begin()); }
    iterator end()
    { return iterator(_M_tree.end()); }
    const_iterator begin() const
    { return const_iterator(_M_tree.begin()); }
    const_iterator end() const
    { return const_iterator(_M_tree.end()); }
    const_iterator cbegin() const
    { return const_iterator(_M_tree.cbegin()); }
    const_iterator cend() const
    { return const_iterator(_M_tree.cend()); }

    reverse_iterator rbegin()
    { return reverse_iterator(_M_tree.rbegin()); }
    reverse_iterator rend()
    { return reverse_iterator(_M_tree.rend()); }
    const_reverse_iterator rbegin() const
    { return const_reverse_iterator(_M_tree.rbegin()); }
    const_reverse_iterator rend() const
    { return const_reverse_iterator(_M_tree.rend()); }
    const_reverse_iterator crbegin() const
    { return const_reverse_iterator(_M_tree.crbegin()); }
    const_reverse_iterator crend() const
    { return const_reverse_iterator(_M_tree.crend()); }

};

template<typename _Key,
         typename _Value,
         typename _Compare = Compare<Pair<_Key, _Value>>>
using Map = MapBase<_Key, _Value, RedBlackTree<Pair<_Key, _Value>, _Compare>>;

template<typename _Key,
         typename _Value,
         typename _Compare = Compare<Pair<_Key, _Value>>>
using AVLMap = MapBase<_Key, _Value, AVLTree<Pair<_Key, _Value>, _Compare>>;

};

#endif // MAP_H
