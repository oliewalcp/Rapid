#include "Core/DoubleLinkedList.h"

template<typename T>
void rapid::DoubleLinkedList<T>::__create_list(ConstReference arg)
{
    if(__Head == nullptr)
    { __initialize(__construct_node(arg)); }
}

template<typename T>
void rapid::DoubleLinkedList<T>::__create_list(Node *n)
{
    if(__Head == nullptr)
    { __initialize(n); }
}

template<typename T>
void rapid::DoubleLinkedList<T>::__insert(const iterator &it, ConstReference arg)
{
    Node *n = __construct_node(arg);
    if(it.__Current == nullptr)
    {
        if(__Tail == nullptr)
        { __initialize(n); }
        else
        {
            __Tail->Next = n;
            n->Previous = __Tail;
            __Tail = n;
        }
    }
    else
    {
        n->Previous = it.__Current->Previous;
        n->Next = it.__Current;
        if(it.__Current->Previous != nullptr)
        { it.__Current->Previous->Next = n; }
        it.__Current->Previous = n;
        if(__Head->Previous != nullptr)
        { __Head = __Head->Previous; }
    }
    __add_size(1);
}

template<typename T>
void rapid::DoubleLinkedList<T>::__erase(const iterator &it)
{
    if(it == end()) return;
    if(it.__Current->Previous != nullptr)
    { it.__Current->Previous->Next = it.__Current->Next; }
    else
    { __Head = it.__Current->Next; }
    if(it.__Current->Next != nullptr)
    { it.__Current->Next->Previous = it.__Current->Previous; }
    if(it.__Current != nullptr)
    { delete it.__Current; }
    __add_size(-1);
}

template<typename T>
typename rapid::DoubleLinkedList<T>::iterator rapid::DoubleLinkedList<T>::__find(ConstReference arg)
{
    Node *temp = __Head;
    while(temp != nullptr)
    {
        const Type &data = temp->Data->const_ref_content();
        if(data == arg)
        { return iterator(temp); }
        temp = temp->Next;
    }
    return end();
}

template<typename T>
rapid::DoubleLinkedList<T>::DoubleLinkedList(const DoubleLinkedList<T> &dll) : __Head(nullptr), __Tail(nullptr), __Size(0)
{
    for(Node *n = dll.__Head; n != nullptr; n = n->Next)
    { push_back(n->Data->const_ref_content()); }
}

template<typename T>
void rapid::DoubleLinkedList<T>::clear()
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
typename rapid::DoubleLinkedList<T>::Type rapid::DoubleLinkedList<T>::front()
{
    if(__Head == nullptr)
    {
        return NodeBase<Type>().content();
    }
    return __Head->Data->content();
}

template<typename T>
typename rapid::DoubleLinkedList<T>::Type rapid::DoubleLinkedList<T>::back()
{
    if(__Tail == nullptr)
    {
        return NodeBase<Type>().content();
    }
    return __Tail->Data->content();
}

#ifndef NDEBUG
void rapid::test_DoubleLinkedList_main()
{
    std::cout << "************debug doubleLinkedList begin************" << std::endl;
    Dlist<int> s;
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
    std::cout << "total size1: " << s.size() << std::endl;
    std::cout << "---------------------" << std::endl;
    s.pop_back();
    std::cout << "total size2: " << s.size() << std::endl;
    auto it = s.begin();
    it++;
    s.erase(++it);
    std::cout << "total size3: " << s.size() << std::endl;
    s.pop_front();
    std::cout << "total size4: " << s.size() << std::endl;
    s.erase(s.find(20));
    std::cout << "total size5: " << s.size() << std::endl;
    s.erase(s.find(70));
    std::cout << "total size6: " << s.size() << std::endl;
    int i = 0;
    for(auto it = s.begin(); it != s.end(); it++, i++)
    {
        std::cout << *it << std::endl;
    }
    std::cout << "---------------------" << std::endl;
    rapid::Dlist<int> ss(s);
    for(auto it = ss.rbegin(); it != ss.rend(); it++)
    {
        std::cout << *it << std::endl;
    }
    std::cout << "************debug doubleLinkedList end************" << std::endl;
}
#endif
