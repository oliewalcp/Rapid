#ifndef STACK_H
#define STACK_H

#include "Core/Memory.h"
#include "Core/TypeTraits.h"

namespace rapid
{
template<typename T>
class Stack
{
private:
    using ValueType = T;
    using Pointer = ValueType*;
    using Reference = ValueType&;
    using ConstReference = const ValueType &;
    using RvalueReference = ValueType&&;
    using SizeType = size_type;

    struct Node
    {
        Node *Next;
        NodeBase<ValueType> *Data;
        Node(ConstReference arg, Node *n = nullptr)
            : Next(n), Data(new NodeBase<ValueType>(arg)) {}
        ~Node() { delete Data; }
    };

    SizeType _M_size;
    Node *_M_top;

    void _F_push(ConstReference arg)
    {
        _M_top = _F_construct_node(arg, _M_top);
        _F_add_size(1);
    }
    void _F_exchange(const Stack &s)
    {
        Node *temp = _M_top;
        SizeType st = _M_size;
        _M_top = s._M_top;
        _M_size = s._M_size;
        s._M_top = temp;
        s._M_size = st;
    }
    inline Node* _F_construct_node(ConstReference arg, Node *next = nullptr)
    { return new Node(arg, next); }

    inline void _F_add_size(SizeType arg)
    { _M_size += arg; }
public:
    Stack() : _M_size(0), _M_top(nullptr) { }
    Stack(const Stack &arg);

    ~Stack()
    { clear(); }

    inline SizeType size()
    { return _M_size; }

    inline bool empty()
    { return size() == 0; }

    inline void push(ConstReference arg)
    { _F_push(arg); }

    inline void push(RvalueReference arg)
    { _F_push(forward<ValueType>(arg)); }

    inline ValueType top()
    { return _M_top != nullptr ? _M_top->Data->content() : NodeBase<ValueType>().content(); }

    void pop()
    {
        if(empty()) return;
        Node *n = _M_top;
        _M_top = _M_top->Next;
        delete n;
        _F_add_size(-1);
    }
    void clear()
    {
        while(!empty())
        { pop(); }
    }

    void swap(const Stack &s)
    { _F_exchange(s); }
    void swap(Stack &&s)
    { _F_exchange(forward<Stack>(s)); }
};

void test_Stack_main();
};

#endif // STACK_H
