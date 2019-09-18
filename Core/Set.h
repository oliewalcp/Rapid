#ifndef SET_H
#define SET_H

#include "AVLTree.h"
#include "RedBlackTree.h"

namespace rapid
{

template<typename _Value,
         typename _TreeType>
class SetBase
{
public:
    using ValueType = _Value;
    using SizeType = size_type;

private:
    using TreeType = _TreeType;

    using FormerIteratorImpl = typename TreeType::fiterator;
    using ConstFormerIteratorImpl = typename TreeType::const_fiterator;
    using MiddleIteratorImpl = typename TreeType::miterator;
    using ConstMiddleIteratorImpl = typename TreeType::const_miterator;
    using AfterIteratorImpl = typename TreeType::aiterator;
    using ConstAfterIteratorImpl = typename TreeType::const_aiterator;

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

        friend class SetBase;

        iterator(IteratorImpl it) : _M_it(it) { }
    public:
        iterator() { }
        iterator(const iterator &it)
            : _M_it(it._M_it) { }
        iterator(iterator &&it)
            : _M_it(forward<iterator>(it)._M_it) { }

        iterator operator++()
        {
            iterator it = *this;
            ++_M_it;
            return it;
        }
        iterator operator++(int)
        {
            ++_M_it;
            return *this;
        }
        iterator operator--()
        {
            iterator it = *this;
            --_M_it;
            return it;
        }
        iterator operator--(int)
        {
            --_M_it;
            return *this;
        }
        iterator operator=(const iterator &it)
        { return _M_it = it._M_it; }
        ValueType operator*()
        { return *_M_it; }
        ValueType* operator->()
        { return &(*_M_it); }
        bool operator==(const iterator &it) const
        { return _M_it == it._M_it; }
        bool operator!=(const iterator &it) const
        { return _M_it != it._M_it; }
    };
    class const_iterator
    {
    private:
        ConstIteratorImpl _M_it;

        friend class SetBase;

        const_iterator(ConstIteratorImpl it) : _M_it(it) { }
    public:
        const_iterator() { }
        const_iterator(const const_iterator &it)
            : _M_it(it._M_it) { }
        const_iterator(const_iterator &&it)
            : _M_it(forward<const_iterator>(it)._M_it) { }

        const_iterator operator++()
        {
            const_iterator it = *this;
            ++_M_it;
            return it;
        }
        const_iterator operator++(int)
        {
            ++_M_it;
            return *this;
        }
        const_iterator operator--()
        {
            const_iterator it = *this;
            --_M_it;
            return it;
        }
        const_iterator operator--(int)
        {
            --_M_it;
            return *this;
        }
        const_iterator operator=(const const_iterator &it)
        { return _M_it = it._M_it; }
        ValueType operator*() const
        { return *_M_it; }
        ValueType* operator->() const
        { return &(*_M_it); }
        bool operator==(const const_iterator &it) const
        { return _M_it == it._M_it; }
        bool operator!=(const const_iterator &it) const
        { return _M_it != it._M_it; }
    };
    class reverse_iterator
    {
    private:
        ReverseIteratorImpl _M_it;

        friend class SetBase;

        reverse_iterator(ReverseIteratorImpl it) : _M_it(it) { }
    public:
        reverse_iterator() { }
        reverse_iterator(const reverse_iterator &it)
            : _M_it(it._M_it) { }
        reverse_iterator(reverse_iterator &&it)
            : _M_it(forward<reverse_iterator>(it)._M_it) { }

        reverse_iterator operator++()
        {
            reverse_iterator it = *this;
            ++_M_it;
            return it;
        }
        reverse_iterator operator++(int)
        {
            ++_M_it;
            return *this;
        }
        reverse_iterator operator--()
        {
            reverse_iterator it = *this;
            --_M_it;
            return it;
        }
        reverse_iterator operator--(int)
        {
            --_M_it;
            return *this;
        }
        reverse_iterator operator=(const reverse_iterator &it)
        { return _M_it = it._M_it; }
        ValueType operator*()
        { return *_M_it; }
        ValueType* operator->()
        { return &(*_M_it); }
        bool operator==(const reverse_iterator &it) const
        { return _M_it == it._M_it; }
        bool operator!=(const reverse_iterator &it) const
        { return _M_it != it._M_it; }
    };
    class const_reverse_iterator
    {
    private:
        ConstReverseIteratorImpl _M_it;

        friend class SetBase;

        const_reverse_iterator(ConstReverseIteratorImpl it) : _M_it(it) { }
    public:
        const_reverse_iterator() { }
        const_reverse_iterator(const const_reverse_iterator &it)
            : _M_it(it._M_it) { }
        const_reverse_iterator(const_reverse_iterator &&it)
            : _M_it(forward<const_reverse_iterator>(it)._M_it) { }

        const_reverse_iterator operator++()
        {
            const_reverse_iterator it = *this;
            ++_M_it;
            return it;
        }
        const_reverse_iterator operator++(int)
        {
            ++_M_it;
            return *this;
        }
        const_reverse_iterator operator--()
        {
            const_reverse_iterator it = *this;
            --_M_it;
            return it;
        }
        const_reverse_iterator operator--(int)
        {
            --_M_it;
            return *this;
        }
        const_reverse_iterator operator=(const const_reverse_iterator &it)
        { return _M_it = it._M_it; }
        ValueType operator*() const
        { return *_M_it; }
        ValueType* operator->() const
        { return &(*_M_it); }
        bool operator==(const const_reverse_iterator &it) const
        { return _M_it == it._M_it; }
        bool operator!=(const const_reverse_iterator &it) const
        { return _M_it != it._M_it; }
    };

    class fiterator
    {
    private:
        FormerIteratorImpl _M_it;

        friend class SetBase;

        fiterator(FormerIteratorImpl it) : _M_it(it) { }
    public:
        fiterator() { }
        fiterator(const fiterator &it)
            : _M_it(it._M_it) { }
        fiterator(fiterator &&it)
            : _M_it(forward<fiterator>(it)._M_it) { }

        fiterator operator++()
        {
            fiterator it = *this;
            ++_M_it;
            return it;
        }
        fiterator operator++(int)
        {
            ++_M_it;
            return *this;
        }
        fiterator operator--()
        {
            fiterator it = *this;
            --_M_it;
            return it;
        }
        fiterator operator--(int)
        {
            --_M_it;
            return *this;
        }
        fiterator operator=(const fiterator &it)
        { return _M_it = it._M_it; }
        ValueType operator*()
        { return *_M_it; }
        ValueType* operator->()
        { return &(*_M_it); }
        bool operator==(const fiterator &it) const
        { return _M_it == it._M_it; }
        bool operator!=(const fiterator &it) const
        { return _M_it != it._M_it; }
    };
    class aiterator
    {
    private:
        AfterIteratorImpl _M_it;

        friend class SetBase;

        aiterator(AfterIteratorImpl it) : _M_it(it) { }
    public:
        aiterator() { }
        aiterator(const aiterator &it)
            : _M_it(it._M_it) { }
        aiterator(aiterator &&it)
            : _M_it(forward<aiterator>(it)._M_it) { }

        aiterator operator++()
        {
            aiterator it = *this;
            ++_M_it;
            return it;
        }
        aiterator operator++(int)
        {
            ++_M_it;
            return *this;
        }
        aiterator operator--()
        {
            aiterator it = *this;
            --_M_it;
            return it;
        }
        aiterator operator--(int)
        {
            --_M_it;
            return *this;
        }
        aiterator operator=(const aiterator &it)
        { return _M_it = it._M_it; }
        ValueType operator*()
        { return *_M_it; }
        ValueType* operator->()
        { return &(*_M_it); }
        bool operator==(const aiterator &it) const
        { return _M_it == it._M_it; }
        bool operator!=(const aiterator &it) const
        { return _M_it != it._M_it; }
    };
    class const_aiterator
    {
    private:
        ConstAfterIteratorImpl _M_it;

        friend class SetBase;

        const_aiterator(ConstAfterIteratorImpl it) : _M_it(it) { }
    public:
        const_aiterator() { }
        const_aiterator(const const_aiterator &it)
            : _M_it(it._M_it) { }
        const_aiterator(const_aiterator &&it)
            : _M_it(forward<const_aiterator>(it)._M_it) { }

        const_aiterator operator++()
        {
            const_aiterator it = *this;
            ++_M_it;
            return it;
        }
        const_aiterator operator++(int)
        {
            ++_M_it;
            return *this;
        }
        const_aiterator operator--()
        {
            const_aiterator it = *this;
            --_M_it;
            return it;
        }
        const_aiterator operator--(int)
        {
            --_M_it;
            return *this;
        }
        const_aiterator operator=(const const_aiterator &it)
        { return _M_it = it._M_it; }
        ValueType operator*() const
        { return *_M_it; }
        ValueType* operator->() const
        { return &(*_M_it); }
        bool operator==(const const_aiterator &it) const
        { return _M_it == it._M_it; }
        bool operator!=(const const_aiterator &it) const
        { return _M_it != it._M_it; }
    };
    class const_fiterator
    {
    private:
        ConstFormerIteratorImpl _M_it;

        friend class SetBase;

        const_fiterator(ConstFormerIteratorImpl it) : _M_it(it) { }
    public:
        const_fiterator() { }
        const_fiterator(const const_fiterator &it)
            : _M_it(it._M_it) { }
        const_fiterator(const_fiterator &&it)
            : _M_it(forward<const_fiterator>(it)._M_it) { }

        const_fiterator operator++()
        {
            const_fiterator it = *this;
            ++_M_it;
            return it;
        }
        const_fiterator operator++(int)
        {
            ++_M_it;
            return *this;
        }
        const_fiterator operator--()
        {
            const_fiterator it = *this;
            --_M_it;
            return it;
        }
        const_fiterator operator--(int)
        {
            --_M_it;
            return *this;
        }
        const_fiterator operator=(const const_fiterator &it)
        { return _M_it = it._M_it; }
        ValueType operator*() const
        { return *_M_it; }
        ValueType* operator->() const
        { return &(*_M_it); }
        bool operator==(const const_fiterator &it) const
        { return _M_it == it._M_it; }
        bool operator!=(const const_fiterator &it) const
        { return _M_it != it._M_it; }
    };

private:
    TreeType _M_tree;

public:
    SetBase() { }
    SetBase(const SetBase &m) : _M_tree(m._M_tree) { }
    SetBase(SetBase &&m) : _M_tree(forward<SetBase>(m)._M_tree) { }

    bool empty() const
    { return _M_tree.empty(); }
    SizeType size() const
    { return _M_tree.size(); }

    iterator insert(const ValueType &data)
    { return _M_tree.insert(data); }
    iterator insert(ValueType &&data)
    { return _M_tree.insert(forward<ValueType>(data)); }
    iterator insert(std::initializer_list<ValueType> arg)
    { return _M_tree.insert(arg); }

    void erase(iterator it)
    { _M_tree.erase(*it); }

    iterator find(const ValueType &key) const
    { return iterator(_M_tree.find(key)); }
    iterator find(ValueType &&key) const
    { return iterator(_M_tree.find(forward<ValueType>(key))); }

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

    aiterator abegin()
    { return aiterator(_M_tree.abegin()); }
    aiterator aend()
    { return aiterator(_M_tree.aend()); }
    const_aiterator abegin() const
    { return const_aiterator(_M_tree.abegin()); }
    const_aiterator aend() const
    { return const_aiterator(_M_tree.aend()); }
    const_aiterator cabegin() const
    { return const_aiterator(_M_tree.cabegin()); }
    const_aiterator caend() const
    { return const_aiterator(_M_tree.caend()); }

    fiterator fbegin()
    { return fiterator(_M_tree.fbegin()); }
    fiterator fend()
    { return fiterator(_M_tree.fend()); }
    const_fiterator fbegin() const
    { return const_fiterator(_M_tree.fbegin()); }
    const_fiterator fend() const
    { return const_fiterator(_M_tree.fend()); }
    const_fiterator cfbegin() const
    { return const_fiterator(_M_tree.cfbegin()); }
    const_fiterator cfend() const
    { return const_fiterator(_M_tree.cfend()); }

    typename TreeType::TreeType to_ordinary_tree()
    { return _M_tree.to_ordinary_tree(); }
};


template<typename _Value,
         typename _Compare = Compare<_Value>>
using Set = SetBase<_Value, RedBlackTree<_Value, _Compare>>;

template<typename _Value,
         typename _Compare = Compare<_Value>>
using AVLSet = SetBase<_Value, AVLTree<_Value, _Compare>>;

}

#endif // SET_H
