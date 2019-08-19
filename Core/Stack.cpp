#include "Core/Stack.h"

template<typename T>
rapid::Stack<T>::Stack(const Stack &arg) : _M_size(0), _M_top(nullptr)
{
    Node *n = arg._M_top;
    while(n != nullptr)
    {
        push(n->Data->const_ref_content());
        n = n->Next;
    }
}

template<typename T>
void rapid::Stack<T>::pop()
{
    if(empty()) return;
    Node *n = _M_top;
    _M_top = _M_top->Next;
    delete n;
    _F_add_size(-1);
}

template<typename T>
void rapid::Stack<T>::clear()
{
    Node *n = _M_top;
    while(n != nullptr)
    {
        Node *temp = n;
        n = n->Next;
        delete temp;
    }
    _M_size = 0;
}

template<typename T>
void rapid::Stack<T>::_F_push(ConstReference arg)
{
    Node *n = _F_construct_node(arg);
    n->Next = _M_top;
    _M_top = n;
    _F_add_size(1);
}
