#include "TestSingleLinkedList.h"
#include "Core/SingleLinkedList.h"
#include <iostream>

void rapid::test_SingleLinkedList_main()
{
    std::cout << "************debug singleLinkedList begin************" << std::endl;
    Slist<int> s;
    s.push_front(70);
    s.push_front(10);
    s.push_front(50);
    s.push_front(40);
    s.push_front(20);
    s.push_front(30);
    s.push_front(60);
    s.push_front(0);
    for(int i : s)
    {
        std::cout << i << std::endl;
    }
    std::cout << "total size: " << s.size() << std::endl;
    std::cout << "---------sort------------" << std::endl;
    s.sort();
    for(int i : s)
    {
        std::cout << i << std::endl;
    }
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

