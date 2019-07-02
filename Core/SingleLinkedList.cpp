#include "Core/SingleLinkedList.h"

template<typename T>
rapid::SingleLinkedList<T>::SingleLinkedList(const SingleLinkedList<T> &sll)
    : __Head(nullptr), __Tail(nullptr), __Size(0)
{
    for(Node *n = sll.__Head; n != nullptr; n = n->Next)
    { push_back(n->Data->const_ref_content()); }
}

template<typename T>
void rapid::SingleLinkedList<T>::__create_list(ConstReference arg)
{
    if(__Head == nullptr)
    { __initialize(__construct_node(arg)); }
}

template<typename T>
void rapid::SingleLinkedList<T>::__create_list(Node *n)
{
    if(__Head == nullptr)
    { __initialize(n); }
}

template<typename T>
void rapid::SingleLinkedList<T>::__insert(const iterator &it, ConstReference arg)
{
    Node *n = __construct_node(arg);
    if(it.__Current == nullptr && it.__Pervious == nullptr)
    { __initialize(n); }
    else
    {
        if(it.__Pervious == nullptr && it.__Current != nullptr)
        {
            n->Next = __Head;
            __Head = n;
        }
        else if(it.__Pervious != nullptr && it.__Current == nullptr)
        {
            __Tail->Next = n;
            __Tail = n;
        }
        else
        {
            it.__Pervious->Next = n;
            n->Next = it.__Current;
        }
    }
    __add_size(1);
}

template<typename T>
void rapid::SingleLinkedList<T>::__erase(iterator &it)
{
    if(it == end()) return;
    if(it.__Pervious != nullptr)
    { it.__Pervious->Next = it.__Next; }
    if(it.__Current != nullptr)
    {
        delete it.__Current;
        it.__Current = it.__Next;
    }
    __add_size(-1);
}

template<typename T>
void rapid::SingleLinkedList<T>::pop_back()
{
    if(__Head == nullptr)
    {
        return;
    }
    if(__Head->Next == nullptr)
    {
        delete __Head->Next;
        __Head = __Tail = nullptr;
    }
    else
    {
        Node *n_before = __Head, *n_after = __Head->Next;
        while(n_after->Next != nullptr)
        {
            n_before = n_before->Next;
            n_after = n_after->Next;
        }
        n_before->Next = nullptr;
        delete n_after;
    }
    __add_size(-1);
}

template<typename T>
void rapid::SingleLinkedList<T>::pop_front()
{
    if(__Head == nullptr) return;
    Node *n = __Head;
    __Head = __Head->Next;
    delete n;
    __add_size(-1);
}

template<typename T>
void rapid::SingleLinkedList<T>::clear()
{
    Node *n = __Head;
    while(n != nullptr)
    {
        Node *temp = n;
        n = n->Next;
        delete temp;
    }
    __Size = 0;
}

template<typename T>
typename rapid::SingleLinkedList<T>::Type rapid::SingleLinkedList<T>::front()
{
    if(__Head == nullptr)
    {
        return NodeBase<Type>().content();
    }
    return __Head->Data->content();
}

template<typename T>
typename rapid::SingleLinkedList<T>::Type rapid::SingleLinkedList<T>::back()
{
    if(__Tail == nullptr)
    {
        return NodeBase<Type>().content();
    }
    return __Tail->Data->content();
}

template<typename T>
typename rapid::SingleLinkedList<T>::iterator rapid::SingleLinkedList<T>::__find(ConstReference arg)
{
    Node *temp = __Head, *p = nullptr;
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
