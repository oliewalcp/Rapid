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
private:
    using Type = T;
    using Pointer = Type*;
    using Reference = Type&;
    using ConstReference = const Type &;
    using RvalueReference = Type&&;
    using SizeType = size_type;

    struct Node
    {
        NodeBase<Type> *Data;
        Node *Next;
        Node(ConstReference arg, Node *n = nullptr) : Data(new NodeBase<Type>(arg)), Next(n) { }
        ~Node() { delete Data; }
    };
    Node *_M_head;
    Node *_M_tail;
    SizeType _M_size;

    inline Node* _F_construct_node(ConstReference arg) { return new Node(arg); }
    inline void _F_add_size(SizeType i) { _M_size += i; }
    void _F_create_list(ConstReference arg);
    void _F_create_list(Node *n);
    inline void _F_initialize(Node *n)
    { _M_tail = _M_head = n; }

    void _F_insert(const iterator &it, ConstReference arg);
    void _F_erase(iterator &it);
    iterator _F_find(ConstReference arg);

public:

    class iterator
    {
    private:
        Node *_M_pervious;
        Node *_M_next;
        Node *_M_current;

        friend class SingleLinkedList;

        inline void _F_set(Node *p, Node *c , Node *n)
        {
            _M_pervious = p;
            _M_current = c;
            _M_next = n;
        }
        inline void _F_set(const iterator &it) { _F_set(it._M_pervious, it._M_current, it._M_next); }

        iterator(Node *p, Node *c , Node *n) : _M_pervious(p), _M_next(n), _M_current(c) { }
    public:
        iterator() : _M_pervious(nullptr), _M_next(nullptr), _M_current(nullptr) {}
        iterator(const iterator &it) { _F_set(it); }
        iterator(iterator && it) { _F_set(forward<iterator>(it)); }

        iterator operator=(const iterator &arg)
        { _F_set(arg._M_pervious, arg._M_current, arg._M_next); }
        iterator operator++()
        {
            _M_pervious = _M_current;
            _M_current = _M_next;
            if(_M_next != nullptr)
            { _M_next = _M_next->Next; }
            return *this;
        }
        iterator operator++(int)
        {
            iterator it = *this;
            _M_pervious = _M_current;
            _M_current = _M_next;
            if(_M_next != nullptr)
            { _M_next = _M_next->Next; }
            return it;
        }
        Type operator*() const
        { return _M_current->Data->content(); }

        Type* operator->() const
        { return &_M_current->Data->address(); }

        bool operator==(const iterator& arg) const
        { return _M_current == arg._M_current; }
        bool operator!=(const iterator& arg) const
        { return _M_current != arg._M_current; }
    };

    SingleLinkedList() : _M_head(nullptr), _M_tail(nullptr), _M_size(0) { }
    SingleLinkedList(const SingleLinkedList<T> &sll);
    virtual ~SingleLinkedList() { clear(); }

    inline SizeType size()
    { return _M_size; }
    inline bool empty()
    { return _M_size == 0; }

    inline void push_back(ConstReference arg)
    { insert(end(), arg); }
    inline void push_back(RvalueReference arg)
    { insert(end(), forward<Type>(arg)); }

    inline void push_front(ConstReference arg)
    { insert(begin(), arg); }
    inline void push_front(RvalueReference arg)
    { insert(begin(), forward<Type>(arg)); }

    void pop_back();
    void pop_front();

    inline void erase(iterator it)
    { _F_erase(it); }
    inline void insert(iterator &it, ConstReference arg)
    { _F_insert(it, arg); }
    inline void insert(iterator && it, ConstReference arg)
    { _F_insert(forward<iterator>(it), arg); }
    inline void insert(iterator &it, RvalueReference arg)
    { _F_insert(it, forward<Type>(arg)); }
    inline void insert(iterator && it, RvalueReference arg)
    { _F_insert(forward<iterator>(it), forward<Type>(arg)); }

    void clear();

    inline iterator begin() { return iterator(nullptr, _M_head, _M_head == nullptr ? nullptr : _M_head->Next); }
    inline iterator end() { return iterator(_M_tail, nullptr, nullptr); }

    Type front();
    Type back();

    iterator find(ConstReference arg)
    { return _F_find(arg); }
    iterator find(RvalueReference arg)
    { return _F_find(forward<Type>(arg)); }

};

template<typename T>
using Slist = SingleLinkedList<T>;


void test_SingleLinkedList_main();
};
#endif // SINGLELINKEDLIST_H
