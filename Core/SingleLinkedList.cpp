#include "Core/SingleLinkedList.h"

template<typename T>
rapid::SingleLinkedList<T>::SingleLinkedList(const SingleLinkedList<T> &sll)
    : _M_head(nullptr), _M_size(0)
{
    iterator temp = before_begin();
    for(auto it = sll.begin(); it != sll.end(); ++it)
    {
        temp = insert_after(temp, *it);
    }
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
typename rapid::SingleLinkedList<T>::iterator rapid::SingleLinkedList<T>::_F_find(ConstReference arg)
{
   for(auto it = begin(); it != end(); ++it)
   {
       if((*it) == arg)
       { return it; }
   }
   return end();
}

template<typename T>
template<typename IteratorType>
typename rapid::SingleLinkedList<T>::iterator
    rapid::SingleLinkedList<T>::insert_after(const_iterator pos, IteratorType b, IteratorType e)
{
    for(IteratorType it = b; true; ++it)
    {
        iterator r = insert_after(pos, *it);
        if(b == e)
            return r;
    }
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
    std::cout << "************debug singleLinkedList end************" << std::endl;
}

