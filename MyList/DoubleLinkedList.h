#ifndef DOUBLELINKEDLIST_H
#define DOUBLELINKEDLIST_H
#include <exception>
#include <string>
template<typename T>
struct DoubleNode
{
    DoubleNode(){next = nullptr; previous = nullptr;}
    DoubleNode *next;
    DoubleNode *previous;
    T value;
};

template<typename T>
class DoubleLinkedList
{
private:
    DoubleNode<T> *head = nullptr;//头结点
    DoubleNode<T> *tail = nullptr;//尾结点
    unsigned int ElementNumber = 0;//元素个数

    inline void create_list();//创建链表
public:
    inline unsigned int size();//获取链表的长度
    inline void push_back(T& arg);//从后面插入元素
    inline void push_front(T& arg);//从前面插入元素
    inline void pop_back();//删除最后一个元素
    inline void pop_front();//删除最前一个元素
    void insert(unsigned int index, T& arg);//在某一个位置插入元素
    void remove(T &arg);//移除某一个元素
    void remove(unsigned int index);//移除索引号为index的元素
    void clear();//清空链表
    inline T &front();//获取头结点的值
    inline T &back();//获取尾节点的值
    T& operator[](unsigned int index);//获取某个位置的元素
    ~DoubleLinkedList();

    //迭代器
    class iterator
    {
    private:
        DoubleNode<T> *current = nullptr;
    public:
        iterator(DoubleNode<T> *p = nullptr) : current(p){}
        T operator*() const
        {
            return current->value;
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
        DoubleNode<T>* operator->() const {
            return current;
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
};

template<typename T>
inline unsigned int DoubleLinkedList<T>::size()
{
    return ElementNumber;
}
template<typename T>
inline T& DoubleLinkedList<T>::front()
{
    if(head == nullptr) return 0;
    return head->value;
}
template<typename T>
inline T& DoubleLinkedList<T>::back()
{
    if(tail == nullptr) return 0;
    return tail->value;
}
template<typename T>
DoubleLinkedList<T>::~DoubleLinkedList()
{
    clear();
}

template<typename T>
inline void DoubleLinkedList<T>::push_back(T &arg)
{
    if(tail == nullptr)
    {
        create_list();
        tail->value = arg;
    }
    else
    {
        DoubleNode<T> *rear = new DoubleNode<T>;
        rear->value = arg;
        rear->previous = tail;
        tail->next = rear;
        tail = rear;
    }
    ElementNumber++;
}

template<typename T>
inline void DoubleLinkedList<T>::push_front(T &arg)
{
    if(head == nullptr)
    {
        create_list();
        head->value = arg;
    }
    else
    {
        DoubleNode<T> *temp = new DoubleNode<T>;
        temp->value = arg;
        temp->next = head;
        head->previous = temp;
        head = temp;
    }
    ElementNumber++;
}

template<typename T>
void DoubleLinkedList<T>::clear()
{
    for(DoubleNode<T>* point = head; point != nullptr;)
    {
        DoubleNode<T>* temp = point->next;
        delete point;
        point = temp;
    }
    ElementNumber = 0;
}

template<typename T>
inline void DoubleLinkedList<T>::create_list()
{
    head = new DoubleNode<T>;
    tail = head;
}

template<typename T>
inline void DoubleLinkedList<T>::pop_back()
{
    DoubleNode<T> *temp = tail;
    tail = tail->previous;
    if(ElementNumber == 1) head = nullptr;
    else tail->next = nullptr;
    delete temp;
    ElementNumber--;
}

template<typename T>
inline void DoubleLinkedList<T>::pop_front()
{
    DoubleNode<T> *temp = head;
    head = head->next;
    if(ElementNumber == 1) tail = nullptr;
    else head->previous = nullptr;
    delete temp;
    ElementNumber--;
}

template<typename T>
void DoubleLinkedList<T>::insert(unsigned int index, T &arg)
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
        DoubleNode<T> *n = new DoubleNode<T>;
        n->value = arg;
        n->next = temp->next;
        temp->next->previous = n;
        n->previous = temp;
        temp->next = n;
        ElementNumber++;
    }
}

template<typename T>
T& DoubleLinkedList<T>::operator [](unsigned int index)
{
    if(ElementNumber == 0) return std::exception("EmptyException");
    if(index >= ElementNumber) return tail->value;
    DoubleNode<T> *temp = head;
    unsigned int i = 0;
    while(i++ < index)
        temp = temp->next;
    return temp->value;
}

template<typename T>
void DoubleLinkedList<T>::remove(T &arg)
{
    for(DoubleNode<T> *temp = head; temp != nullptr; temp = temp->next)
    {
        if(temp->value == arg)
        {
            temp->previous->next = temp->next;
            temp->next->previous = temp->previous;
            delete temp;
            break;
        }
    }
}

template<typename T>
void DoubleLinkedList<T>::remove(unsigned int index)
{
    if(ElementNumber == 0) return;
    else if(ElementNumber == 1) pop_front();
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
        temp->previous->next = temp->next;
        temp->next->previous = temp->previous;
        delete temp;
    }
}
#endif // DOUBLELINKEDLIST_H
