#include "Core/Stack.h"

template<typename T>
rapid::Stack<T>::Stack(const Stack &arg) : __Size(0), __Top(nullptr)
{
    Node *n = arg.__Top;
    while(n != nullptr)
    {
        push(n->Data->const_ref_content());
        n = n->Next;
    }
}

template<typename T>
void rapid::Stack<T>::pop()
{
    if(empty()) return;
    Node *n = __Top;
    __Top = __Top->Next;
    delete n;
    __add_size(-1);
}

template<typename T>
void rapid::Stack<T>::clear()
{
    Node *n = __Top;
    while(n != nullptr)
    {
        Node *temp = n;
        n = n->Next;
        delete temp;
    }
    __Size = 0;
}

template<typename T>
void rapid::Stack<T>::__push(ConstReference arg)
{
    Node *n = __construct_node(arg);
    n->Next = __Top;
    __Top = n;
    __add_size(1);
}

#ifndef NDEBUG
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
#endif
