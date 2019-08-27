#ifndef STACK_H
#define STACK_H

#include "Memory.h"
#include "TypeTraits.h"

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
        Node(ConstReference arg) : Next(nullptr), Data(new NodeBase<ValueType>(arg)) {}
        ~Node() { delete Data; }
    };

    SizeType _M_size;
    Node *_M_top;

    void _F_push(ConstReference arg);

    inline Node* _F_construct_node(ConstReference arg)
    { return new Node(arg); }

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
    { return _M_top->Data->content(); }

    void pop();
    void clear();
};

void test_Stack_main();
};

#endif // STACK_H
