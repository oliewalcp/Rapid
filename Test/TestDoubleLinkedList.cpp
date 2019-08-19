#include "TestCase.h"
#include "Core/DoubleLinkedList.h"

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
