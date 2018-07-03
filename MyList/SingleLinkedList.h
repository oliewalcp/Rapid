#ifndef SINGLELINKEDLIST_H
#define SINGLELINKEDLIST_H
#include <exception>
#include <string>
template<typename T>
struct SingleNode
{
    SingleNode *next;
    T value;
};
//单链表
template<typename T>
class SingleLinkedList
{
private:
    SingleNode<T> *head = nullptr;//头结点
    SingleNode<T> *tail = nullptr;//尾结点
    unsigned int ElementNumber = 0;//元素个数

    inline void create_list();//创建链表
public:
    inline unsigned int size();//获取链表的长度
    inline bool empty(){return number > 0 ? false : true;}
    inline void push_back(T& arg);//从后面插入元素
    inline void push_front(T& arg);//从前面插入元素
    void pop_back();//删除最后一个元素
    void pop_front();//删除最前一个元素
    void insert(unsigned int index, T& arg);//在某一个位置插入元素
    void remove(T &arg);//移除某一个元素
    void remove(unsigned int index);//移除索引号为index的元素
    void clear();//清空链表
    inline T &front();//获取头结点的值
    inline T &back();//获取尾节点的值
    T& operator[](unsigned int index);//获取某个位置的元素
    ~SingleLinkedList();

    //迭代器
    class iterator
    {
    private:
        SingleNode<T> *current = nullptr;
    public:
        iterator(SingleNode<T> *p = nullptr) : current(p){}
        T operator*() const
        {
            return current->value;
        }
        iterator& operator++() {
            current = current->next;
            return *this;
        }
        iterator operator++(int) {
            SingleNode<T>* tmp = current;
            current = current->next;
            return iterator(tmp);
        }
        SingleNode<T>* operator->() const {
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
inline unsigned int SingleLinkedList<T>::size()
{
    return ElementNumber;
}
template<typename T>
inline T& SingleLinkedList<T>::front()
{
    if(head == nullptr) return 0;
    return head->value;
}
template<typename T>
inline T& SingleLinkedList<T>::back()
{
    if(tail == nullptr) return 0;
    return tail->value;
}
template<typename T>
SingleLinkedList<T>::~SingleLinkedList()
{
    clear();
}

template<typename T>
inline void SingleLinkedList<T>::push_back(T &arg)
{
    if(head == nullptr)
    {
        create_list();
        tail->value = arg;
    }
    else
    {
        SingleNode<T> *rear = new SingleNode<T>;
        rear->value = arg;
        tail->next = rear;
        tail = rear;
    }
    ElementNumber++;
}

template<typename T>
inline void SingleLinkedList<T>::push_front(T &arg)
{
    if(head == nullptr)
    {
        create_list();
        head->value = arg;
    }
    else
    {
        SingleNode<T> *temp = new SingleNode<T>;
        temp->value = arg;
        temp->next = head;
        head = temp;
    }
    ElementNumber++;
}

template<typename T>
void SingleLinkedList<T>::clear()
{
    for(SingleNode<T>* point = head; point != nullptr;)
    {
        SingleNode<T>* temp = point->next;
        delete point;
        point = temp;
    }
    ElementNumber = 0;
}

template<typename T>
inline void SingleLinkedList<T>::create_list()
{
    head = new SingleNode<T>;
    head->next = nullptr;
    tail = head;
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
void SingleLinkedList<T>::insert(unsigned int index, T &arg)
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
        SingleNode<T> *n = new SingleNode<T>;
        n->value = arg;
        n->next = temp->next;
        temp->next = n;
        ElementNumber++;
    }
}

template<typename T>
T& SingleLinkedList<T>::operator [](unsigned int index)
{
    if(ElementNumber == 0) return std::exception("EmptyException");
    if(index >= ElementNumber) return tail->value;
    SingleNode<T> *temp = head;
    unsigned int i = 0;
    while(i++ < index)
        temp = temp->next;
    return temp->value;
}

template<typename T>
void SingleLinkedList<T>::remove(T &arg)
{
    for(SingleNode<T> *temp = head; temp != nullptr; temp = temp->next)
    {
        if(temp->value == arg)
        {
            SingleNode<T> *noNeed = temp->next;
            memcpy(temp, noNeed, sizeof(SingleNode<T>));
            delete noNeed;
            break;
        }
    }
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
        delete noNeed;
    }
}
#endif // SINGLELINKEDLIST_H
