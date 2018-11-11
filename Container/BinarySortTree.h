#ifndef BINARYTREE_H
#define BINARYTREE_H
#include "C_Base.h"
#include <stack>

template <typename Key, typename Value>
struct BinaryNode
{
private:
    typedef unsigned int __uint32;
public:
    BinaryNode(_C_Base<Key> *key = nullptr, _C_Base<Value> *value = nullptr) :
        First(key), Second(value), Left(nullptr), Right(nullptr), Parent(nullptr) {}
    ~BinaryNode()
    {
        delete First;
        delete Second;
    }
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
protected:
    typedef unsigned int _uint32;
    BinaryNode<_Key, _Value> *_Root;
    _uint32 _ElementNumber;

    virtual void _insert_node(const _Key &key, const _Value &value)
    {
        BinaryNode<_Key, _Value> *node = new BinaryNode<_Key, _Value>((_C_Base<_Key> *)&key, (_C_Base<_Value> *)&value);
        _insert_node(node);
    }
    virtual void _insert_node(BinaryNode<_Key, _Value> *node);
    virtual void _add_size(BinaryNode<_Key, _Value> *new_node, int size = 1);
    virtual BinaryNode<_Key, _Value> *_get_leftchild(BinaryNode<_Key, _Value> *node);
    virtual BinaryNode<_Key, _Value> * _get_rightchild(BinaryNode<_Key, _Value> *node);
    virtual void _remove_node(const _Key &key);
    virtual void _remove_node(_Key && key);
    virtual void _remove_node(const miterator &it);
    virtual void _remove_node(const fiterator &it);
    virtual void _remove_node(const aiterator &it);
    virtual void _remove_node(const BinaryNode<_Key, _Value> *node);
    virtual bool _contains_node(const _Key &key);
public:
    BinarySortTree() : _ElementNumber(0), _Root(nullptr) {}
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

    virtual const BinaryNode<_Key, _Value> *get_node(const _Key& key);
    virtual const BinaryNode<_Key, _Value> *get_node(_Key && key);

    const BinaryNode<_Key, _Value> * operator[](const _Key& key);
    const BinaryNode<_Key, _Value> * operator[](_Key && key);

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
        const BinaryNode<_Key, _Value> * operator->() const
        { return __Current; }
        const BinaryNode<_Key, _Value> * operator*() const
        { return __Current; }
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
                //如果下一个节点是当前结点的父结点
                if(__Next == __Current->Parent)
                {
                    //如果下一个节点的父结点不为空
                    if(__Next->Parent != nullptr)
                    {
                        //如果下一个节点的父结点的右结点不为空
                        if(__Next->Parent->Right != nullptr)
                        {
                            BinaryNode<_Key, _Value> *temp = _get_leftchild(__Next->Parent->Right);
                            if(temp == __Next->Parent->Right) __Next = _get_rightchild(__Next->Parent->Right);
                            else __Next = temp;
                        }
                        else __Next = __Next->Parent;
                    }
                    else __Next = nullptr;
                }
                //如果下一个结点是当前结点的兄弟结点
                else if(__Next == __Current->Parent->Right)
                    __Next = __Current->Parent;
                //如果下一个节点是当前结点的最近兄弟树枝中的最左结点
                else
                {
                    //如果下一个节点的父结点不是当前结点的父结点
                    if(__Next->Parent != __Current->Parent)
                    {
                        if(__Next->Parent->Right != nullptr)
                        {
                            BinaryNode<_Key, _Value> *temp = _get_leftchild(__Next->Parent->Right);
                            if(temp == __Next->Parent->Right) __Next = _get_rightchild(__Next->Parent->Right);
                            else __Next = temp;
                        }
                        else __Next = __Next->Parent;
                    }
                    else __Next = nullptr;
                }
                __Current = temp;
            }
        }
    public:
        After_iterator(BinaryNode<_Key, _Value> *cur, BinaryNode<_Key, _Value> *nex) : __Current(cur), __Next(nex) {}
        After_iterator(BinaryNode<_Key, _Value> *arg = nullptr) :
            __Current(arg), __Next(arg->Left == nullptr ? arg->Right : nullptr) {}
        After_iterator& operator++()
        {
            __get_next_node();
            return *this;
        }
        After_iterator& operator++(int)
        {
            After_iterator it(__Current, __Next);
            __get_next_node();
            return it;
        }
        const BinaryNode<_Key, _Value> * operator->() const
        { return __Current; }
        const BinaryNode<_Key, _Value> * operator*() const
        { return __Current; }
        bool operator==(const After_iterator &arg) const {
            return arg.__Current == this->__Current;
        }
        bool operator!=(const After_iterator &arg) const {
            return arg.__Current != this->__Current;
        }
    } aiterator;
    typedef aiterator iterator;
    virtual aiterator abegin()
    {
        BinaryNode<_Key, _Value> *temp = _get_leftchild(_Root);
        if(temp->Parent == nullptr) return aiterator(temp, nullptr);
        return aiterator(temp, temp->Parent->Right == nullptr ? temp->Parent : temp->Parent->Right);
    }
    virtual aiterator aend()
    { return aiterator(); }

    virtual iterator begin()
    { return abegin(); }
    virtual iterator end()
    { return aend(); }

    typedef class Middle_iterator
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
                //如果下一个节点是当前结点的父结点
                if(__Next == __Current->Parent)
                {
                    //如果下一个节点的右子结点不为空
                    if(__Next->Right != nullptr)
                        __Next = __Next->Parent->Right;
                    else __Next = __Next->Parent;
                }
                //如果下一个结点是当前结点的右子结点
                else if(__Next == __Current->Right)
                    __Next = __Current->Parent;
                //如果下一个节点是当前结点的爷爷结点
                else if(__Next == __Current->Parent->Parent)
                {
                    BinaryNode<_Key, _Value> *temp = _get_leftchild(__Next->Right);
                    if(temp == __Next->Right) __Next = __Next->Right;
                    else __Next = temp;
                }
                __Current = temp;
            }
        }
    public:
        Middle_iterator(BinaryNode<_Key, _Value> *cur, BinaryNode<_Key, _Value> *nex) : __Current(cur), __Next(nex) {}
        Middle_iterator(BinaryNode<_Key, _Value> *arg = nullptr) :
            __Current(arg), __Next(arg->Left == nullptr ? arg->Right : nullptr) {}
        Middle_iterator& operator++()
        {
            __get_next_node();
            return *this;
        }
        Middle_iterator& operator++(int)
        {
            Middle_iterator it(__Current, __Next);
            __get_next_node();
            return it;
        }
        const BinaryNode<_Key, _Value> * operator->() const
        { return __Current; }
        const BinaryNode<_Key, _Value> * operator*() const
        { return __Current; }
        bool operator==(const Middle_iterator &arg) const
        { return arg.__Current == this->__Current; }
        bool operator!=(const Middle_iterator &arg) const
        { return arg.__Current != this->__Current; }
    } miterator;
    virtual miterator mbegin()
    {
        BinaryNode<_Key, _Value> *temp = _get_leftchild(_Root);
        if(temp->Parent == nullptr) return aiterator(temp, nullptr);
        return aiterator(temp, temp->Parent);
    }
    virtual miterator mend()
    { return miterator(); }
};

template<typename _Key, typename _Value, typename _Compare>
void BinarySortTree<_Key, _Value, _Compare>::_insert_node(BinaryNode<_Key, _Value> *node)
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
                _add_size(node);
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
                _add_size(node);
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
void BinarySortTree<_Key, _Value, _Compare>::_add_size(BinaryNode<_Key, _Value> *new_node, int size)
{
    while(new_node != nullptr && new_node->Parent != nullptr)
    {
        if(new_node == new_node->Parent->Left)
            new_node->Parent->LeftChildSize += size;
        else
            new_node->Parent->RightChildSize += size;
        new_node = new_node->Parent;
    }
}

template<typename _Key, typename _Value, typename _Compare>
BinaryNode<_Key, _Value> * BinarySortTree<_Key, _Value, _Compare>::_get_leftchild(BinaryNode<_Key, _Value> *node)
{
    BinaryNode<_Key, _Value> *result = node;
    while(result->Left != nullptr)
        result = result->Left;
    return result;
}

template<typename _Key, typename _Value, typename _Compare>
BinaryNode<_Key, _Value> *BinarySortTree<_Key, _Value, _Compare>::_get_rightchild(BinaryNode<_Key, _Value> *node)
{
    BinaryNode<_Key, _Value> *result = node;
    while(result->Right != nullptr)
        result = result->Right;
    return result;
}

template<typename _Key, typename _Value, typename _Compare>
void BinarySortTree<_Key, _Value, _Compare>::_remove_node(const _Key &key)
{
    const BinaryNode<_Key, _Value> *temp = get_node(key);
    _remove_node(temp);
}

template<typename _Key, typename _Value, typename _Compare>
void BinarySortTree::_remove_node(_Key &&key)
{  _remove_node(std::farward<_Key&&>(key)); }

template<typename _Key, typename _Value, typename _Compare>
void BinarySortTree<_Key, _Value, _Compare>::_remove_node(const BinaryNode<_Key, _Value> *node)
{
    if(node == nullptr) return;
    BinaryNode<_Key, _Value> *no_need = node;
    BinaryNode<_Key, _Value> *right_min = _get_leftchild(node->Right);
    //if [node] has right children
    if(right_min != nullptr)
    {
        node->Left->Parent = right_min;
        right_min->Left = node->Left;
        _add_size(node->Left, node->LeftChildSize + 1);
    }
    //if [node] doesn't have right children
    else
    {
        if(node == _Root)
        {
            _Root = node->Left;
            node->Left->Parent = nullptr;
        }
        else
        {
            //if [node] is a left child
            if(node == node->Parent->Left) node->Parent->Left = node->Left;
            //if [node] is a right child
            else node->Parent->Right = node->Left;
            node->Left->Parent = node->Parent;
            _add_size(node, -1);
        }
    }
    delete no_need;
}

template<typename _Key, typename _Value, typename _Compare>
void BinarySortTree<_Key, _Value, _Compare>::_remove_node(const BinarySortTree<_Key, _Value, _Compare>::miterator &it)
{ _remove_node(*it); }

template<typename _Key, typename _Value, typename _Compare>
void BinarySortTree<_Key, _Value, _Compare>::_remove_node(const BinarySortTree<_Key, _Value, _Compare>::aiterator &it)
{ _remove_node(*it); }

template<typename _Key, typename _Value, typename _Compare>
void BinarySortTree<_Key, _Value, _Compare>::_remove_node(const BinarySortTree<_Key, _Value, _Compare>::fiterator &it)
{ _remove_node(*it); }

template<typename _Key, typename _Value, typename _Compare>
bool BinarySortTree<_Key, _Value, _Compare>::_contains_node(const _Key &key)
{ return get_node(key) != nullptr; }

template<typename _Key, typename _Value, typename _Compare>
void BinarySortTree<_Key, _Value, _Compare>::insert(const _Key &key, const _Value &value)
{ _insert_node(key, value); }

template<typename _Key, typename _Value, typename _Compare>
void BinarySortTree<_Key, _Value, _Compare>::insert(_Key &&key, _Value &&value)
{ _insert_node(std::forward<_Key&&>(key), std::forward<_Value&&>(value)); }

template<typename _Key, typename _Value, typename _Compare>
void BinarySortTree<_Key, _Value, _Compare>::insert(_Key &&key, const _Value &value)
{ _insert_node(std::forward<_Key&&>(key), value); }

template<typename _Key, typename _Value, typename _Compare>
void BinarySortTree<_Key, _Value, _Compare>::insert(const _Key &key, _Value &&value)
{ _insert_node(key, std::forward<_Value&&>(value)); }

template<typename _Key, typename _Value, typename _Compare>
void BinarySortTree<_Key, _Value, _Compare>::remove(const _Key &key)
{ _remove_node(key); }

template<typename _Key, typename _Value, typename _Compare>
void BinarySortTree<_Key, _Value, _Compare>::remove(_Key &&key)
{ _remove_node(std::farward<_Key&&>(key)); }

template<typename _Key, typename _Value, typename _Compare>
bool BinarySortTree<_Key, _Value, _Compare>::contains(const _Key &key)
{ _contains_node(key); }

template<typename _Key, typename _Value, typename _Compare>
bool BinarySortTree<_Key, _Value, _Compare>::contains(_Key &&key)
{ _contains_node(std::farward<_Key&&>(key)); }

template<typename _Key, typename _Value, typename _Compare>
const BinaryNode<_Key, _Value> *BinarySortTree<_Key, _Value, _Compare>::get_node(const _Key &key)
{
    _Compare com;
    for(fiterator it = fbegin(); it != fend(); it++)
        if(!com(it->Second, key) && !com(key, it->Second))
            return *it;
}

template<typename _Key, typename _Value, typename _Compare>
const BinaryNode<_Key, _Value> *BinarySortTree<_Key, _Value, _Compare>::get_node(_Key && key)
{ return get_node(std::farward<_Key&&>(key)); }

template<typename _Key, typename _Value, typename _Compare>
const BinaryNode<_Key, _Value> *BinarySortTree::operator[](const _Key &key)
{ return get_node(key); }

template<typename _Key, typename _Value, typename _Compare>
const BinaryNode<_Key, _Value> *BinarySortTree::operator[](_Key &&key)
{ return get_node(std::farward<_Key&&>(key)); }

#endif // BINARYTREE_H
