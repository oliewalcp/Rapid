#ifndef DOUBLELINKEDLIST_H
#define DOUBLELINKEDLIST_H

#include "Core/TLNode.h"
#include "Core/TypeTraits.h"
#include "Core/Version.h"
#include "Core/Compare.h"
#include <initializer_list>

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
        void swap(Node *node)
        {
            NodeBase<ValueType> *temp = Data;
            Data = node->Data;
            node->Data = temp;
        }
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
    public:
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

        iterator(Node *n) : _M_current(n) { }
        iterator() : _M_current(nullptr) { }
        iterator(const iterator &it)
            : _M_current(it._M_current) { }
        iterator(iterator && it)
            : _M_current(forward<iterator>(it)._M_current) { }

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
        Reference operator*() const
        { return _M_current->data(); }
        Pointer operator->() const
        { return _M_current->address(); }
        bool operator==(const iterator &it) const
        { return _M_current == it._M_current; }
        bool operator!=(const iterator &it) const
        { return _M_current != it._M_current; }
        iterator operator=(const iterator &it)
        { return iterator(_M_current = it._M_current); }
    };

    class reverse_iterator
    {
    public:
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

        reverse_iterator(Node *n) : _M_current(n) { }
        reverse_iterator() : _M_current(nullptr) { }
        reverse_iterator(const reverse_iterator &it)
            : _M_current(it._M_current) { }
        reverse_iterator(reverse_iterator && it)
            : _M_current(forward<reverse_iterator>(it)._M_current) { }

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
        Reference operator*() const
        { return _M_current->data(); }
        Pointer operator->() const
        { return _M_current->address(); }
        bool operator==(const reverse_iterator &it) const
        { return _M_current == it._M_current; }
        bool operator!=(const reverse_iterator &it) const
        { return _M_current != it._M_current; }
        reverse_iterator operator=(const reverse_iterator &it)
        { return reverse_iterator(_M_current = it._M_current); }
    };

    class const_iterator
    {
    public:
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

        iterator _F_const_cast()
        { return iterator(const_cast<Node*>(_M_current)); }
        const_iterator(Node *n) : _M_current(n) { }
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
        Reference operator*() const
        { return _M_current->data(); }
        Pointer operator->() const
        { return _M_current->address(); }
        bool operator==(const const_iterator &it) const
        { return _M_current == it._M_current; }
        bool operator!=(const const_iterator &it) const
        { return _M_current != it._M_current; }
        const_iterator operator=(const const_iterator &it)
        { return const_iterator(_M_current = it._M_current); }
    };

    class const_reverse_iterator
    {
    public:
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

        const_reverse_iterator(Node *n) : _M_current(n) { }
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
        Reference operator*() const
        { return _M_current->data(); }
        Pointer operator->() const
        { return _M_current->address(); }
        bool operator==(const const_reverse_iterator &it) const
        { return _M_current == it._M_current; }
        bool operator!=(const const_reverse_iterator &it) const
        { return _M_current != it._M_current; }
        const_reverse_iterator operator=(const const_reverse_iterator &it)
        { return const_reverse_iterator(_M_current = it._M_current); }
    };

    DoubleLinkedList() { }
    DoubleLinkedList(const DoubleLinkedList<ValueType> &dll)
    { insert<const_iterator>(cend(), dll.begin(), dll.end()); }
    template<typename IteratorType>
    DoubleLinkedList(const IteratorType &b, const IteratorType &e)
    { insert<IteratorType>(cend(), b, e); }
    DoubleLinkedList(std::initializer_list<ValueType> arg)
    {
        for(auto it = arg.begin(); it != arg.end(); ++it)
        { push_back(*it); }
    }
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

    void sort()
    { sort(Compare<ValueType>()); }
    template<typename _Compare>
    void sort(_Compare c);
};
//-----------------------impl-----------------------//
//-----------------------impl-----------------------//
//-----------------------impl-----------------------//
//-----------------------impl-----------------------//
//-----------------------impl-----------------------//
template<typename T>
template<typename ... Args>
typename DoubleLinkedList<T>::iterator
    DoubleLinkedList<T>::_F_insert(const_iterator it, const Args & ... args)
{
    _F_add_size(1);
    if(it == cend())
    {
        _M_tail = _F_construct_node(_M_tail, nullptr, args...);
        if(_M_head == nullptr)
        {
            _M_head = _M_tail;
        }
        return iterator(_M_tail);
    }
    iterator remove_const_it = it._F_const_cast();
    Node *n = _F_construct_node(remove_const_it._M_current->Previous, remove_const_it._M_current, args...);
    if(_M_head->Previous != nullptr)
    {
        _M_head = _M_head->Previous;
    }
    return iterator(n);
}

template<typename T>
template<typename IteratorType>
typename DoubleLinkedList<T>::iterator
    DoubleLinkedList<T>::insert(const_iterator pos, IteratorType b, IteratorType e)
{
    iterator result = pos._F_const_cast();
    while(b != e)
    {
        result = insert(result, *b);
        ++b;
    }
    return result;
}

template<typename T>
void DoubleLinkedList<T>::_F_erase(const_iterator it)
{
    if(it == end())
        return;
    if(it._M_current == _M_tail)
    {
        _M_tail = _M_tail->Previous;
    }
    else if(it._M_current == _M_head)
    {
        _M_head = _M_head->Next;
    }
    it._F_const_cast()._M_current->dealloc();
    delete it._M_current;
    _F_add_size(-1);
}

template<typename T>
typename DoubleLinkedList<T>::iterator
    DoubleLinkedList<T>::_F_find(ConstReference arg)
{
    Node *temp = _M_head;
    while(temp != nullptr)
    {
        if(temp->Data->const_ref_content() == arg)
        { return iterator(temp); }
        temp = temp->Next;
    }
    return end();
}

template<typename T>
void DoubleLinkedList<T>::clear()
{
    Node *n = _M_head;
    while(n != nullptr)
    {
        Node *m = n->Next;
        delete n;
        n = m;
    }
    _M_head = _M_tail = nullptr;
    _M_size = 0;
}

template<typename T>
void DoubleLinkedList<T>::reverse()
{
    Node *n1 = _M_head;
    Node *n2 = _M_tail;
    while(n1 != n2 && n2->Next != n1)
    {
        NodeBase<ValueType> *temp = n1->Data;
        n1->Data = n2->Data;
        n2->Data = temp;
        n1 = n1->Next;
        n2 = n2->Previous;
    }
}

template<typename T>
template<typename _Compare>
void DoubleLinkedList<T>::sort(_Compare c)
{
    Node *max_pos = nullptr;
    Node *min_pos = _M_head;
    for(Node *v = _M_head; v != nullptr; v = v->Next)
    {
        Node *current_pos = nullptr;
        for(Node *b = min_pos; b->Next != max_pos; b = b->Next)
        {
            Node *temp = b->Next;
            if(c(temp->data(), b->data()) > 0)
            {
                temp->swap(b);
                current_pos = temp;
            }
        }
        if(current_pos == nullptr)
        {
            break;
        }
        max_pos = current_pos;
        current_pos = nullptr;
        for(Node *n = max_pos; n != min_pos; n = n->Previous)
        {
            Node *temp = n->Previous;
            if(c(n->data(), temp->data()) > 0)
            {
                temp->swap(n);
                current_pos = n;
            }
        }
        min_pos = min_pos->Next;
        if(current_pos == nullptr)
        {
            break;
        }
    }
}

template<typename T>
using Dlist = DoubleLinkedList<T>;

template<typename T>
using List = DoubleLinkedList<T>;



};
#endif // DOUBLELINKEDLIST_H
