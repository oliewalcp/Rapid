#ifndef SINGLELINKEDLIST_H
#define SINGLELINKEDLIST_H
#include "C_Base.h"

template<typename T>
struct SingleNode
{
    SingleNode(_C_Base<T> *arg) : value(arg), next(nullptr) {}
    SingleNode *next;
    _C_Base<T> *value;
};
//单链表
template<typename T>
class SingleLinkedList
{
private:
    SingleNode<T> *head = nullptr;//头结点
    SingleNode<T> *tail = nullptr;//尾结点
    unsigned int ElementNumber = 0;//元素个数

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
    inline SingleNode<T>* __get_node(const T & arg)
    {
        return new SingleNode<T>((_C_Base<T> *)(&arg));
    }
    inline SingleNode<T>* __get_node(T && arg)
    {
        return new SingleNode<T>((_C_Base<T> *)(&arg));
    }
    inline void __add_tail(SingleNode<T> *arg)
    {
        tail->next = arg;
        tail = arg;
    }
    inline void __add_head(SingleNode<T> *arg)
    {
        arg->next = head;
        head = arg;
    }
    inline void __add_middle(SingleNode<T> *new_n, SingleNode<T> *old_n)
    {
        new_n->next = old_n->next;
        old_n->next = new_n;
        ElementNumber++;
    }
public:
    inline unsigned int size();//获取链表的长度
    inline bool empty(){return ElementNumber > 0 ? false : true;}
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
        T operator*() const
        {
            return current->value;
        }
        iterator& operator++() {
            last = current;
            current = current->next;
            return *this;
        }
        iterator operator++(int) {
            last = current;
            current = current->next;
            return iterator(last);
        }
        T* operator->() const {
            return &current->value;
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
inline unsigned int SingleLinkedList<T>::size()
{
    return ElementNumber;
}
template<typename T>
SingleLinkedList<T>::~SingleLinkedList()
{
    clear();
}

template<typename T>
void SingleLinkedList<T>::push_back(const T &arg)
{
    if(head == nullptr)
    {
        __create_list(arg);
    }
    else
    {
        __add_tail(__get_node(arg));
    }
    ElementNumber++;
}

template<typename T>
void SingleLinkedList::push_back(T &&arg)
{
    if(head == nullptr)
    {
        __create_list(arg);
    }
    else
    {
        __add_tail(__get_node(std::move(arg)));
    }
    ElementNumber++;
}

template<typename T>
void SingleLinkedList<T>::push_front(const T &arg)
{
    if(head == nullptr)
    {
        __create_list(arg);
    }
    else
    {
        __add_head(__get_node(arg));
    }
    ElementNumber++;
}

template<typename T>
void SingleLinkedList::push_front(T &&arg)
{
    if(head == nullptr)
    {
        __create_list(arg);
    }
    else
    {
        __add_head(__get_node(std::move(arg)));
    }
    ElementNumber++;
}

template<typename T>
void SingleLinkedList<T>::clear()
{
    for(SingleNode<T>* point = head; point != nullptr;)
    {
        SingleNode<T>* temp = point->next;
        delete point->value;
        delete point;
        point = temp;
    }
    ElementNumber = 0;
}
template<typename T>
void SingleLinkedList<T>::pop_back()
{
    if(ElementNumber == 1) pop_front();
    else
    {
        SingleNode<T> *temp = head;
        while(temp->next != tail)
            temp = temp->next;
        delete tail->value;
        delete tail;
        tail = temp;
        tail->next = nullptr;
        ElementNumber--;
    }
}

template<typename T>
void SingleLinkedList<T>::pop_front()
{
    if(ElementNumber == 1) tail = nullptr;
    SingleNode<T> *temp = head;
    head = head->next;
    delete temp;
    ElementNumber--;
}

template<typename T>
void SingleLinkedList<T>::insert(unsigned int index, const T &arg)
{
    if(ElementNumber == 0) push_front(arg);
    else if(index >= ElementNumber) push_back(arg);
    else
    {
        //找到目标节点的前一个节点
        SingleNode<T> *temp = head;
        unsigned int i = 0;
        while(i++ < index - 1)
            temp = temp->next;
        __add_middle(__get_node(arg), temp);
    }
}

template<typename T>
void SingleLinkedList::insert(unsigned int index, T &&arg)
{
    if(ElementNumber == 0) push_front(std::move(arg));
    else if(index >= ElementNumber) push_back(std::move(arg));
    else
    {
        //找到目标节点的前一个节点
        SingleNode<T> *temp = head;
        unsigned int i = 0;
        while(i++ < index - 1)
            temp = temp->next;
        //新建结点
        __add_middle(__get_node(std::move(arg)), temp);
    }
}

template<typename T>
T& SingleLinkedList<T>::operator [](unsigned int index)
{
    if(ElementNumber == 0) return std::exception("EmptyException");
    if(index >= ElementNumber) return tail->value->__data;
    SingleNode<T> *temp = head;
    unsigned int i = 0;
    while(i++ < index)
        temp = temp->next;
    return temp->value->__data;
}

template<typename T>
void SingleLinkedList<T>::remove(unsigned int index)
{
    if(ElementNumber == 0) return;
    else if(ElementNumber == 1) pop_front();
    else
    {
        //查找目标位置的前一个结点
        SingleNode<T> *temp = head;
        for(unsigned int i = 0; i < index - 1; i++)
        {
            if(temp->next == nullptr) break;
            temp = temp->next;
        }
        //将下一个节点的内容复制到当前结点
        SingleNode<T> *noNeed = temp->next;
        memcpy(temp, noNeed, sizeof(SingleNode<T>));
        delete noNeed->value;
        delete noNeed;
    }
}

template<typename T>
void SingleLinkedList<T>::reverse()
{
    if(ElementNumber <= 0) return;
    SingleNode<T> *node = head, *save = node->next, *temp;
    while(save != nullptr)
    {
        temp = node;
        node = save;
        save = save->next;
        node->next = temp;
    }
    node = head;
    head = tail;
    tail = node;
    tail->next = nullptr;
}
#endif // SINGLELINKEDLIST_H
