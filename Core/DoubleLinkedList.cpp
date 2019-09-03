#include "Core/DoubleLinkedList.h"

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
    for(auto it = dll2.begin(); it != dll2.end(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}
