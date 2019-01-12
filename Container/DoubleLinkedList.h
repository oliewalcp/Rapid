#ifndef DOUBLELINKEDLIST_H
#define DOUBLELINKEDLIST_H
#include "Exception.h"
#include "C_Base.h"

template<typename T>
struct DoubleNode
{
    DoubleNode(_C_Base<T> *arg) : Next(nullptr), Previous(nullptr), Value(arg) {}
    DoubleNode *Next;
    DoubleNode *Previous;
    _C_Base<T> *Value;
};

template<typename T>
class DoubleLinkedList
{
private:
    DoubleNode<T> *__Head = nullptr;
    DoubleNode<T> *__Tail = nullptr;
    unsigned int __ElementNumber;

    inline void __create_list(const T& arg)
    {
        __Head = __get_node(arg);
        __Tail = __Head;
    }
    inline DoubleNode<T>* __get_node(const T & arg)
    {
        return new DoubleNode<T>((_C_Base<T> *)(&arg));
    }
    inline void __add_tail(DoubleNode<T> *arg)
    {
        __Tail->Next = arg;
        arg->Previous = __Tail;
        __Tail = arg;
    }
    inline void __add_head(DoubleNode<T> *arg)
    {
        arg->Next = __Head;
        __Head->Previous = arg;
        __Head = arg;
    }
    inline void __add_middle(DoubleNode<T> *new_n, DoubleNode<T> *old_n)
    {
        new_n->Next = old_n->Next;
        old_n->Next->Previous = new_n;
        new_n->Previous = old_n;
        old_n->Next = new_n;
        __ElementNumber++;
    }
    inline void __remove(DoubleNode<T> *arg)
    {
        if(arg->Previous != nullptr) arg->Previous->Next = arg->Next;
        if(arg->Next != nullptr) arg->Next->Previous = arg->Previous;
        delete arg->Value;
        delete arg;
        __ElementNumber--;
    }
public:
    inline unsigned int size(){ return __ElementNumber; }
    inline bool empty(){ return __ElementNumber > 0 ? false : true; }
    void push_back(const T& arg);
    void push_back(T && arg);
    void push_front(const T& arg);
    void push_front(T && arg);
    inline void pop_back();
    inline void pop_front();
    void insert(const unsigned int index, const T& arg);
    void insert(const unsigned int index, T && arg);
    void remove(const unsigned int index);

    void reverse();
    void clear();
    T& operator[](const unsigned int index);
    ~DoubleLinkedList();

    class iterator
    {
    private:
        DoubleNode<T> *__Current = nullptr;

        friend class DoubleLinkedList;
    public:
        iterator(DoubleNode<T> *p = nullptr) : __Current(p){}
        T& operator*() const
        {
            return __Current->Value->Data;
        }
        iterator& operator++() {
            __Current = __Current->Next;
            return *this;
        }
        iterator operator++(int) {
            DoubleNode<T>* tmp = __Current;
            __Current = __Current->Next;
            return iterator(tmp);
        }
        T* operator->() const {
            return &__Current->Value->Data;
        }
        bool operator==(const iterator &arg) const {
            return arg.__Current == this->__Current;
        }

        bool operator!=(const iterator &arg) const {
            return arg.__Current != this->__Current;
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

    class reverse_iterator
    {
    private:
        DoubleNode<T> *__Current = nullptr;

        friend class DoubleLinkedList;
    public:
        reverse_iterator(DoubleNode<T> *p = nullptr) : __Current(p){}
        T operator*() const
        { return __Current->Value->Data; }
        reverse_iterator& operator++() {
            __Current = __Current->Previous;
            return *this;
        }
        reverse_iterator operator++(int) {
            DoubleNode<T>* temp = __Current;
            __Current = __Current->Previous;
            return reverse_iterator(temp);
        }
        T* operator->() const
        { return &__Current->Value->Data; }
        bool operator==(const reverse_iterator &arg) const
        { return arg.__Current == this->__Current; }

        bool operator!=(const reverse_iterator &arg) const
        { return arg.__Current != this->__Current; }
    };
    reverse_iterator rbegin() const
    { return reverse_iterator(__Tail); }
    reverse_iterator rend() const
    { return reverse_iterator(nullptr); }

    inline void erase(iterator it)
    { __remove(it.__Current); }
    inline void erase(reverse_iterator it)
    { __remove(it.__Current); }
    inline T &front()
    { return *iterator(__Head); }
    inline T &back()
    { return *iterator(__Tail); }
};

template<typename T>
DoubleLinkedList<T>::~DoubleLinkedList()
{ clear(); }

template<typename T>
void DoubleLinkedList<T>::push_back(const T &arg)
{
    if(__Tail == nullptr)  __create_list(arg);
    else __add_tail(__get_node(arg));
    __ElementNumber++;
}

template<typename T>
void DoubleLinkedList<T>::push_back(T &&arg)
{
    if(__Tail == nullptr) __create_list(std::forward<T&&>(arg));
    else __add_tail(__get_node(std::forward<T&&>(arg)));
    __ElementNumber++;
}

template<typename T>
void DoubleLinkedList<T>::push_front(const T &arg)
{
    if(__Head == nullptr) __create_list(arg);
    else __add_head(__get_node(arg));
    __ElementNumber++;
}

template<typename T>
void DoubleLinkedList<T>::push_front(T &&arg)
{
    if(__Head == nullptr) __create_list(std::forward<T&&>(arg));
    else __add_head(__get_node(std::forward<T&&>(arg)));
    __ElementNumber++;
}

template<typename T>
void DoubleLinkedList<T>::clear()
{
    while(size() > 0)
        pop_front();
    __ElementNumber = 0;
}

template<typename T>
inline void DoubleLinkedList<T>::pop_back()
{
    DoubleNode<T> *temp = __Tail;
    __Tail = __Tail->Previous;
    if(__ElementNumber == 1) __Head = nullptr;
    else __Tail->Next = nullptr;
    __remove(temp);
}

template<typename T>
inline void DoubleLinkedList<T>::pop_front()
{
    DoubleNode<T> *temp = __Head;
    __Head = __Head->Next;
    if(__ElementNumber == 1) __Tail = nullptr;
    else __Head->Previous = nullptr;
    __remove(temp);
}

template<typename T>
void DoubleLinkedList<T>::insert(const unsigned int index, const T &arg)
{
    if(__ElementNumber == 0) push_front(arg);
    else if(index >= __ElementNumber) push_back(arg);
    else
    {
        //find the former node of target node
        DoubleNode<T> *temp = __Head;
        unsigned int i = 0;
        while(i++ < index - 1)
            temp = temp->Next;
        //create node
        __add_middle(__get_node(arg), temp);
    }
}

template<typename T>
void DoubleLinkedList<T>::insert(const unsigned int index, T &&arg)
{
    if(__ElementNumber == 0) push_front(std::move(arg));
    else if(index >= __ElementNumber) push_back(std::move(arg));
    else
    {
        //find the former node of target node
        DoubleNode<T> *temp = __Head;
        unsigned int i = 0;
        while(i++ < index - 1)
            temp = temp->Next;
        //create node
        __add_middle(__get_node(std::move(arg)), temp);
    }
}

template<typename T>
T& DoubleLinkedList<T>::operator [](const unsigned int index)
{
    if(empty()) throw Exception("List is empty");
    if(index >= __ElementNumber) return __Tail->Value->Data;
    DoubleNode<T> *temp = __Head;
    unsigned int i = 0;
    while(i++ < index)
        temp = temp->Next;
    return temp->Value->Data;
}

template<typename T>
void DoubleLinkedList<T>::remove(const unsigned int index)
{
    if(__ElementNumber == 0) return;
    if(__ElementNumber == 1) pop_front();
    else if(index >= __ElementNumber) pop_back();
    else
    {
        DoubleNode<T> *temp = __Head;
        for(unsigned int i = 0; i < index; i++)
        {
            if(temp->Next == nullptr) break;
            temp = temp->Next;
        }
        __remove(temp);
    }
}
template<typename T>
void DoubleLinkedList<T>::reverse()
{
    if(__ElementNumber <= 0) return;
    DoubleNode<T> *node = __Head, *save = node->Next, *temp;
    while(save != nullptr)
    {
        temp = node;
        node = save;
        save = save->Next;
        node->Next = temp;
        temp->Previous = node;
    }
    node = __Head;
    __Head = __Tail;
    __Tail = node;
    __Tail->Next = nullptr;
}
#endif // DOUBLELINKEDLIST_H
