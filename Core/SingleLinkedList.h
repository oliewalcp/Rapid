#ifndef SINGLELINKEDLIST_H
#define SINGLELINKEDLIST_H

#include "Memory.h"
#include "TypeTraits.h"

namespace rapid
{

template<typename T>
class SingleLinkedList
{
public:
    class iterator;
    class const_iterator;
private:
    using ValueType = T;
    using Pointer = ValueType*;
    using Reference = ValueType&;
    using ConstReference = const ValueType &;
    using RvalueReference = ValueType&&;
    using SizeType = size_type;

    struct Node
    {
        NodeBase<ValueType> *Data;
        Node *Next;
        Node() : Data(new NodeBase<ValueType>()), Next(nullptr) { }
        template<typename ... Args>
        Node(Node *n, const Args & ... args)
            : Data(new NodeBase<ValueType>(args...)), Next(n) { }

        ~Node() { delete Data; }
        Reference data() const
        { return Data->ref_content(); }
        Pointer address() const
        { return Data->address(); }
    };
    Node *_M_head = new Node();
    SizeType _M_size = 0;

    template<typename ... Args>
    Node* _F_construct_node(Node *n, const Args & ... args)
    { return new Node(n, args...); }

    void _F_add_size(SizeType i)
    { _M_size += i; }

    template<typename ... Args>
    iterator _F_insert_after(iterator it, const Args &... args)
    {
        _F_add_size(1);
        return iterator(it._M_current->Next = _F_construct_node(it._M_current->Next, args...));
    }

    void _F_erase_after(iterator it)
    {
        if(it == end()) return;
        if(it._M_current->Next != nullptr)
        {
            Node *temp = it._M_current->Next->Next;
            delete it._M_current->Next;
            it._M_current->Next = temp;
        }
        _F_add_size(-1);
    }

    iterator _F_find(ConstReference arg) const;

public:

    class iterator
    {
    private:
        Node *_M_current;

        const_iterator _F_const_cast()
        { return const_iterator(_M_current); }

        friend class SingleLinkedList;
        iterator(Node *c) : _M_current(c) { }
    public:
        iterator() : _M_current(nullptr) { }
        iterator(const iterator &it) : _M_current(it._M_current) { }
        iterator(iterator && it) : _M_current(forward<iterator>(it)._M_current) { }

        iterator operator=(const iterator &arg)
        { return iterator(_M_current = arg._M_current); }
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
        Reference operator*() const
        { return _M_current->data(); }

        Pointer operator->() const
        { return _M_current->address(); }

        bool operator==(const iterator& arg) const
        { return _M_current == arg._M_current; }
        bool operator!=(const iterator& arg) const
        { return _M_current != arg._M_current; }
    };
    class const_iterator
    {
    private:
        const Node *_M_current;

        friend class SingleLinkedList;

        iterator _F_const_cast()
        { return iterator(const_cast<Node*>(_M_current)); }

        const_iterator(Node *c) : _M_current(c) { }
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

        const_iterator operator=(const const_iterator &arg)
        { return const_iterator(_M_current = arg._M_current); }
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
        Reference operator*() const
        { return _M_current->data(); }

        Pointer operator->() const
        { return _M_current->address(); }

        bool operator==(const const_iterator& arg) const
        { return _M_current == arg._M_current; }
        bool operator!=(const const_iterator& arg) const
        { return _M_current != arg._M_current; }
    };

    SingleLinkedList() { }
    SingleLinkedList(const SingleLinkedList<T> &sll)
    { insert_after(before_begin(), sll.begin(), sll.end()); }
    ~SingleLinkedList()
    {
        clear();
        delete _M_head;
    }

    void swap(const SingleLinkedList &sll)
    {
        Node *temp_node = _M_head;
        SizeType temp_size = _M_size;
        _M_head = sll._M_head;
        _M_size = sll._M_size;
        sll._M_head = temp_node;
        sll._M_size = temp_size;
    }

    SizeType size()
    { return _M_size; }
    bool empty()
    { return size() == 0; }

    void push_front(ConstReference arg)
    { _F_insert_after(before_begin(), arg); }
    void push_front(RvalueReference arg)
    { _F_insert_after(before_begin(), forward<ValueType>(arg)); }

    void pop_front()
    { _F_erase_after(before_begin()); }

    void erase_after(iterator it)
    { _F_erase_after(it); }
    void erase_after(const_iterator it)
    { _F_erase_after(it._F_const_cast()); }

    iterator insert_after(iterator it, ConstReference arg)
    { return _F_insert_after(it, arg); }
    iterator insert_after(iterator it, RvalueReference arg)
    { return _F_insert_after(it, forward<ValueType>(arg)); }

    iterator insert_after(const_iterator it, ConstReference arg)
    { return _F_insert_after(it._F_const_cast(), arg); }
    iterator insert_after(const_iterator it, RvalueReference arg)
    { return _F_insert_after(it._F_const_cast(), forward<ValueType>(arg)); }

    template<typename IteratorType>
    iterator insert_after(const_iterator pos, IteratorType b, IteratorType e);

    void clear();
    void reverse();

    iterator before_begin()
    { return iterator(_M_head); }
    iterator begin()
    { return iterator(_M_head->Next); }
    iterator end()
    { return iterator(); }
    const_iterator before_begin() const
    { return const_iterator(_M_head); }
    const_iterator begin() const
    { return const_iterator(_M_head->Next); }
    const_iterator end() const
    { return const_iterator(); }
    const_iterator cbefore_begin() const
    { return const_iterator(_M_head); }
    const_iterator cbegin() const
    { return const_iterator(_M_head->Next); }
    const_iterator cend() const
    { return const_iterator(); }

    Reference front()
    { return empty() ? NodeBase<ValueType>().ref_content() : *begin(); }
    Reference front() const
    { return empty() ? NodeBase<ValueType>().ref_content() : *begin(); }

    iterator find(ConstReference arg)
    { return _F_find(arg); }
    iterator find(RvalueReference arg)
    { return _F_find(forward<ValueType>(arg)); }

    iterator find(ConstReference arg) const
    { return _F_find(arg); }
    iterator find(RvalueReference arg) const
    { return _F_find(forward<ValueType>(arg)); }

    template<typename ... Args>
    iterator emplace_front(const Args & ... args)
    { return _F_insert_after(before_begin(), args...); }
    template<typename ... Args>
    iterator emplace_front(Args && ... args)
    { return _F_insert_after(before_begin(), forward<Args>(args)...); }
    template<typename ... Args>
    iterator emplace_after(const_iterator it, const Args & ... args)
    { return _F_insert_after(it._F_const_cast(), args...); }
    template<typename ... Args>
    iterator emplace_after(iterator it, const Args & ... args)
    { return _F_insert_after(it, args...); }
    template<typename ... Args>
    iterator emplace_after(const_iterator it, Args && ... args)
    { return _F_insert_after(it._F_const_cast(), forward<Args>(args)...); }
    template<typename ... Args>
    iterator emplace_after(iterator it, Args && ... args)
    { return _F_insert_after(it, forward<Args>(args)...); }

};

template<typename T>
using Slist = SingleLinkedList<T>;


void test_SingleLinkedList_main();
};
#endif // SINGLELINKEDLIST_H
