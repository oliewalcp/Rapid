#ifndef STACK_H
#define STACK_H
#include "C_Base.h"
#include "../Exception.h"

template<typename T>
struct StackNode
{
    StackNode(_C_Base<T> *data) : next(nullptr), data(data) {}
    StackNode<T> *next;
    _C_Base<T> *data;
};

template<typename T>
class Stack
{
private:
    unsigned int number = 0;
    StackNode<T> *topNode = nullptr;

    inline void __push(T && arg)
    {
        StackNode<T> *temp = new StackNode<T>((_C_Base<T> *)(&arg));
        temp->next = topNode;
        topNode = temp;
    }
public:
    ~Stack();
    inline unsigned int size(){return number;}//获取元素个数
    inline bool empty(){return topNode != nullptr ? false : true;}
    inline void pop()
    {
        if(topNode == nullptr)
            return;
        StackNode<T> *temp = topNode;
        topNode = topNode->next;
        delete temp->data;
        delete temp;
        number--;
    }
    inline void push(const T& arg)
    {
        StackNode<T> *temp = new StackNode<T>((_C_Base<T> *)(&arg));
        temp->next = topNode;
        topNode = temp;
    }
    inline void push(T && arg)
    {
        __push(std::move(arg));
    }
    inline T& top()
    {
        if(topNode == nullptr)
            throw Exception("Stack is empty");
        return topNode->data->__data;
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
    while(topNode != nullptr)
    {
        StackNode<T> *temp = topNode;
        topNode = topNode->next;
        delete temp->data;
        delete temp;
    }
    number = 0;
}
#endif // STACK_H
