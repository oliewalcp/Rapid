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
    using SizeType = size_type;

private:

    struct Node
    {
        NodeBase<ValueType> *Data;
        Node *Next;
        Node *Previous;
        Node(ConstReference arg, Node *p = nullptr, Node *n = nullptr)
            : Data(new NodeBase<ValueType>(arg)), Next(n), Previous(p)
        {
            if(p != nullptr)
            { p->set_next(this); }
            if(n != nullptr)
            { n->set_previous(this); }
        }
        ~Node()
        {
            delete Data;
            if(Next != nullptr)
            { delete Next; }
        }
        void dealloc()
        {
            if(Previous != nullptr)
            { Previous->set_next(Next); }
            if(Next != nullptr)
            { Next->set_previous(Previous); }
            set_next(nullptr);
        }
        void set_next(Node *n)
        { Next = n; }
        void set_previous(Node *n)
        { Previous = n; }
    };
    Node *_M_head = nullptr;
    Node *_M_tail = nullptr;
    SizeType _M_size = 0;

    inline Node* _F_construct_node(ConstReference arg, Node *p = nullptr, Node *n = nullptr)
    { return new Node(arg, p, n); }
    inline void _F_add_size(SizeType i)
    { _M_size += i; }
    iterator _F_insert(const iterator &it, ConstReference arg)
    {
        _F_add_size(1);
        if(it == end())
        {
            _M_tail = _F_construct_node(arg, _M_tail);
            if(_M_head == nullptr)
            { _M_head = _M_tail; }
            return iterator(_M_tail);
        }
        Node *n = _F_construct_node(arg, it._M_current->Previous, it._M_current);
        if(_M_head->Previous != nullptr)
        { _M_head = _M_head->Previous; }
        return iterator(n);
    }
    void _F_erase(const iterator &it)
    {
        if(it == end()) return;
        if(it._M_current == _M_tail)
        { _M_tail = _M_tail->Previous; }
        else if(it._M_current == _M_head)
        { _M_head = _M_head->Next; }
        it._M_current->dealloc();
        delete it._M_current;
        _F_add_size(-1);
    }
    iterator _F_find(ConstReference arg);

public:

    class iterator
    {
    private:
        Node *_M_current;

        friend class DoubleLinkedList;

        iterator(Node *n) : _M_current(n) { }
    public:
        iterator() : _M_current(nullptr) { }
        iterator(const iterator &it) : _M_current(it._M_current) { }
        iterator(iterator && it) : _M_current(forward<iterator>(it)._M_current) { }

        iterator operator=(const iterator &it)
        { return iterator(_M_current = it._M_current); }
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
        { return _M_current->Data->address(); }

        bool operator==(const iterator& arg) const
        { return _M_current == arg._M_current; }
        bool operator!=(const iterator& arg) const
        { return _M_current != arg._M_current; }
    };

    class const_iterator
    {
    private:
        Node *_M_current;

        friend class DoubleLinkedList;

        const_iterator(Node *n) : _M_current(n) { }
    public:
        const_iterator() : _M_current(nullptr) { }
        const_iterator(const const_iterator &it) : _M_current(it._M_current) { }
        const_iterator(const_iterator && it) : _M_current(forward<const_iterator>(it)._M_current) { }

        const_iterator operator=(const const_iterator &it)
        { return const_iterator(_M_current = it._M_current); }
        const_iterator operator++()
        {
            if(_M_current != nullptr)
            { _M_current = _M_current->Next; }
            return *this;
        }
        const_iterator operator++(int)
        {
            const_iterator it = *this;
            if(_M_current != nullptr)
            { _M_current = _M_current->Next; }
            return it;
        }
        const_iterator operator--()
        {
            if(_M_current != nullptr && _M_current->Previous != nullptr)
            { _M_current = _M_current->Previous; }
            return *this;
        }
        const_iterator operator--(int)
        {
            const_iterator it = *this;
            if(_M_current != nullptr && _M_current->Previous != nullptr)
            { _M_current = _M_current->Previous; }
            return it;
        }
        ValueType operator*() const
        { return _M_current->Data->content(); }

        ValueType* operator->() const
        { return _M_current->Data->address(); }

        bool operator==(const const_iterator& arg) const
        { return _M_current == arg._M_current; }
        bool operator!=(const const_iterator& arg) const
        { return _M_current != arg._M_current; }
    };

    class reverse_iterator
    {
    private:
        Node *_M_current;

        friend class DoubleLinkedList;

        reverse_iterator(Node *n) : _M_current(n) { }
    public:
        reverse_iterator(const reverse_iterator &it) : _M_current(it._M_current) { }
        reverse_iterator(reverse_iterator && it) : _M_current(forward<reverse_iterator>(it)._M_current) { }

        reverse_iterator operator=(const reverse_iterator &it)
        { return reverse_iterator(_M_current = it._M_current); }
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
        { return _M_current->Data->address(); }

        bool operator==(const reverse_iterator& arg) const
        { return _M_current == arg._M_current; }
        bool operator!=(const reverse_iterator& arg) const
        { return _M_current != arg._M_current; }
    };

    class const_reverse_iterator
    {
    private:
        Node *_M_current;

        friend class DoubleLinkedList;

        const_reverse_iterator(Node *n) : _M_current(n) { }
    public:
        const_reverse_iterator(const const_reverse_iterator &it) : _M_current(it._M_current) { }
        const_reverse_iterator(const_reverse_iterator && it) : _M_current(forward<const_reverse_iterator>(it)._M_current) { }

        const_reverse_iterator operator=(const const_reverse_iterator &it)
        { return const_reverse_iterator(_M_current = it._M_current); }
        const_reverse_iterator operator++()
        {
            if(_M_current != nullptr)
            { _M_current = _M_current->Previous; }
            return *this;
        }
        const_reverse_iterator operator++(int)
        {
            const_reverse_iterator it = *this;
            if(_M_current != nullptr)
            { _M_current = _M_current->Previous; }
            return it;
        }
        const_reverse_iterator operator--()
        {
            if(_M_current != nullptr && _M_current->Next != nullptr)
            { _M_current = _M_current->Next; }
            return *this;
        }
        const_reverse_iterator operator--(int)
        {
            reverse_iterator it = *this;
            if(_M_current != nullptr && _M_current->Next != nullptr)
            { _M_current = _M_current->Next; }
            return it;
        }
        ValueType operator*() const
        { return _M_current->Data->content(); }

        ValueType* operator->() const
        { return _M_current->Data->address(); }

        bool operator==(const const_reverse_iterator& arg) const
        { return _M_current == arg._M_current; }
        bool operator!=(const const_reverse_iterator& arg) const
        { return _M_current != arg._M_current; }
    };

    DoubleLinkedList() { }
    DoubleLinkedList(DoubleLinkedList<ValueType> &dll)
    {
        for(auto it = dll.begin(); it != dll.end(); ++it)
        { push_back(*it); }
    }
    ~DoubleLinkedList()
    { clear(); }

    inline SizeType size()
    { return _M_size; }
    inline bool empty()
    { return _M_size == 0; }

    inline iterator push_back(ConstReference arg)
    { return insert(end(), arg); }
    inline iterator push_back(RvalueReference arg)
    { return insert(end(), forward<ValueType>(arg)); }

    inline iterator push_front(ConstReference arg)
    { return insert(begin(), arg); }
    inline iterator push_front(RvalueReference arg)
    { return insert(begin(), forward<ValueType>(arg)); }

    inline void pop_back()
    { erase(iterator(_M_tail)); }
    inline void pop_front()
    { erase(iterator(_M_head)); }

    inline void erase(iterator &it)
    { _F_erase(it); }
    inline void erase(iterator && it)
    { _F_erase(forward<iterator>(it)); }
    inline iterator insert(iterator &it, ConstReference arg)
    { return _F_insert(it, arg); }
    inline iterator insert(iterator && it, ConstReference arg)
    { return _F_insert(forward<iterator>(it), arg); }
    inline iterator insert(iterator &it, RvalueReference arg)
    { return _F_insert(it, forward<ValueType>(arg)); }
    inline iterator insert(iterator && it, RvalueReference arg)
    { return _F_insert(forward<iterator>(it), forward<ValueType>(arg)); }

    void clear()
    {
        if(_M_head != nullptr)
        { delete _M_head; }
        _M_head = _M_tail = nullptr;
        _M_size = 0;
    }

    iterator begin()
    { return iterator(_M_head); }
    iterator end()
    { return iterator(_M_tail == nullptr ? nullptr : _M_tail->Next); }
    const_iterator cbegin()
    { return const_iterator(_M_head); }
    const_iterator cend()
    { return const_iterator(_M_tail == nullptr ? nullptr : _M_tail->Next); }
    reverse_iterator rbegin()
    { return reverse_iterator(_M_head); }
    reverse_iterator rend()
    { return reverse_iterator(_M_tail == nullptr ? nullptr : _M_head->Previous); }
    const_reverse_iterator crbegin()
    { return const_reverse_iterator(_M_head); }
    const_reverse_iterator crend()
    { return const_reverse_iterator(_M_tail == nullptr ? nullptr : _M_head->Previous); }

    ValueType front()
    { return _M_head == nullptr ? NodeBase<ValueType>().content() : _M_head->Data->content(); }
    ValueType back()
    { return _M_tail == nullptr ? NodeBase<ValueType>().content() : _M_tail->Data->content(); }

    inline iterator find(ConstReference arg)
    { return _F_find(arg); }
    inline iterator find(RvalueReference arg)
    { return _F_find(forward<ValueType>(arg)); }

};

template<typename T>
using Dlist = DoubleLinkedList<T>;

template<typename T>
using List = DoubleLinkedList<T>;

void test_DoubleLinkedList_main();

};
#endif // DOUBLELINKEDLIST_H
