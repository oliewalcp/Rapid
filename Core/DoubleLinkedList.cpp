#include "Core/DoubleLinkedList.h"

template<typename T>
void rapid::DoubleLinkedList<T>::_F_create_list(ConstReference arg)
{
    if(_M_head == nullptr)
    { _F_initialize(_F_construct_node(arg)); }
}

template<typename T>
void rapid::DoubleLinkedList<T>::_F_create_list(Node *n)
{
    if(_M_head == nullptr)
    { _F_initialize(n); }
}

template<typename T>
void rapid::DoubleLinkedList<T>::_F_insert(const iterator &it, ConstReference arg)
{
    Node *n = _F_construct_node(arg);
    if(it._M_current == nullptr)
    {
        if(_M_tail == nullptr)
        { _F_initialize(n); }
        else
        {
            _M_tail->Next = n;
            n->Previous = _M_tail;
            _M_tail = n;
        }
    }
    else
    {
        n->Previous = it._M_current->Previous;
        n->Next = it._M_current;
        if(it._M_current->Previous != nullptr)
        { it._M_current->Previous->Next = n; }
        it._M_current->Previous = n;
        if(_M_head->Previous != nullptr)
        { _M_head = _M_head->Previous; }
    }
    _F_add_size(1);
}

template<typename T>
void rapid::DoubleLinkedList<T>::_F_erase(const iterator &it)
{
    if(it == end()) return;
    if(it._M_current->Previous != nullptr)
    { it._M_current->Previous->Next = it._M_current->Next; }
    else
    { _M_head = it._M_current->Next; }
    if(it._M_current->Next != nullptr)
    { it._M_current->Next->Previous = it._M_current->Previous; }
    if(it._M_current != nullptr)
    { delete it._M_current; }
    _F_add_size(-1);
}

template<typename T>
typename rapid::DoubleLinkedList<T>::iterator rapid::DoubleLinkedList<T>::_F_find(ConstReference arg)
{
    Node *temp = _M_head;
    while(temp != nullptr)
    {
        const ValueType &data = temp->Data->const_ref_content();
        if(data == arg)
        { return iterator(temp); }
        temp = temp->Next;
    }
    return end();
}

template<typename T>
rapid::DoubleLinkedList<T>::~DoubleLinkedList()
{
    clear();
}

template<typename T>
rapid::DoubleLinkedList<T>::DoubleLinkedList(const DoubleLinkedList<T> &dll) : _M_head(nullptr), _M_tail(nullptr), _M_size(0)
{
    for(Node *n = dll._M_head; n != nullptr; n = n->Next)
    { push_back(n->Data->const_ref_content()); }
}

template<typename T>
void rapid::DoubleLinkedList<T>::clear()
{
    Node *n = _M_head;
    while(n != nullptr)
    {
        Node *temp = n;
        n = n->Next;
        delete temp;
    }
    _M_size = 0;
}

template<typename T>
typename rapid::DoubleLinkedList<T>::ValueType rapid::DoubleLinkedList<T>::front()
{
    if(_M_head == nullptr)
    {
        return NodeBase<ValueType>().content();
    }
    return _M_head->Data->content();
}

template<typename T>
typename rapid::DoubleLinkedList<T>::ValueType rapid::DoubleLinkedList<T>::back()
{
    if(_M_tail == nullptr)
    {
        return NodeBase<ValueType>().content();
    }
    return _M_tail->Data->content();
}

#include <iostream>

void rapid::test_DoubleLinkedList_main()
{

}
