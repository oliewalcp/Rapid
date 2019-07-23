#ifndef SINGLELINKEDLIST_H
#define SINGLELINKEDLIST_H

#include "Memory.h"

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
    using SizeType = long;

    struct Node
    {
        NodeBase<Type> *Data;
        Node *Next;
        Node(ConstReference arg, Node *n = nullptr) : Data(new NodeBase<Type>(arg)), Next(n) { }
        ~Node() { delete Data; }
    };
    Node *__Head;
    Node *__Tail;
    SizeType __Size;

    inline Node* __construct_node(ConstReference arg) { return new Node(arg); }
    inline void __add_size(SizeType i) { __Size += i; }
    void __create_list(ConstReference arg);
    void __create_list(Node *n);
    inline void __initialize(Node *n)
    { __Tail = __Head = n; }

    void __insert(const iterator &it, ConstReference arg);
    void __erase(iterator &it);
    iterator __find(ConstReference arg);

public:

    class iterator
    {
    private:
        Node *__Pervious;
        Node *__Next;
        Node *__Current;

        friend class SingleLinkedList;

        inline void __set(Node *p, Node *c , Node *n)
        {
            __Pervious = p;
            __Current = c;
            __Next = n;
        }
        inline void __set(const iterator &it) { __set(it.__Pervious, it.__Current, it.__Next); }

        iterator(Node *p, Node *c , Node *n) : __Pervious(p), __Next(n), __Current(c) { }
    public:
        iterator() : __Pervious(nullptr), __Next(nullptr), __Current(nullptr) {}
        iterator(const iterator &it) { __set(it); }
        iterator(iterator && it) { __set(std::forward<iterator>(it)); }

        iterator operator=(const iterator &arg)
        { __set(arg.__Pervious, arg.__Current, arg.__Next); }
        iterator operator++()
        {
            __Pervious = __Current;
            __Current = __Next;
            if(__Next != nullptr)
            { __Next = __Next->Next; }
            return *this;
        }
        iterator operator++(int)
        {
            iterator it = *this;
            __Pervious = __Current;
            __Current = __Next;
            if(__Next != nullptr)
            { __Next = __Next->Next; }
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

    SingleLinkedList() : __Head(nullptr), __Tail(nullptr), __Size(0) { }
    SingleLinkedList(const SingleLinkedList<T> &sll);
    virtual ~SingleLinkedList() { clear(); }

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

    void pop_back();
    void pop_front();

    inline void erase(iterator it)
    { __erase(it); }
    inline void insert(iterator &it, ConstReference arg)
    { __insert(it, arg); }
    inline void insert(iterator && it, ConstReference arg)
    { __insert(std::forward<iterator>(it), arg); }
    inline void insert(iterator &it, RvalueReference arg)
    { __insert(it, std::forward<Type>(arg)); }
    inline void insert(iterator && it, RvalueReference arg)
    { __insert(std::forward<iterator>(it), std::forward<Type>(arg)); }

    void clear();

    inline iterator begin() { return iterator(nullptr, __Head, __Head == nullptr ? nullptr : __Head->Next); }
    inline iterator end() { return iterator(__Tail, nullptr, nullptr); }

    Type front();
    Type back();

    iterator find(ConstReference arg)
    { return __find(arg); }
    iterator find(RvalueReference arg)
    { return __find(std::forward<Type>(arg)); }

};

template<typename T>
using Slist = SingleLinkedList<T>;

#ifndef NDEBUG
void test_SingleLinkedList_main();
#endif

};
#endif // SINGLELINKEDLIST_H
