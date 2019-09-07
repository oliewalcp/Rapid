#ifndef DOUBLELINKEDLIST_H
#define DOUBLELINKEDLIST_H

#include "Memory.h"
#include "TypeTraits.h"
#include "IteratorBase.h"

namespace rapid
{

template<typename T>
class DoubleLinkedList
{
public:
    using ValueType = T;
    using Pointer = ValueType*;
    using Reference = ValueType&;
    using ConstReference = const ValueType &;
    using RvalueReference = ValueType&&;
    using SizeType = size_type;

    class iterator;
    class const_iterator;
    class reverse_iterator;
    class const_reverse_iterator;
private:

    struct Node
    {
        NodeBase<ValueType> *Data;
        Node *Next;
        Node *Previous;

        template<typename ... Args>
        Node(Node *p, Node *n, const Args & ... args)
            : Data(new NodeBase<ValueType>(args...)), Next(n), Previous(p)
        {
            if(p != nullptr)
            { p->Next = this; }
            if(n != nullptr)
            { n->Previous = this; }
        }

        ~Node()
        { delete Data; }
        void dealloc()
        {
            if(Next != nullptr)
            { Next->Previous = Previous; }
            if(Previous != nullptr)
            { Previous->Next = Next; }
        }
        Reference data() const
        { return Data->ref_content(); }
        Pointer address() const
        { return Data->address(); }
    };
    Node *_M_head = nullptr;
    Node *_M_tail = nullptr;
    SizeType _M_size = 0;

    template<typename ... Args>
    Node* _F_construct_node(Node *p, Node *n, const Args & ... args)
    { return new Node(p, n, args...); }

    void _F_add_size(SizeType i)
    { _M_size += i; }

    template<typename ... Args>
    iterator _F_insert(const_iterator it, const Args & ... args);

    void _F_erase(const_iterator it);
    iterator _F_find(ConstReference arg);

public:
    class iterator
    {
    protected:
        Node *_M_current;
        void _F_next()
        {
            if(_M_current != nullptr)
            { _M_current = _M_current->Next; }
        }
        void _F_previous()
        {
            if(_M_current != nullptr)
            { _M_current = _M_current->Previous; }
        }
        const_iterator _F_const_cast()
        { return const_iterator(_M_current); }

        friend DoubleLinkedList;
        iterator(Node *n) : _M_current(n) { }
    public:
        iterator() : _M_current(nullptr) { }
        iterator(const iterator &it)
            : _M_current(it._M_current) { }
        iterator(iterator && it)
            : _M_current(forward<iterator>(it)._M_current) { }

        iterator operator++()
        {
            iterator it = *this;
            _F_next();
            return it;
        }
        iterator operator++(int)
        {
            _F_next();
            return *this;
        }
        iterator operator--()
        {
            iterator it = *this;
            _F_previous();
            return it;
        }
        iterator operator--(int)
        {
            _F_previous();
            return *this;
        }
        Reference operator*() const
        { return _M_current->data(); }
        Pointer operator->() const
        { return _M_current->address(); }
        bool operator==(const iterator &it) const
        { return _M_current == it._M_current; }
        bool operator==(iterator &&it) const
        { return _M_current == forward<iterator>(it)._M_current; }
        bool operator!=(const iterator &it) const
        { return _M_current != it._M_current; }
        bool operator!=(iterator &&it) const
        { return _M_current != forward<iterator>(it)._M_current; }
        iterator operator=(const iterator &it)
        { return _M_current = it._M_current; }
    };

    class reverse_iterator
    {
    protected:
        Node *_M_current;
        void _F_next()
        {
            if(_M_current != nullptr)
            { _M_current = _M_current->Previous; }
        }
        void _F_previous()
        {
            if(_M_current != nullptr)
            { _M_current = _M_current->Next; }
        }

        friend DoubleLinkedList;
        reverse_iterator(Node *n) : _M_current(n) { }
    public:
        reverse_iterator() : _M_current(nullptr) { }
        reverse_iterator(const reverse_iterator &it)
            : _M_current(it._M_current) { }
        reverse_iterator(reverse_iterator && it)
            : _M_current(forward<reverse_iterator>(it)._M_current) { }

        reverse_iterator operator++()
        {
            reverse_iterator it = *this;
            _F_next();
            return it;
        }
        reverse_iterator operator++(int)
        {
            _F_next();
            return *this;
        }
        reverse_iterator operator--()
        {
            reverse_iterator it = *this;
            _F_previous();
            return it;
        }
        reverse_iterator operator--(int)
        {
            _F_previous();
            return *this;
        }
        Reference operator*() const
        { return _M_current->data(); }
        Pointer operator->() const
        { return _M_current->address(); }
        bool operator==(const reverse_iterator &it) const
        { return _M_current == it._M_current; }
        bool operator==(reverse_iterator &&it) const
        { return _M_current == forward<reverse_iterator>(it)._M_current; }
        bool operator!=(const reverse_iterator &it) const
        { return _M_current != it._M_current; }
        bool operator!=(reverse_iterator &&it) const
        { return _M_current != forward<reverse_iterator>(it)._M_current; }
        reverse_iterator operator=(const reverse_iterator &it)
        { return _M_current = it._M_current; }
    };

    class const_iterator
    {
    private:
        const Node *_M_current;
        void _F_next()
        {
            if(_M_current != nullptr)
            { _M_current = _M_current->Next; }
        }
        void _F_previous()
        {
            if(_M_current != nullptr)
            { _M_current = _M_current->Previous; }
        }

        friend DoubleLinkedList;

        iterator _F_const_cast()
        { return iterator(const_cast<Node*>(_M_current)); }
        const_iterator(Node *n) : _M_current(n) { }
    public:
        const_iterator() : _M_current(nullptr) { }
        const_iterator(const const_iterator &it)
            : _M_current(it._M_current) { }
        const_iterator(const_iterator && it)
            : _M_current(forward<const_iterator>(it)._M_current) { }
        const_iterator(const iterator &it)
            : _M_current(it._M_current) { }
        const_iterator(iterator &&it)
            : _M_current(forward<iterator>(it)._M_current) { }

        const_iterator operator++()
        {
            const_iterator it = *this;
            _F_next();
            return it;
        }
        const_iterator operator++(int)
        {
            _F_next();
            return *this;
        }
        const_iterator operator--()
        {
            const_iterator it = *this;
            _F_previous();
            return it;
        }
        const_iterator operator--(int)
        {
            _F_previous();
            return *this;
        }
        Reference operator*() const
        { return _M_current->data(); }
        Pointer operator->() const
        { return _M_current->address(); }
        bool operator==(const const_iterator &it) const
        { return _M_current == it._M_current; }
        bool operator!=(const const_iterator &it) const
        { return _M_current != it._M_current; }
        const_iterator operator=(const const_iterator &it)
        { return _M_current = it._M_current; }
    };

    class const_reverse_iterator
    {
    protected:
        const Node *_M_current;
        void _F_next()
        {
            if(_M_current != nullptr)
            { _M_current = _M_current->Previous; }
        }
        void _F_previous()
        {
            if(_M_current != nullptr)
            { _M_current = _M_current->Next; }
        }

        reverse_iterator _F_const_cast()
        { return reverse_iterator(const_cast<Node*>(_M_current)); }

        friend DoubleLinkedList;
        const_reverse_iterator(Node *n) : _M_current(n) { }
    public:
        const_reverse_iterator() : _M_current(nullptr) { }
        const_reverse_iterator(const const_reverse_iterator &it)
            : _M_current(it._M_current) { }
        const_reverse_iterator(const_reverse_iterator && it)
            : _M_current(forward<const_reverse_iterator>(it)._M_current) { }
        const_reverse_iterator(const reverse_iterator &it)
            : _M_current(it._M_current) { }
        const_reverse_iterator(reverse_iterator &&it)
            : _M_current(forward<reverse_iterator>(it)._M_current) { }

        const_reverse_iterator operator++()
        {
            const_reverse_iterator it = *this;
            _F_next();
            return it;
        }
        const_reverse_iterator operator++(int)
        {
            _F_next();
            return *this;
        }
        const_reverse_iterator operator--()
        {
            const_reverse_iterator it = *this;
            _F_previous();
            return it;
        }
        const_reverse_iterator operator--(int)
        {
            _F_previous();
            return *this;
        }
        Reference operator*() const
        { return _M_current->data(); }
        Pointer operator->() const
        { return _M_current->address(); }
        bool operator==(const const_reverse_iterator &it) const
        { return _M_current == it._M_current; }
        bool operator==(const_reverse_iterator &&it) const
        { return _M_current == forward<const_reverse_iterator>(it)._M_current; }
        bool operator!=(const const_reverse_iterator &it) const
        { return _M_current != it._M_current; }
        bool operator!=(const_reverse_iterator &&it) const
        { return _M_current != forward<const_reverse_iterator>(it)._M_current; }
        const_reverse_iterator operator=(const const_reverse_iterator &it)
        { return _M_current = it._M_current; }
    };

    DoubleLinkedList() { }
    DoubleLinkedList(const DoubleLinkedList<ValueType> &dll)
    { insert<const_iterator>(cend(), dll.begin(), dll.end()); }
    template<typename IteratorType>
    DoubleLinkedList(const IteratorType &b, const IteratorType &e)
    { insert<IteratorType>(cend(), b, e); }
    ~DoubleLinkedList()
    { clear(); }

    SizeType size() const
    { return _M_size; }
    bool empty() const
    { return size() == 0; }

    iterator push_back(ConstReference arg)
    { return insert(end(), arg); }
    iterator push_back(RvalueReference arg)
    { return insert(end(), forward<ValueType>(arg)); }

    iterator push_front(ConstReference arg)
    { return insert(begin(), arg); }
    iterator push_front(RvalueReference arg)
    { return insert(begin(), forward<ValueType>(arg)); }

    void pop_back()
    { erase(iterator(_M_tail)); }
    void pop_front()
    { erase(iterator(_M_head)); }

    void erase(iterator it)
    { _F_erase(it); }
    void erase(const_iterator it)
    { _F_erase(it); }

    iterator insert(iterator it, ConstReference arg)
    { return _F_insert(it, arg); }
    iterator insert(iterator it, RvalueReference arg)
    { return _F_insert(it, forward<ValueType>(arg)); }

    iterator insert(const_iterator it, ConstReference arg)
    { return _F_insert(it, arg); }
    iterator insert(const_iterator it, RvalueReference arg)
    { return _F_insert(it, forward<ValueType>(arg)); }

    template<typename IteratorType>
    iterator insert(const_iterator pos, IteratorType b, IteratorType e);

    void clear();

    iterator begin()
    { return iterator(_M_head); }
    iterator end()
    { return iterator(_M_tail == nullptr ? nullptr : _M_tail->Next); }
    const_iterator begin() const
    { return const_iterator(_M_head); }
    const_iterator end() const
    { return const_iterator(_M_tail == nullptr ? nullptr : _M_tail->Next); }
    const_iterator cbegin() const
    { return const_iterator(_M_head); }
    const_iterator cend() const
    { return const_iterator(_M_tail == nullptr ? nullptr : _M_tail->Next); }
    reverse_iterator rbegin()
    { return reverse_iterator(_M_head); }
    reverse_iterator rend()
    { return reverse_iterator(_M_tail == nullptr ? nullptr : _M_head->Previous); }
    const_reverse_iterator rbegin() const
    { return const_reverse_iterator(_M_head); }
    const_reverse_iterator rend() const
    { return const_reverse_iterator(_M_tail == nullptr ? nullptr : _M_head->Previous); }
    const_reverse_iterator crbegin() const
    { return const_reverse_iterator(_M_head); }
    const_reverse_iterator crend() const
    { return const_reverse_iterator(_M_tail == nullptr ? nullptr : _M_head->Previous); }

    Reference front() const
    { return _M_head == nullptr ? NodeBase<ValueType>().ref_content() : _M_head->data(); }
    Reference back() const
    { return _M_tail == nullptr ? NodeBase<ValueType>().ref_content() : _M_tail->data(); }

    iterator find(ConstReference arg)
    { return _F_find(arg); }
    iterator find(RvalueReference arg)
    { return _F_find(forward<ValueType>(arg)); }

    void reverse();

    template<typename ... Args>
    iterator emplace_front(const Args & ... args)
    { return _F_insert(begin(), args...); }
    template<typename ... Args>
    iterator emplace_front(Args && ... args)
    { return _F_insert(begin(), forward<Args>(args)...); }
    template<typename ... Args>
    iterator emplace_back(const Args & ... args)
    { return _F_insert(end(), args...); }
    template<typename ... Args>
    iterator emplace_back(Args && ... args)
    { return _F_insert(end(), forward<Args>(args)...); }
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
    { return _F_insert(it, forward<Args...>(args...)); }
};

template<typename T>
using Dlist = DoubleLinkedList<T>;

template<typename T>
using List = DoubleLinkedList<T>;

void test_DoubleLinkedList_main();

};
#endif // DOUBLELINKEDLIST_H
