#include "Core/DoubleLinkedList.h"

template<typename T>
template<typename ... Args>
typename rapid::DoubleLinkedList<T>::iterator
    rapid::DoubleLinkedList<T>::_F_insert(const_iterator it, const Args & ... args)
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
typename rapid::DoubleLinkedList<T>::iterator
    rapid::DoubleLinkedList<T>::insert(const_iterator pos, IteratorType b, IteratorType e)
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
void rapid::DoubleLinkedList<T>::_F_erase(const_iterator it)
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
typename rapid::DoubleLinkedList<T>::iterator
    rapid::DoubleLinkedList<T>::_F_find(ConstReference arg)
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
void rapid::DoubleLinkedList<T>::clear()
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
void rapid::DoubleLinkedList<T>::reverse()
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

#include <iostream>

void rapid::test_DoubleLinkedList_main()
{
    std::cout << "---------" << __func__ << "---------" << std::endl;
    DoubleLinkedList<int> dll1;
    auto it = dll1.push_back(10);
    dll1.push_back(20);
    dll1.push_back(30);
    dll1.push_front(100);
    dll1.push_front(200);
    it = dll1.insert(it, 0);
    dll1.insert(it, -10);
    it = dll1.insert(it, -20);
    std::cout << "size = " << dll1.size() << std::endl;
    for(int i : dll1)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    std::cout << "--------------------------------" << std::endl;
    dll1.erase(it);
    DoubleLinkedList<int> dll2(dll1);
    dll2.pop_back();
    dll2.pop_front();
    std::cout << "size = " << dll2.size() << std::endl;
    for(auto it = dll2.begin(); it != dll2.end(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    std::cout << "----------------reverse----------------" << std::endl;
    std::cout << "original data: " << std::endl;
    for(int i : dll1)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    std::cout << "reverse data: " << std::endl;
    dll1.reverse();
    for(int i : dll1)
    {
        std::cout << i << " ";
    }
}
