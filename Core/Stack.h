#ifndef STACK_H
#define STACK_H

#include "Memory.h"

namespace rapid
{
template<typename T>
class Stack
{
private:
    using Type = T;
    using Pointer = Type*;
    using Reference = Type&;
    using ConstReference = const Type &;
    using RvalueReference = Type&&;
    using SizeType = long;

    struct Node
    {
        Node *Next;
        NodeBase<Type> *Data;
        Node(ConstReference arg) : Next(nullptr), Data(new NodeBase<Type>(arg)) {}
        ~Node() { delete Data; }
    };

    SizeType __Size;
    Node *__Top;

    void __push(ConstReference arg);

    inline Node* __construct_node(ConstReference arg)
    { return new Node(arg); }

    inline void __add_size(SizeType arg)
    { __Size += arg; }
public:
    Stack() : __Size(0), __Top(nullptr) { }
    Stack(const Stack &arg);

    ~Stack()
    { clear(); }

    inline SizeType size()
    { return __Size; }

    inline bool empty()
    { return size() == 0; }

    inline void push(ConstReference arg)
    { __push(arg); }

    inline void push(RvalueReference arg)
    { __push(std::forward<Type>(arg)); }

    inline Type top()
    { return __Top->Data->content(); }

    void pop();
    void clear();
};

#ifndef NDEBUG
void test_Stack_main();
#endif

};

#endif // STACK_H
