#ifndef BINARYTREE_H
#define BINARYTREE_H
#include "C_Base.h"

template <typename Key, typename Value>
struct BinaryNode
{
private:
    typedef unsigned int __uint32;
public:
    BinaryNode(_C_Base<Key> *key = nullptr, _C_Base<Value> *value = nullptr) :
        First(key), Second(value), Left(nullptr), Right(nullptr), Parent(nullptr) {}
    BinaryNode<Key, Value> *Left;
    BinaryNode<Key, Value> *Right;
    BinaryNode<Key, Value> *Parent;
    _C_Base<Key> *First;
    _C_Base<Value> *Second;
    __uint32 LeftChildSize = 0;
    __uint32 RightChildSize = 0;
};

template<typename _Key, typename _Value, typename _Compare = Compare>
class BinarySortTree
{
private:
    typedef unsigned int __uint32;
protected:
    BinaryNode<_Key, _Value> *_Root;
    __uint32 _ElementNumber;

    virtual void __insert_node(const _Key &key, const _Value &value)
    {
        BinaryNode<_Key, _Value> *node = new BinaryNode<_Key, _Value>((_C_Base<_Key> *)&key, (_C_Base<_Value> *)&value);
        __insert_node(node);
    }
    virtual void __insert_node(BinaryNode<_Key, _Value> *node);
    virtual void __add_size(BinaryNode<_Key, _Value> *new_node, __uint32 size = 1);
    virtual BinaryNode<_Key, _Value> *__get_leftchild(BinaryNode<_Key, _Value> *node);
    virtual BinaryNode<_Key, _Value> * __get_rightchild(BinaryNode<_Key, _Value> *node);
public:
    virtual ~BinarySortTree() { clear(); }
    virtual void clear();

    virtual void size() { return _ElementNumber; }

    virtual void insert(const _Key& key, const _Value& value);
    virtual void insert(_Key && key, _Value && value);
    virtual void insert(_Key && key, const _Value& value);
    virtual void insert(const _Key& key, _Value && value);

    virtual void remove(const _Key& key);
    virtual void remove(_Key && key);

    virtual bool contains(const _Key& key);
    virtual bool contains(_Key && key);

    typedef class Former_iterator
    {
    private:
        BinaryNode<_Key, _Value> *__Current;
        BinaryNode<_Key, _Value> *__Next;

        void __get_next_node()
        {
            if(__Next == nullptr) __Current = nullptr;
            else
            {
                BinaryNode<_Key, _Value> *temp = __Next;
                //如果下一个节点是当前结点的左孩子
                if(__Next == __Current->Left)
                    __Next = __Next->Left != nullptr ? __Next->Left : __Current->Right;
                //如果下一个节点是当前结点的兄弟结点
                else if(__Next == __Current->Parent->Right)
                    __Next = __Current->Parent->Parent->Right;
                //如果下一个节点是当前结点的叔叔结点
                else if(__Next == __Current->Parent->Right)
                    __Next = __Next->Left != nullptr ? __Next->Left : __Current->Right;
                __Current = temp;
            }
        }
    public:
        Former_iterator(BinaryNode<_Key, _Value> *cur, BinaryNode<_Key, _Value> *nex) : __Current(cur), __Next(nex) {}
        Former_iterator(BinaryNode<_Key, _Value> *arg = nullptr) :
            __Current(arg), __Next(arg->Left == nullptr ? arg->Right : nullptr) {}
        Former_iterator& operator++() {
            __get_next_node();
            return *this;
        }
        Former_iterator& operator++(int) {
            Former_iterator it(__Current, __Next);
            __get_next_node();
            return it;
        }
        BinaryNode<_Key, _Value> * operator->() const {
            return &__Current;
        }
        bool operator==(const Former_iterator &arg) const {
            return arg.__Current == this->__Current;
        }
        bool operator!=(const Former_iterator &arg) const {
            return arg.__Current != this->__Current;
        }
    } fiterator;

    virtual fiterator fbegin()
    { return fiterator(_Root, _Root->Left == nullptr ? _Root->Right : _Root->Left); }
    virtual fiterator fend()
    { return fiterator(); }

    typedef class After_iterator
    {
    private:
        BinaryNode<_Key, _Value> *__Current;
        BinaryNode<_Key, _Value> *__Next;

        void __get_next_node()
        {
            if(__Next == nullptr) __Current = nullptr;
            else
            {
                BinaryNode<_Key, _Value> *temp = __Next;
                if(__Next == __Current->Parent)
                {
                    if(__Next->Parent != nullptr)
                    {
                        if(__Next->Parent->Right != nullptr) __Next = __get_leftchild(__Next->Parent->Right);
                        else __Next = __Next->Parent;
                    }
                    else __Next = nullptr;
                }
                else if(__Next == __Current->Parent->Right)
                {
                    __Next = __Current->Parent;
                }
                else
                {
                    if(__Next->Parent != nullptr)
                    {
                        if(__Next->Parent->Right != nullptr) __Next = __get_leftchild(__Next->Parent->Right);
                        else __Next = __Next->Parent;
                    }
                    else __Next = nullptr;
                }
                //如果下一个节点是当前结点的父孩子
                if(__Next == __Current->Parent)
                    __Next = __Next->Left != nullptr ? __Next->Left : __Current->Right;
                //如果下一个节点是当前结点的兄弟结点
                else if(__Next == __Current->Parent->Right)
                    __Next = __Current->Parent;
                //如果下一个节点是当前结点的叔叔结点
                else if(__Next == __Current->Parent->Right)
                    __Next = __Next->Left != nullptr ? __Next->Left : __Current->Right;
                __Current = temp;
            }
        }
    public:
        After_iterator(BinaryNode<_Key, _Value> *cur, BinaryNode<_Key, _Value> *nex) : __Current(cur), __Next(nex) {}
        After_iterator(BinaryNode<_Key, _Value> *arg = nullptr) :
            __Current(arg), __Next(arg->Left == nullptr ? arg->Right : nullptr) {}
        After_iterator& operator++() {
            __get_next_node();
            return *this;
        }
        After_iterator& operator++(int) {
            After_iterator it(__Current, __Next);
            __get_next_node();
            return it;
        }
        BinaryNode<_Key, _Value> * operator->() const {
            return &__Current;
        }
        bool operator==(const Former_iterator &arg) const {
            return arg.__Current == this->__Current;
        }
        bool operator!=(const Former_iterator &arg) const {
            return arg.__Current != this->__Current;
        }
    } aiterator;
    typedef aiterator iterator;
    virtual aiterator abegin()
    {
        BinaryNode<_Key, _Value> *temp = __get_leftchild(_Root);
        if(temp->Parent == nullptr) return aiterator(temp, nullptr);
        return aiterator(temp, temp->Parent->Right == nullptr ? temp->Parent : temp->Parent->Right);
    }
    virtual aiterator aend()
    { return aiterator(); }

    virtual iterator begin()
    { return abegin(); }
    virtual iterator end()
    { return aend(); }
};

template<typename _Key, typename _Value, typename _Compare>
void BinarySortTree<_Key, _Value, _Compare>::__insert_node(BinaryNode<_Key, _Value> *node)
{
    BinaryNode<_Key, _Value> *temp = _Root;
    _Compare com;
    while(temp != nullptr)
    {
        if(com(temp->First, node->First))
        {
            if(temp->Left == nullptr)
            {
                temp->Left = node;
                node->Parent = temp;
                __add_size(node);
                break;
            }
            else temp = temp->Left;
        }
        else if(com(node->First, temp->First))
        {
            if(temp->Right == nullptr)
            {
                temp->Right = node;
                node->Parent = temp;
                __add_size(node);
                break;
            }
            else temp = temp->Right;
        }
        else
        {
            temp->Left = node->Left;
            temp->Right = node->Right;
        }
    }
}

template<typename _Key, typename _Value, typename _Compare>
void BinarySortTree<_Key, _Value, _Compare>::__add_size(BinaryNode<_Key, _Value> *new_node, BinarySortTree::__uint32 size)
{
    while(new_node != nullptr)
    {
        if(new_node == new_node->Parent->Left)
            new_node->Parent->LeftChildSize += size;
        else
            new_node->Parent->RightChildSize += size;
        new_node = new_node->Parent;
    }
}

template<typename _Key, typename _Value, typename _Compare>
BinaryNode<_Key, _Value> * BinarySortTree<_Key, _Value, _Compare>::__get_leftchild(BinaryNode<_Key, _Value> *node)
{
    BinaryNode<_Key, _Value> *result = node;
    while(result->Left != nullptr)
        result = result->Left;
    return result;
}

template<typename _Key, typename _Value, typename _Compare>
BinaryNode<_Key, _Value> *BinarySortTree<_Key, _Value, _Compare>::__get_rightchild(BinaryNode<_Key, _Value> *node)
{
    BinaryNode<_Key, _Value> *result = node;
    while(result->Right != nullptr)
        result = result->Right;
    return result;
}

template<typename _Key, typename _Value, typename _Compare>
void BinarySortTree<_Key, _Value, _Compare>::insert(const _Key &key, const _Value &value)
{
    __insert_node(key, value);
}

template<typename _Key, typename _Value, typename _Compare>
void BinarySortTree<_Key, _Value, _Compare>::insert(_Key &&key, _Value &&value)
{
    __insert_node(std::forward<_Key&&>(key), std::forward<_Value&&>(value));
}

template<typename _Key, typename _Value, typename _Compare>
void BinarySortTree<_Key, _Value, _Compare>::insert(_Key &&key, const _Value &value)
{
    __insert_node(std::forward<_Key&&>(key), value);
}

template<typename _Key, typename _Value, typename _Compare>
void BinarySortTree<_Key, _Value, _Compare>::insert(const _Key &key, _Value &&value)
{
    __insert_node(key, std::forward<_Value&&>(value));
}

template<typename _Key, typename _Value, typename _Compare>
void BinarySortTree<_Key, _Value, _Compare>::remove(const _Key &key)
{

}

template<typename _Key, typename _Value, typename _Compare>
void BinarySortTree<_Key, _Value, _Compare>::remove(_Key &&key)
{

}

template<typename _Key, typename _Value, typename _Compare>
bool BinarySortTree<_Key, _Value, _Compare>::contains(const _Key &key)
{

}

template<typename _Key, typename _Value, typename _Compare>
bool BinarySortTree<_Key, _Value, _Compare>::contains(_Key &&key)
{

}

#endif // BINARYTREE_H
