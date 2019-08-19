#include "Core/SingleLinkedList.h"

template<typename T>
rapid::SingleLinkedList<T>::SingleLinkedList(const SingleLinkedList<T> &sll)
    : _M_head(nullptr), _M_tail(nullptr), _M_size(0)
{
    for(Node *n = sll._M_head; n != nullptr; n = n->Next)
    { push_back(n->Data->const_ref_content()); }
}

template<typename T>
void rapid::SingleLinkedList<T>::_F_create_list(ConstReference arg)
{
    if(_M_head == nullptr)
    { _F_initialize(_F_construct_node(arg)); }
}

template<typename T>
void rapid::SingleLinkedList<T>::_F_create_list(Node *n)
{
    if(_M_head == nullptr)
    { _F_initialize(n); }
}

template<typename T>
void rapid::SingleLinkedList<T>::_F_insert(const iterator &it, ConstReference arg)
{
    Node *n = _F_construct_node(arg);
    if(it._M_current == nullptr && it._M_pervious == nullptr)
    { _F_initialize(n); }
    else
    {
        if(it._M_pervious == nullptr && it._M_current != nullptr)
        {
            n->Next = _M_head;
            _M_head = n;
        }
        else if(it._M_pervious != nullptr && it._M_current == nullptr)
        {
            _M_tail->Next = n;
            _M_tail = n;
        }
        else
        {
            it._M_pervious->Next = n;
            n->Next = it._M_current;
        }
    }
    _F_add_size(1);
}

template<typename T>
void rapid::SingleLinkedList<T>::_F_erase(iterator &it)
{
    if(it == end()) return;
    if(it._M_pervious != nullptr)
    { it._M_pervious->Next = it._M_next; }
    if(it._M_current != nullptr)
    {
        delete it._M_current;
        it._M_current = it._M_next;
    }
    _F_add_size(-1);
}

template<typename T>
void rapid::SingleLinkedList<T>::pop_back()
{
    if(_M_head == nullptr)
    {
        return;
    }
    if(_M_head->Next == nullptr)
    {
        delete _M_head->Next;
        _M_head = _M_tail = nullptr;
    }
    else
    {
        Node *n_before = _M_head, *n_after = _M_head->Next;
        while(n_after->Next != nullptr)
        {
            n_before = n_before->Next;
            n_after = n_after->Next;
        }
        n_before->Next = nullptr;
        delete n_after;
    }
    _F_add_size(-1);
}

template<typename T>
void rapid::SingleLinkedList<T>::pop_front()
{
    if(_M_head == nullptr) return;
    Node *n = _M_head;
    _M_head = _M_head->Next;
    delete n;
    _F_add_size(-1);
}

template<typename T>
void rapid::SingleLinkedList<T>::clear()
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
typename rapid::SingleLinkedList<T>::Type rapid::SingleLinkedList<T>::front()
{
    if(_M_head == nullptr)
    {
        return NodeBase<Type>().content();
    }
    return _M_head->Data->content();
}

template<typename T>
typename rapid::SingleLinkedList<T>::Type rapid::SingleLinkedList<T>::back()
{
    if(_M_tail == nullptr)
    {
        return NodeBase<Type>().content();
    }
    return _M_tail->Data->content();
}

template<typename T>
typename rapid::SingleLinkedList<T>::iterator rapid::SingleLinkedList<T>::_F_find(ConstReference arg)
{
    Node *temp = _M_head, *p = nullptr;
    while(temp != nullptr)
    {
        const Type &data = temp->Data->const_ref_content();
        if(data == arg)
        {
            return iterator(p, temp, temp->Next);
        }
        p = temp;
        temp = temp->Next;
    }
    return end();
}

#ifndef NDEBUG
void rapid::test_SingleLinkedList_main()
{
    std::cout << "************debug singleLinkedList begin************" << std::endl;
    Slist<int> s;
    s.push_back(10);
    s.push_back(20);
    s.push_back(30);
    s.push_back(40);
    s.push_back(50);
    s.push_back(60);
    s.push_back(70);
    s.push_front(0);
    for(int i : s)
    {
        std::cout << i << std::endl;
    }
    std::cout << "total size: " << s.size() << std::endl;
    std::cout << "---------------------" << std::endl;
    s.pop_back();
    auto it = s.begin();
    it++;
    s.erase(++it);
    s.erase(s.find(70));
    std::cout << "total size: " << s.size() << std::endl;
    s.pop_front();
    Slist<int> ss(s);
    for(int i : ss)
    {
        std::cout << i << std::endl;
    }
    std::cout << "total size: " << ss.size() << std::endl;
    std::cout << "************debug singleLinkedList end************" << std::endl;
}
#endif
