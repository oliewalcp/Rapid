#ifndef DOUBLELINKEDLIST_H
#define DOUBLELINKEDLIST_H

#include "Memory.h"
#include "TypeTraits.h"

namespace rapid
{

template<typename T>
class DoubleLinkedList
{
public:
    class iterator;
    class reverse_iterator;

    using ValueType = T;
    using Pointer = ValueType*;
    using Reference = ValueType&;
    using ConstReference = const ValueType &;
    using RvalueReference = ValueType&&;
    using SizeType = size_t;

    using const_iterator = iterator;
    using const_reverse_iterator = const reverse_iterator;
private:

    struct Node
    {
        NodeBase<ValueType> *Data;
        Node *Next;
        Node *Previous;
        Node(ConstReference arg, Node *n = nullptr, Node *p = nullptr) : Data(new NodeBase<ValueType>(arg)), Next(n), Previous(p) { }
        ~Node() { delete Data; }
    };
    Node *_M_head;
    Node *_M_tail;
    SizeType _M_size;

    inline Node* _F_construct_node(ConstReference arg, Node *n = nullptr, Node *p = nullptr) { return new Node(arg, n, p); }
    inline void _F_add_size(SizeType i) { _M_size += i; }
    void _F_create_list(ConstReference arg);
    void _F_create_list(Node *n);
    inline void _F_initialize(Node *n)
    { _M_tail = _M_head = n; }
    void _F_insert(const iterator &it, ConstReference arg);
    void _F_erase(const iterator &it);
    iterator _F_find(ConstReference arg);

public:

    class iterator
    {
    private:
        Node *_M_current;

        friend class DoubleLinkedList;

        inline void _M_set(const iterator &it)
        { _M_current = it._M_current; }

        iterator(Node *n) : _M_current(n) { }
    public:
        iterator() : _M_current(nullptr) { }
        iterator(const iterator &it) { _M_set(it); }
        iterator(iterator && it) { _M_set(forward<iterator>(it)); }

        iterator operator=(const iterator &it)
        { _M_set(it); }
        iterator operator++()
        {
            if(_M_current != nullptr)
            { _M_current = _M_current->Next; }
            return *this;
        }
        iterator operator++(int)
        {
            iterator it = *this;
            if(_M_current != nullptr)
            { _M_current = _M_current->Next; }
            return it;
        }
        iterator operator--()
        {
            if(_M_current != nullptr && _M_current->Previous != nullptr)
            { _M_current = _M_current->Previous; }
            return *this;
        }
        iterator operator--(int)
        {
            iterator it = *this;
            if(_M_current != nullptr && _M_current->Previous != nullptr)
            { _M_current = _M_current->Previous; }
            return it;
        }
        ValueType operator*() const
        { return _M_current->Data->content(); }

        ValueType* operator->() const
        { return &_M_current->Data->address(); }

        bool operator==(const iterator& arg) const
        { return _M_current == arg._M_current; }
        bool operator!=(const iterator& arg) const
        { return _M_current != arg._M_current; }
    };

    class reverse_iterator
    {
    private:
        Node *_M_current;

        friend class DoubleLinkedList;

        inline void __set(Node *n)
        { _M_current = n; }
        inline void __set(const reverse_iterator &it)
        { __set(it._M_current); }

        reverse_iterator(Node *n) : _M_current(n) { }
    public:
        reverse_iterator(const reverse_iterator &it) { __set(it); }
        reverse_iterator(reverse_iterator && it) { __set(forward<reverse_iterator>(it)); }

        reverse_iterator operator++()
        {
            if(_M_current != nullptr)
            { _M_current = _M_current->Previous; }
            return *this;
        }
        reverse_iterator operator++(int)
        {
            reverse_iterator it = *this;
            if(_M_current != nullptr)
            { _M_current = _M_current->Previous; }
            return it;
        }
        reverse_iterator operator--()
        {
            if(_M_current != nullptr && _M_current->Next != nullptr)
            { _M_current = _M_current->Next; }
            return *this;
        }
        reverse_iterator operator--(int)
        {
            reverse_iterator it = *this;
            if(_M_current != nullptr && _M_current->Next != nullptr)
            { _M_current = _M_current->Next; }
            return it;
        }
        ValueType operator*() const
        { return _M_current->Data->content(); }

        ValueType* operator->() const
        { return &_M_current->Data->address(); }

        bool operator==(const reverse_iterator& arg) const
        { return _M_current == arg._M_current; }
        bool operator!=(const reverse_iterator& arg) const
        { return _M_current != arg._M_current; }
    };

    DoubleLinkedList() : _M_head(nullptr), _M_tail(nullptr), _M_size(0) { }
    DoubleLinkedList(const DoubleLinkedList<ValueType> &dll);
    virtual ~DoubleLinkedList() { clear(); }

    inline SizeType size()
    { return _M_size; }
    inline bool empty()
    { return _M_size == 0; }

    inline void push_back(ConstReference arg)
    { insert(end(), arg); }
    inline void push_back(RvalueReference arg)
    { insert(end(), forward<ValueType>(arg)); }

    inline void push_front(ConstReference arg)
    { insert(begin(), arg); }
    inline void push_front(RvalueReference arg)
    { insert(begin(), forward<ValueType>(arg)); }

    inline void pop_back()
    { erase(iterator(_M_tail)); }
    inline void pop_front()
    { erase(iterator(_M_head)); }

    inline void erase(iterator &it)
    { _F_erase(it); }
    inline void erase(iterator && it)
    { _F_erase(forward<iterator>(it)); }
    inline void insert(iterator &it, ConstReference arg)
    { _F_insert(it, arg); }
    inline void insert(iterator && it, ConstReference arg)
    { _F_insert(forward<iterator>(it), arg); }
    inline void insert(iterator &it, RvalueReference arg)
    { _F_insert(it, forward<ValueType>(arg)); }
    inline void insert(iterator && it, RvalueReference arg)
    { _F_insert(forward<iterator>(it), forward<ValueType>(arg)); }

    void clear();

    iterator begin()
    { return iterator(_M_head); }
    iterator end()
    { return iterator(_M_tail == nullptr ? nullptr : _M_tail->Next); }
    reverse_iterator rbegin()
    { return reverse_iterator(_M_tail); }
    reverse_iterator rend()
    { return reverse_iterator(_M_head == nullptr ? nullptr : _M_head->Previous); }

    ValueType front();
    ValueType back();

    inline iterator find(ConstReference arg)
    { return _F_find(arg); }
    inline iterator find(RvalueReference arg)
    { return _F_find(forward<ValueType>(arg)); }

};

template<typename T>
using Dlist = DoubleLinkedList<T>;

template<typename T>
using List = DoubleLinkedList<T>;

};
#endif // DOUBLELINKEDLIST_H
