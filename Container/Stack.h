#ifndef STACK_H
#define STACK_H

template<typename T>
struct StackNode
{
    StackNode() {next = nullptr;}
    StackNode<T> *next;
    T arg;
};

template<typename T>
class Stack
{
private:
    unsigned int number = 0;
    StackNode<T> *topNode = nullptr;
public:
    unsigned int size(){return number;}//获取元素个数
    inline bool empty(){return topNode != nullptr ? false : true;}
    void pop()
    {
        if(topNode == nullptr) return;
        StackNode<T> *temp = topNode;
        topNode = topNode->next;
        delete temp;
        number--;
    }
    void push(const T& arg)
    {
        StackNode<T> *temp = new StackNode<T>;
        temp->arg = arg;
        temp->next = topNode;
        topNode = temp;
        number++;
    }
    T top()
    {
        if(topNode == nullptr)
            return T();
        return topNode->arg;
    }
    void clear();
};

template<typename T>
void Stack<T>::clear()
{
    while(topNode != nullptr)
    {
        StackNode<T> *temp = topNode;
        topNode = topNode->next;
        delete temp;
    }
    number = 0;
}
#endif // STACK_H
