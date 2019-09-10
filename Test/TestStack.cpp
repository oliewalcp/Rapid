#include "TestStack.h"
#include <iostream>

void rapid::test_Stack_main()
{
    std::cout << "************debug Stack begin************" << std::endl;
    Stack<int> s;
    s.push(10);
    std::cout << "top value: " << s.top() << std::endl;
    s.push(40);
    std::cout << "top value: " << s.top() << std::endl;
    s.push(20);
    std::cout << "top value: " << s.top() << std::endl;
    s.push(30);
    std::cout << "top value: " << s.top() << std::endl;
    s.push(60);
    std::cout << "top value: " << s.top() << std::endl;
    s.push(50);
    std::cout << "top value: " << s.top() << std::endl;
    s.push(70);
    std::cout << "top value: " << s.top() << std::endl;
    s.push(0);
    std::cout << "top value: " << s.top() << std::endl;
    std::cout << "total size: " << s.size() << std::endl;
    std::cout << "---------------------" << std::endl;
    s.pop();
    std::cout << "top value: " << s.top() << std::endl;
    s.pop();
    std::cout << "top value: " << s.top() << std::endl;
    std::cout << "total size: " << s.size() << std::endl;
    Stack<int> ss(s);
    std::cout << "top value: " << ss.top() << std::endl;
    std::cout << "total size: " << ss.size() << std::endl;
    std::cout << "************debug Stack end************" << std::endl;
}
