#ifndef SINGLELINKEDLIST_CPP
#define SINGLELINKEDLIST_CPP

#include "SingleLinkedList.h"
#include "Core/Stack.h"

template<typename T>
void rapid::SingleLinkedList<T>::clear()
{
    Node *n = _M_head->Next;
    while(n != nullptr)
    {
        Node *temp = n;
        n = n->Next;
        delete temp;
    }
    _M_size = 0;
}

template<typename T>
void rapid::SingleLinkedList<T>::reverse()
{
    Stack<Node *> s;
    for(Node *n = _M_head->Next; n != nullptr; n = n->Next)
    {
        s.push(n);
    }
    _M_head->Next = s.top();
    while(!s.empty())
    {
        Node *top_node = s.top();
        top_node->Next = nullptr;
        s.pop();
        if(s.empty()) break;
        Node *top_under_node = s.top();
        top_node->Next = top_under_node;
    }
}

template<typename T>
typename rapid::SingleLinkedList<T>::iterator
    rapid::SingleLinkedList<T>::_F_find(ConstReference arg) const
{
   for(const_iterator it = begin(); it != end(); ++it)
   {
       if((*it) == arg)
       { return it._F_const_cast(); }
   }
   return cend()._F_const_cast();
}

template<typename T>
template<typename IteratorType>
typename rapid::SingleLinkedList<T>::iterator
    rapid::SingleLinkedList<T>::insert_after(const_iterator pos, IteratorType b, IteratorType e)
{
    iterator r;
    while(b != e)
    {
        r = insert_after(pos, *b);
        ++b;
    }
    return r;
}

#include <iostream>
void rapid::test_SingleLinkedList_main()
{
    std::cout << "************debug singleLinkedList begin************" << std::endl;
    Slist<int> s;
    s.push_front(10);
    s.push_front(20);
    s.push_front(30);
    s.push_front(40);
    s.push_front(50);
    s.push_front(60);
    s.push_front(70);
    s.push_front(0);
    for(int i : s)
    {
        std::cout << i << std::endl;
    }
    std::cout << "total size: " << s.size() << std::endl;
    std::cout << "---------------------" << std::endl;
    s.pop_front();
    auto it = s.begin();
    it++;
    s.erase_after(++it);
    s.erase_after(s.find(70));
    std::cout << "total size: " << s.size() << std::endl;
    s.pop_front();
    Slist<int> ss(s);
    for(int i : ss)
    {
        std::cout << i << std::endl;
    }
    std::cout << "total size: " << ss.size() << std::endl;
    std::cout << "---------reverse------------" << std::endl;
    ss.reverse();
    for(int i : ss)
    {
        std::cout << i << std::endl;
    }
    std::cout << "************debug singleLinkedList end************" << std::endl;
}

#endif // end SINGLELINKEDLIST_CPP
