#ifndef DOUBLELINKEDLIST_H
#define DOUBLELINKEDLIST_H

#include "Memory.h"

namespace rapid
{

template<typename T>
class DoubleLinkedList
{
public:
    class iterator;
    class reverse_iterator;
private:
    using Type = T;
    using Pointer = Type*;
    using Reference = Type&;
    using ConstReference = const Type &;
    using RvalueReference = Type&&;
    using SizeType = long;

    struct Node
    {
        NodeBase<Type> *Data;
        Node *Next;
        Node *Previous;
        Node(ConstReference arg, Node *n = nullptr, Node *p = nullptr) : Data(new NodeBase<Type>(arg)), Next(n), Previous(p) { }
        ~Node() { delete Data; }
    };
    Node *__Head;
    Node *__Tail;
    SizeType __Size;

    inline Node* __construct_node(ConstReference arg, Node *n = nullptr, Node *p = nullptr) { return new Node(arg, n, p); }
    inline void __add_size(SizeType i) { __Size += i; }
    void __create_list(ConstReference arg);
    void __create_list(Node *n);
    inline void __initialize(Node *n)
    { __Tail = __Head = n; }
    void __insert(const iterator &it, ConstReference arg);
    void __erase(const iterator &it);
    iterator __find(ConstReference arg);

public:

    class iterator
    {
    private:
        Node *__Current;

        friend class DoubleLinkedList;

        inline void __set(const iterator &it)
        { __Current = it.__Current; }

        iterator(Node *n) : __Current(n) { }
    public:
        iterator() : __Current(nullptr) { }
        iterator(const iterator &it) { __set(it); }
        iterator(iterator && it) { __set(std::forward<iterator>(it)); }

        iterator operator=(const iterator &it)
        { __set(it); }
        iterator operator++()
        {
            if(__Current != nullptr)
            { __Current = __Current->Next; }
            return *this;
        }
        iterator operator++(int)
        {
            iterator it = *this;
            if(__Current != nullptr)
            { __Current = __Current->Next; }
            return it;
        }
        iterator operator--()
        {
            if(__Current != nullptr && __Current->Previous != nullptr)
            { __Current = __Current->Previous; }
            return *this;
        }
        iterator operator--(int)
        {
            iterator it = *this;
            if(__Current != nullptr && __Current->Previous != nullptr)
            { __Current = __Current->Previous; }
            return it;
        }
        Type operator*() const
        { return __Current->Data->content(); }

        Type* operator->() const
        { return &__Current->Data->address(); }

        bool operator==(const iterator& arg) const
        { return __Current == arg.__Current; }
        bool operator!=(const iterator& arg) const
        { return __Current != arg.__Current; }
    };

    class reverse_iterator
    {
    private:
        Node *__Current;

        friend class DoubleLinkedList;

        inline void __set(Node *n)
        { __Current = n; }
        inline void __set(const reverse_iterator &it)
        { __set(it.__Current); }

        reverse_iterator(Node *n) : __Current(n) { }
    public:
        reverse_iterator(const reverse_iterator &it) { __set(it); }
        reverse_iterator(reverse_iterator && it) { __set(std::forward<reverse_iterator>(it)); }

        reverse_iterator operator++()
        {
            if(__Current != nullptr)
            { __Current = __Current->Previous; }
            return *this;
        }
        reverse_iterator operator++(int)
        {
            reverse_iterator it = *this;
            if(__Current != nullptr)
            { __Current = __Current->Previous; }
            return it;
        }
        reverse_iterator operator--()
        {
            if(__Current != nullptr && __Current->Next != nullptr)
            { __Current = __Current->Next; }
            return *this;
        }
        reverse_iterator operator--(int)
        {
            reverse_iterator it = *this;
            if(__Current != nullptr && __Current->Next != nullptr)
            { __Current = __Current->Next; }
            return it;
        }
        Type operator*() const
        { return __Current->Data->content(); }

        Type* operator->() const
        { return &__Current->Data->address(); }

        bool operator==(const reverse_iterator& arg) const
        { return __Current == arg.__Current; }
        bool operator!=(const reverse_iterator& arg) const
        { return __Current != arg.__Current; }
    };

    DoubleLinkedList() : __Head(nullptr), __Tail(nullptr), __Size(0) { }
    DoubleLinkedList(const DoubleLinkedList<Type> &dll);
    virtual ~DoubleLinkedList() { clear(); }

    inline SizeType size()
    { return __Size; }
    inline bool empty()
    { return __Size == 0; }

    inline void push_back(ConstReference arg)
    { insert(end(), arg); }
    inline void push_back(RvalueReference arg)
    { insert(end(), std::forward<Type>(arg)); }

    inline void push_front(ConstReference arg)
    { insert(begin(), arg); }
    inline void push_front(RvalueReference arg)
    { insert(begin(), std::forward<Type>(arg)); }

    inline void pop_back()
    { erase(iterator(__Tail)); }
    inline void pop_front()
    { erase(iterator(__Head)); }

    inline void erase(iterator &it)
    { __erase(it); }
    inline void erase(iterator && it)
    { __erase(std::forward<iterator>(it)); }
    inline void insert(iterator &it, ConstReference arg)
    { __insert(it, arg); }
    inline void insert(iterator && it, ConstReference arg)
    { __insert(std::forward<iterator>(it), arg); }
    inline void insert(iterator &it, RvalueReference arg)
    { __insert(it, std::forward<Type>(arg)); }
    inline void insert(iterator && it, RvalueReference arg)
    { __insert(std::forward<iterator>(it), std::forward<Type>(arg)); }

    void clear();

    iterator begin() { return iterator(__Head); }
    iterator end() { return iterator(__Tail == nullptr ? nullptr : __Tail->Next); }
    reverse_iterator rbegin() { return reverse_iterator(__Tail); }
    reverse_iterator rend() { return reverse_iterator(__Head == nullptr ? nullptr : __Head->Previous); }

    Type front();
    Type back();

    inline iterator find(ConstReference arg) { return __find(arg); }
    inline iterator find(RvalueReference arg) { return __find(std::forward<Type>(arg)); }

};

template<typename T>
using Dlist = DoubleLinkedList<T>;

template<typename T>
using List = DoubleLinkedList<T>;

#ifndef NDEBUG
void test_DoubleLinkedList_main();
#endif

};
#endif // DOUBLELINKEDLIST_H
