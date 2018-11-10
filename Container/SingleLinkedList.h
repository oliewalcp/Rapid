#ifndef SINGLELINKEDLIST_H
#define SINGLELINKEDLIST_H
#include "C_Base.h"
#include "../Exception.h"

template<typename T>
struct SingleNode
{
    SingleNode(_C_Base<T> *arg) : Value(arg), Next(nullptr) {}
    SingleNode *Next;
    _C_Base<T> *Value;
};
//单链表
template<typename T>
class SingleLinkedList
{
private:
    SingleNode<T> *__Head = nullptr;//头结点
    SingleNode<T> *__Tail = nullptr;//尾结点
    unsigned int __ElementNumber = 0;//元素个数

    inline void __create_list(const T& arg)//创建链表
    {
        __Head = __get_node(arg);
        __Tail = __Head;
    }
    inline SingleNode<T>* __get_node(const T & arg)
    {
        return new SingleNode<T>((_C_Base<T> *)(&arg));
    }
    inline void __add_tail(SingleNode<T> *arg)
    {
        __Tail->Next = arg;
        __Tail = arg;
    }
    inline void __add_head(SingleNode<T> *arg)
    {
        arg->Next = __Head;
        __Head = arg;
    }
    inline void __add_middle(SingleNode<T> *new_n, SingleNode<T> *old_n)
    {
        new_n->Next = old_n->Next;
        old_n->Next = new_n;
        __ElementNumber++;
    }
public:
    inline unsigned int size();//获取链表的长度
    inline bool empty(){return __ElementNumber > 0 ? false : true;}
    void push_back(const T& arg);//从后面插入元素
    void push_back(T && arg);
    void push_front(const T &arg);//从前面插入元素
    void push_front(T && arg);
    void pop_back();//删除最后一个元素
    void pop_front();//删除最前一个元素
    void insert(unsigned int index, const T& arg);//在某一个位置插入元素
    void insert(unsigned int index, T && arg);//在某一个位置插入元素
    void remove(unsigned int index);//移除索引号为index的元素
    void reverse();//反转
    void clear();//清空链表
    T& operator[](unsigned int index);//获取某个位置的元素
    ~SingleLinkedList();

    //迭代器
    class iterator
    {
    private:
        SingleNode<T> *current = nullptr;
        SingleNode<T> *last = nullptr;
    public:
        iterator(SingleNode<T> *p = nullptr) : current(p){}
        T& operator*() const
        {
            return current->Value->Data;
        }
        iterator& operator++() {
            last = current;
            current = current->Next;
            return *this;
        }
        iterator operator++(int) {
            last = current;
            current = current->Next;
            return iterator(last);
        }
        T* operator->() const {
            return &current->Value;
        }
        bool operator==(const iterator &arg) const {
            return arg.current == this->current;
        }

        bool operator!=(const iterator &arg) const {
            return arg.current != this->current;
        }
    };

    iterator begin() const
    {
        return iterator(__Head);
    }
    iterator end() const
    {
        return iterator(nullptr);
    }
    inline T &front()//获取头结点的值
    {
        return *iterator(__Head);
    }
    inline T &back()//获取尾节点的值
    {
        return *iterator(__Tail);
    }
};

template<typename T>
inline unsigned int SingleLinkedList<T>::size()
{
    return __ElementNumber;
}
template<typename T>
SingleLinkedList<T>::~SingleLinkedList()
{
    clear();
}

template<typename T>
void SingleLinkedList<T>::push_back(const T &arg)
{
    if(__Head == nullptr)
    {
        __create_list(arg);
    }
    else
    {
        __add_tail(__get_node(arg));
    }
    __ElementNumber++;
}

template<typename T>
void SingleLinkedList<T>::push_back(T &&arg)
{
    if(__Head == nullptr)
    {
        __create_list(std::forward<T&&>(arg));
    }
    else
    {
        __add_tail(__get_node(std::forward<T&&>(arg)));
    }
    __ElementNumber++;
}

template<typename T>
void SingleLinkedList<T>::push_front(const T &arg)
{
    if(__Head == nullptr)
    {
        __create_list(arg);
    }
    else
    {
        __add_head(__get_node(arg));
    }
    __ElementNumber++;
}

template<typename T>
void SingleLinkedList<T>::push_front(T &&arg)
{
    if(__Head == nullptr)
    {
        __create_list(std::forward<T&&>(arg));
    }
    else
    {
        __add_head(__get_node(std::forward<T&&>(arg)));
    }
    __ElementNumber++;
}

template<typename T>
void SingleLinkedList<T>::clear()
{
    while(size() > 0)
    {
        pop_front();
    }
    __ElementNumber = 0;
}
template<typename T>
void SingleLinkedList<T>::pop_back()
{
    if(__ElementNumber == 1) pop_front();
    else
    {
        SingleNode<T> *temp = __Head;
        while(temp->Next != __Tail)
            temp = temp->Next;
        delete __Tail->Value;
        delete __Tail;
        __Tail = temp;
        __Tail->Next = nullptr;
        __ElementNumber--;
    }
}

template<typename T>
void SingleLinkedList<T>::pop_front()
{
    if(__ElementNumber == 1) __Tail = nullptr;
    SingleNode<T> *temp = __Head;
    __Head = __Head->Next;
    delete temp;
    __ElementNumber--;
}

template<typename T>
void SingleLinkedList<T>::insert(unsigned int index, const T &arg)
{
    if(__ElementNumber == 0) push_front(arg);
    else if(index >= __ElementNumber) push_back(arg);
    else
    {
        //找到目标节点的前一个节点
        SingleNode<T> *temp = __Head;
        unsigned int i = 0;
        while(i++ < index - 1)
            temp = temp->Next;
        __add_middle(__get_node(arg), temp);
    }
}

template<typename T>
void SingleLinkedList<T>::insert(unsigned int index, T &&arg)
{
    if(__ElementNumber == 0) push_front(std::forward<T&&>(arg));
    else if(index >= __ElementNumber) push_back(std::forward<T&&>(arg));
    else
    {
        //找到目标节点的前一个节点
        SingleNode<T> *temp = __Head;
        unsigned int i = 0;
        while(i++ < index - 1)
            temp = temp->Next;
        //新建结点
        __add_middle(__get_node(std::forward<T&&>(arg)), temp);
    }
}

template<typename T>
T& SingleLinkedList<T>::operator [](unsigned int index)
{
    if(__ElementNumber == 0) return Exception("EmptyException");
    if(index >= __ElementNumber) return __Tail->Value->Data;
    SingleNode<T> *temp = __Head;
    unsigned int i = 0;
    while(i++ < index)
        temp = temp->Next;
    return temp->Value->Data;
}

template<typename T>
void SingleLinkedList<T>::remove(unsigned int index)
{
    if(__ElementNumber == 0) return;
    else if(__ElementNumber == 1) pop_front();
    else
    {
        //查找目标位置的前一个结点
        SingleNode<T> *temp = __Head;
        for(unsigned int i = 0; i < index - 1; i++)
        {
            if(temp->Next == nullptr) break;
            temp = temp->Next;
        }
        //将下一个节点的内容复制到当前结点
        SingleNode<T> *noNeed = temp->Next;
        memcpy(temp, noNeed, sizeof(SingleNode<T>));
        delete noNeed->Value;
        delete noNeed;
    }
}

template<typename T>
void SingleLinkedList<T>::reverse()
{
    if(__ElementNumber <= 0) return;
    SingleNode<T> *node = __Head, *save = node->Next, *temp;
    while(save != nullptr)
    {
        temp = node;
        node = save;
        save = save->Next;
        node->Next = temp;
    }
    node = __Head;
    __Head = __Tail;
    __Tail = node;
    __Tail->Next = nullptr;
}
#endif // SINGLELINKEDLIST_H
