#ifndef DOUBLELINKEDLIST_H
#define DOUBLELINKEDLIST_H
#include "../Exception.h"
#include "C_Base.h"

template<typename T>
struct DoubleNode
{
    DoubleNode(_C_Base<T> *arg) : next(nullptr), previous(nullptr), value(arg) {}
    DoubleNode *next;
    DoubleNode *previous;
    _C_Base<T> *value;
};

template<typename T>
class DoubleLinkedList
{
private:
    DoubleNode<T> *head = nullptr;//头结点
    DoubleNode<T> *tail = nullptr;//尾结点
    unsigned int ElementNumber;//元素个数

    inline void __create_list(const T& arg)//创建链表
    {
        head = __get_node(arg);
        tail = head;
    }
    inline void __create_list(T && arg)//创建链表
    {
        head = __get_node(std::move(arg));
        tail = head;
    }
    inline DoubleNode<T>* __get_node(const T & arg)
    {
        return new DoubleNode<T>((_C_Base<T> *)(&arg));
    }
    inline DoubleNode<T>* __get_node(T && arg)
    {
        return new DoubleNode<T>((_C_Base<T> *)(&arg));
    }
    inline void __add_tail(DoubleNode<T> *arg)
    {
        tail->next = arg;
        arg->previous = tail;
        tail = arg;
    }
    inline void __add_head(DoubleNode<T> *arg)
    {
        arg->next = head;
        head->previous = arg;
        head = arg;
    }
    inline void __add_middle(DoubleNode<T> *new_n, DoubleNode<T> *old_n)
    {
        new_n->next = old_n->next;
        old_n->next->previous = new_n;
        new_n->previous = old_n;
        old_n->next = new_n;
        ElementNumber++;
    }
    inline void __remove(DoubleNode<T> *arg)
    {
        if(arg->previous != nullptr) arg->previous->next = arg->next;
        if(arg->next != nullptr) arg->next->previous = arg->previous;
        delete arg->value;
        delete arg;
        ElementNumber--;
    }
public:
    inline unsigned int size(){ return ElementNumber; }//获取链表的长度
    inline bool empty(){ return ElementNumber > 0 ? false : true; }
    void push_back(const T& arg);//从后面插入元素
    void push_back(T && arg);
    void push_front(const T& arg);//从前面插入元素
    void push_front(T && arg);
    inline void pop_back();//删除最后一个元素
    inline void pop_front();//删除最前一个元素
    void insert(const unsigned int index, const T& arg);//在某一个位置插入元素
    void insert(const unsigned int index, T && arg);//在某一个位置插入元素
    void remove(const unsigned int index);//移除索引号为index的元素

    void reverse();//反转
    void clear();//清空链表
    T& operator[](const unsigned int index);//获取某个位置的元素
    ~DoubleLinkedList();

    //迭代器
    class iterator
    {
    private:
        DoubleNode<T> *current = nullptr;

        friend class DoubleLinkedList;
    public:
        iterator(DoubleNode<T> *p = nullptr) : current(p){}
        T& operator*() const
        {
            return current->value->__data;
        }
        iterator& operator++() {
            current = current->next;
            return *this;
        }
        iterator operator++(int) {
            DoubleNode<T>* tmp = current;
            current = current->next;
            return iterator(tmp);
        }
        T* operator->() const {
            return &current->value->__data;
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
        return iterator(head);
    }
    iterator end() const
    {
        return iterator(nullptr);
    }

    //反向迭代器
    class reverse_iterator
    {
    private:
        DoubleNode<T> *current = nullptr;

        friend class DoubleLinkedList;
    public:
        reverse_iterator(DoubleNode<T> *p = nullptr) : current(p){}
        T operator*() const
        {
            return current->value->__data;
        }
        reverse_iterator& operator++() {
            current = current->previous;
            return *this;
        }
        reverse_iterator operator++(int) {
            DoubleNode<T>* temp = current;
            current = current->previous;
            return reverse_iterator(temp);
        }
        T* operator->() const {
            return &current->value->__data;
        }
        bool operator==(const reverse_iterator &arg) const {
            return arg.current == this->current;
        }

        bool operator!=(const reverse_iterator &arg) const {
            return arg.current != this->current;
        }
    };
    reverse_iterator rbegin() const
    {
        return reverse_iterator(tail);
    }
    reverse_iterator rend() const
    {
        return reverse_iterator(nullptr);
    }

    inline void erase(iterator it)
    {
        __remove(it.current);
    }
    inline void erase(reverse_iterator it)
    {
        __remove(it.current);
    }
    inline T &front()//获取头结点的值
    {
        return *iterator(head);
    }
    inline T &back()//获取尾节点的值
    {
        return *iterator(tail);
    }
};

template<typename T>
DoubleLinkedList<T>::~DoubleLinkedList()
{
    clear();
}

template<typename T>
void DoubleLinkedList<T>::push_back(const T &arg)
{
    if(tail == nullptr)
        __create_list(arg);
    else
    {
        __add_tail(__get_node(arg));
    }
    ElementNumber++;
}

template<typename T>
void DoubleLinkedList<T>::push_back(T &&arg)
{
    if(tail == nullptr)
        __create_list(std::move(arg));
    else
    {
        __add_tail(__get_node(std::move(arg)));
    }
    ElementNumber++;
}

template<typename T>
void DoubleLinkedList<T>::push_front(const T &arg)
{
    if(head == nullptr)
        __create_list(arg);
    else
    {
        __add_head(__get_node(arg));
    }
    ElementNumber++;
}

template<typename T>
void DoubleLinkedList<T>::push_front(T &&arg)
{
    if(head == nullptr)
        __create_list(arg);
    else
    {
        __add_head(__get_node(std::move(arg)));
    }
    ElementNumber++;
}

template<typename T>
void DoubleLinkedList<T>::clear()
{
    while(size() > 0)
    {
        DoubleNode<T>* temp = head;
        delete temp->value;
        pop_front();
    }
    ElementNumber = 0;
}

template<typename T>
inline void DoubleLinkedList<T>::pop_back()
{
    DoubleNode<T> *temp = tail;
    tail = tail->previous;
    if(ElementNumber == 1) head = nullptr;
    else tail->next = nullptr;
    __remove(temp);
}

template<typename T>
inline void DoubleLinkedList<T>::pop_front()
{
    DoubleNode<T> *temp = head;
    head = head->next;
    if(ElementNumber == 1) tail = nullptr;
    else head->previous = nullptr;
    __remove(temp);
}

template<typename T>
void DoubleLinkedList<T>::insert(const unsigned int index, const T &arg)
{
    if(ElementNumber == 0) push_front(arg);
    else if(index >= ElementNumber) push_back(arg);
    else
    {
        //找到目标节点的前一个节点
        DoubleNode<T> *temp = head;
        unsigned int i = 0;
        while(i++ < index - 1)
            temp = temp->next;
        //新建结点
        __add_middle(__get_node(arg), temp);
    }
}

template<typename T>
void DoubleLinkedList<T>::insert(const unsigned int index, T &&arg)
{
    if(ElementNumber == 0) push_front(std::move(arg));
    else if(index >= ElementNumber) push_back(std::move(arg));
    else
    {
        //找到目标节点的前一个节点
        DoubleNode<T> *temp = head;
        unsigned int i = 0;
        while(i++ < index - 1)
            temp = temp->next;
        //新建结点
        __add_middle(__get_node(std::move(arg)), temp);
    }
}

template<typename T>
T& DoubleLinkedList<T>::operator [](const unsigned int index)
{
    if(empty()) throw Exception("List is empty");
    if(index >= ElementNumber) return tail->value->__data;
    DoubleNode<T> *temp = head;
    unsigned int i = 0;
    while(i++ < index)
        temp = temp->next;
    return temp->value->__data;
}

template<typename T>
void DoubleLinkedList<T>::remove(const unsigned int index)
{
    if(ElementNumber == 0) return;
    if(ElementNumber == 1) pop_front();
    else if(index >= ElementNumber) pop_back();
    else
    {
        //查找目标位置结点
        DoubleNode<T> *temp = head;
        for(unsigned int i = 0; i < index; i++)
        {
            if(temp->next == nullptr) break;
            temp = temp->next;
        }
        __remove(temp);
    }
}
template<typename T>
void DoubleLinkedList<T>::reverse()
{
    if(ElementNumber <= 0) return;
    DoubleNode<T> *node = head, *save = node->next, *temp;
    while(save != nullptr)
    {
        temp = node;
        node = save;
        save = save->next;
        node->next = temp;
        temp->previous = node;
    }
    node = head;
    head = tail;
    tail = node;
    tail->next = nullptr;
}
#endif // DOUBLELINKEDLIST_H
