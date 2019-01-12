#ifndef STACK_H
#define STACK_H
#include "C_Base.h"
#include "Exception.h"

template<typename T>
struct StackNode
{
    StackNode(_C_Base<T> *Data) : Next(nullptr), Data(Data) {}
    StackNode<T> *Next;
    _C_Base<T> *Data;
};

template<typename T>
class Stack
{
private:
    unsigned int __Number = 0;
    StackNode<T> *__TopNode = nullptr;

    inline void __push(const T & arg)
    {
        StackNode<T> *temp = new StackNode<T>((_C_Base<T> *)(&arg));
        temp->Next = __TopNode;
        __TopNode = temp;
        __Number++;
    }
public:
    ~Stack();
    inline unsigned int size(){return __Number;}//获取元素个数
    inline bool empty(){return __TopNode != nullptr ? false : true;}
    inline void pop()
    {
        if(__TopNode == nullptr)
            return;
        StackNode<T> *temp = __TopNode;
        __TopNode = __TopNode->Next;
        delete temp->Data;
        delete temp;
        __Number--;
    }
    inline void push(const T& arg)
    {
        __push(arg);
    }
    inline void push(T && arg)
    {
        __push(std::forward<T&&>(arg));
    }
    inline T& top()
    {
        if(__TopNode == nullptr)
            throw Exception("Stack is empty");
        return __TopNode->Data->Data;
    }
    void clear();
};

template<typename T>
Stack<T>::~Stack()
{
    clear();
}

template<typename T>
void Stack<T>::clear()
{
    while(size() > 0)
    {
        pop();
    }
    __Number = 0;
}
#endif // STACK_H
