#include "TestDoubleLinkedList.h"
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
