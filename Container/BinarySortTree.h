#ifndef BINARYTREE_H
#define BINARYTREE_H
#include "C_Base.h"
#include "../Exception.h"

template<typename _First, typename _Second>
struct Pair
{
    Pair() {}
    Pair(const _First *first, const _Second *second) : First(*first), Second(*second) {}
    _First First;
    _Second Second;
};

template <typename Key, typename Value>
struct BinaryNode
{
private:
    typedef unsigned int __uint32;
public:
    BinaryNode(Pair<Key, Value>* pair) :
        Data(pair), Left(nullptr), Right(nullptr), Parent(nullptr) {}
    BinaryNode<Key, Value> *Left;
    BinaryNode<Key, Value> *Right;
    BinaryNode<Key, Value> *Parent;
    Pair<Key, Value> *Data;
    __uint32 LeftChildSize = 0;
    __uint32 RightChildSize = 0;
};

template<typename _Key, typename _Value, typename _Compare = Compare>
class BinarySortTree
{
public:
    BinarySortTree() : _ElementNumber(0), _Root(nullptr) {}
    virtual ~BinarySortTree() { clear(); }
    virtual void clear();

    virtual unsigned int size() { return _ElementNumber; }

    virtual void insert(const _Key& key, const _Value& value);
    virtual void insert(_Key && key, _Value && value);
    virtual void insert(_Key && key, const _Value& value);
    virtual void insert(const _Key& key, _Value && value);

    virtual void remove(const _Key& key);
    virtual void remove(_Key && key);

    virtual bool contains(const _Key& key);
    virtual bool contains(_Key && key);

    _Value& operator[](const _Key& key);
    _Value& operator[](_Key && key);

    typedef class Former_iterator
    {
    private:
        BinaryNode<_Key, _Value> *__Current;
        BinaryNode<_Key, _Value> *__Next;

        BinaryNode<_Key, _Value> * __get_closest_left_parent(BinaryNode<_Key, _Value> *node)
        {
            BinaryNode<_Key, _Value> *temp = node;
            while(temp != nullptr)
            {
                if(temp->Parent == nullptr) return nullptr;
                if(temp == temp->Parent->Left) return temp->Parent;
                temp = temp->Parent;
            }
            return temp;
        }
        BinaryNode<_Key, _Value> *__get_current_node()
        { return __Current; }
        void __get_next_node()
        {
            if(__Next == nullptr) __Current = nullptr;
            else
            {
                BinaryNode<_Key, _Value> *temp = __Next;
                //if [__Next] is a child of [__Current]
                if(__Next->Parent == __Current)
                {
                    if(__Next->Left != nullptr) __Next = __Next->Left;
                    else if(__Next->Right != nullptr) __Next = __Next->Right;
                    else
                    {
                        BinaryNode<_Key, _Value> *p = __get_closest_left_parent(__Next);
                        while(p != nullptr && p->Right == nullptr)
                            p = __get_closest_left_parent(p);
                        if(p == nullptr) __Next = nullptr;
                        else __Next = p->Right;
                    }
                }
                //if [__Next] is a brother of [__Current]
                else if(__Next == __Current->Parent->Right)
                    __Next = __Current->Parent->Parent != nullptr ? __Current->Parent->Parent->Right : nullptr;
                else __Next = __Next->Left != nullptr ? __Next->Left : __Next->Right;
                __Current = temp;
            }
        }
    public:
        Former_iterator(BinaryNode<_Key, _Value> *cur, BinaryNode<_Key, _Value> *nex) : __Current(cur), __Next(nex) {}
        Former_iterator(BinaryNode<_Key, _Value> *arg = nullptr) :
            __Current(arg), __Next(arg == nullptr ? nullptr : (arg->Left == nullptr ? arg->Right : nullptr))
        {}
        Former_iterator& operator++() {
            __get_next_node();
            return *this;
        }
        Former_iterator& operator++(int) {
            Former_iterator it(__Current, __Next);
            __get_next_node();
            return it;
        }
        const Pair<_Key, _Value> * operator->() const
        { return __Current->Data; }
        Pair<_Key, _Value> operator*() const
        { return *(__Current->Data); }
        bool operator==(const Former_iterator &arg) const {
            return arg.__Current == this->__Current;
        }
        bool operator!=(const Former_iterator &arg) const {
            return arg.__Current != this->__Current;
        }

        friend class BinarySortTree;
    } fiterator;

    virtual fiterator fbegin()
    { return fiterator(_Root, _Root == nullptr ? nullptr : (_Root->Left == nullptr ? _Root->Right : _Root->Left)); }
    virtual fiterator fend()
    { return fiterator(nullptr, nullptr); }

    typedef class After_iterator
    {
    private:
        BinaryNode<_Key, _Value> *__Current;
        BinaryNode<_Key, _Value> *__Next;

        BinaryNode<_Key, _Value> *__get_current_node()
        { return __Current; }
        void __get_next_node()
        {
            if(__Next == nullptr) __Current = nullptr;
            else
            {
                BinaryNode<_Key, _Value> *temp = __Next;
                //if [__Next] is [__Current]'s parent
                if(__Next == __Current->Parent)
                {
                    //if [__Next]'s parent is not null
                    if(__Next->Parent != nullptr)
                    {
                        //if [__Next] is left child
                        if(__Next == __Next->Parent->Left)
                        {
                            BinaryNode<_Key, _Value> *brother = __Next->Parent->Right;
                            if(brother == nullptr) __Next = __Next->Parent;
                            else
                            {
                                while(true)
                                {
                                    if(brother->Left == nullptr)
                                    {
                                        if(brother->Right != nullptr)
                                            brother = brother->Right;
                                        else break;
                                    }
                                    else brother = brother->Left;
                                }
                                __Next = brother;
                            }
                        }
                        else __Next = __Next->Parent;
                    }
                    else __Next = nullptr;
                }
                //if [__Next] is brother of [__Current]
                else if(__Next == __Current->Parent->Right)
                    __Next = __Current->Parent;
                else
                {
                    if(__Next->Parent == nullptr) __Next = nullptr;
                    else if(__Next == __Next->Parent->Left)
                        __Next = __Next->Parent->Right == nullptr ? __Next->Parent : __Next->Parent->Right;
                    else __Next = __Next->Parent;
                }
                __Current = temp;
            }
        }
    public:
        After_iterator(BinaryNode<_Key, _Value> *cur, BinaryNode<_Key, _Value> *nex) : __Current(cur), __Next(nex) {}
        After_iterator(BinaryNode<_Key, _Value> *arg = nullptr) :
            __Current(arg), __Next(arg == nullptr ? nullptr : (arg->Left == nullptr ? arg->Right : nullptr)) {}
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
        const Pair<_Key, _Value> * operator->() const
        { return __Current->Data; }
        Pair<_Key, _Value> operator*() const
        { return *(__Current->Data); }
        bool operator==(const After_iterator &arg) const {
            return arg.__Current == this->__Current;
        }
        bool operator!=(const After_iterator &arg) const {
            return arg.__Current != this->__Current;
        }

        friend class BinarySortTree;
    } aiterator;
    virtual aiterator abegin()
    {
        BinaryNode<_Key, _Value> *temp = _Root;
        if(temp == nullptr) return aiterator(nullptr, nullptr);
        temp = _get_under_node(temp);
        if(temp->Parent == nullptr) return aiterator(temp, nullptr);
        else if(temp->Parent->Right == temp || temp->Parent->Right == nullptr) return aiterator(temp, temp->Parent);
        else return aiterator(temp, _get_under_node(temp->Parent->Right));
    }
    virtual aiterator aend()
    { return aiterator(nullptr, nullptr); }

    typedef class Middle_iterator
    {
    private:
        BinaryNode<_Key, _Value> *__Current;
        BinaryNode<_Key, _Value> *__Next;

        BinaryNode<_Key, _Value> *__get_current_node()
        { return __Current; }
        BinaryNode<_Key, _Value> * __get_closest_left_parent(BinaryNode<_Key, _Value> *node)
        {
            BinaryNode<_Key, _Value> *temp = node;
            while(temp != nullptr)
            {
                if(temp->Parent == nullptr) return nullptr;
                if(temp == temp->Parent->Left) return temp->Parent;
                temp = temp->Parent;
            }
            return temp;
        }
        void __get_next_node()
        {
            if(__Next == nullptr) __Current = nullptr;
            else
            {
                BinaryNode<_Key, _Value> *temp = __Next;
                BinaryNode<_Key, _Value> *right = BinarySortTree<_Key, _Value, _Compare>::_get_leftchild(__Next->Right);
                __Next = right == nullptr ? __get_closest_left_parent(__Next) : right;
                __Current = temp;
            }
        }
    public:
        Middle_iterator(BinaryNode<_Key, _Value> *cur, BinaryNode<_Key, _Value> *nex) : __Current(cur), __Next(nex) {}
        Middle_iterator(BinaryNode<_Key, _Value> *arg = nullptr) :
            __Current(arg), __Next(arg == nullptr ? nullptr : (arg->Left == nullptr ? arg->Right : nullptr)) {}
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
        const Pair<_Key, _Value>* operator->() const
        { return __Current->Data; }
        Pair<_Key, _Value> operator*() const
        { return *(__Current->Data); }
        bool operator==(const Middle_iterator &arg) const
        { return arg.__Current == this->__Current; }
        bool operator!=(const Middle_iterator &arg) const
        { return arg.__Current != this->__Current; }

        friend class BinarySortTree;
    } miterator;
    typedef miterator iterator;
    virtual miterator mbegin()
    {
        BinaryNode<_Key, _Value> *temp = BinarySortTree<_Key, _Value, _Compare>::_get_leftchild(_Root);
        if(temp->Parent == nullptr)
            return miterator(temp, BinarySortTree<_Key, _Value, _Compare>::_get_leftchild(temp->Right));
        return miterator(temp, temp->Parent);
    }
    virtual miterator mend()
    { return miterator(nullptr, nullptr); }

    virtual iterator begin()
    { return mbegin(); }
    virtual iterator end()
    { return mend(); }
protected:
    BinaryNode<_Key, _Value> *_Root;
    unsigned int _ElementNumber;

    virtual void _insert_node(const _Key &key, const _Value &value)
    {
        BinaryNode<_Key, _Value> *node = new BinaryNode<_Key, _Value>(_get_pair(key, value));
        _insert_node(node);
    }
    virtual void _insert_node(BinaryNode<_Key, _Value> *node);
    virtual void _add_size(const BinaryNode<_Key, _Value> *new_node, int size = 1);
    virtual BinaryNode<_Key, _Value> * _left_child(BinaryNode<_Key, _Value> *node)
    { return BinarySortTree<_Key, _Value, _Compare>::_get_leftchild(node); }
    virtual BinaryNode<_Key, _Value> * _right_child(BinaryNode<_Key, _Value> *node)
    { return BinarySortTree<_Key, _Value, _Compare>::_get_rightchild(node); }
    static BinaryNode<_Key, _Value> * _get_leftchild(BinaryNode<_Key, _Value> *node)
    {
        BinaryNode<_Key, _Value> *result = node;
        while(result != nullptr && result->Left != nullptr)
            result = result->Left;
        return result;
    }
    static BinaryNode<_Key, _Value> * _get_rightchild(BinaryNode<_Key, _Value> *node)
    {
        BinaryNode<_Key, _Value> *result = node;
        while(result->Right != nullptr)
            result = result->Right;
        return result;
    }
    virtual void _remove_node(const _Key &key);
    virtual void _remove_node(_Key && key);
    virtual void _remove_node(Former_iterator &it);
    virtual void _remove_node(Middle_iterator &it);
    virtual void _remove_node(After_iterator &it);
    virtual void _remove_node(const BinaryNode<_Key, _Value> *node);
    virtual bool _contains_node(const _Key &key);
    virtual BinaryNode<_Key, _Value> * _get_node(const _Key& key);
    virtual void _release_node(const BinaryNode<_Key, _Value> *node)
    {
        delete node->Data;
        delete node;
    }
    virtual Pair<_Key, _Value>* _get_pair(const _Key &key, const _Value &value)
    { return new Pair<_Key, _Value>(&key, &value); }
    virtual BinaryNode<_Key, _Value> * _get_under_node(BinaryNode<_Key, _Value> *node)
    {
        while(node != nullptr)
        {
            if(node->Left == nullptr)
            {
                if(node->Right != nullptr)
                    node = node->Right;
                else return node;
            }
            else node = node->Left;
        }
    }
};

template<typename _Key, typename _Value, typename _Compare>
void BinarySortTree<_Key, _Value, _Compare>::_insert_node(BinaryNode<_Key, _Value> *node)
{
    if(_Root == nullptr)
    {
        _Root = node;
        _add_size(_Root);
        return;
    }
    BinaryNode<_Key, _Value> *temp = _Root;
    _Compare com;
    while(temp != nullptr)
    {
        if(com(temp->Data->First, node->Data->First))
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
        else if(com(node->Data->First, temp->Data->First))
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
        else
        {
            memcpy(&temp->Data->Second, &node->Data->Second, sizeof(_Value));
            break;
        }
    }
}

template<typename _Key, typename _Value, typename _Compare>
void BinarySortTree<_Key, _Value, _Compare>::_add_size(const BinaryNode<_Key, _Value> *new_node, int size)
{
    while(new_node != nullptr && new_node->Parent != nullptr)
    {
        if(new_node == new_node->Parent->Left)
            new_node->Parent->LeftChildSize += size;
        else
            new_node->Parent->RightChildSize += size;
        new_node = new_node->Parent;
    }
    _ElementNumber += size;
}

template<typename _Key, typename _Value, typename _Compare>
void BinarySortTree<_Key, _Value, _Compare>::_remove_node(const _Key &key)
{
    const BinaryNode<_Key, _Value> *temp = _get_node(key);
    _remove_node(temp);
}

template<typename _Key, typename _Value, typename _Compare>
void BinarySortTree<_Key, _Value, _Compare>::_remove_node(_Key &&key)
{  _remove_node(std::forward<_Key&&>(key)); }

template<typename _Key, typename _Value, typename _Compare>
void BinarySortTree<_Key, _Value, _Compare>::_remove_node(const BinaryNode<_Key, _Value> *node)
{
    if(node == nullptr) return;
    const BinaryNode<_Key, _Value> *no_need = node;
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
    _release_node(no_need);
}

template<typename _Key, typename _Value, typename _Compare>
void BinarySortTree<_Key, _Value, _Compare>::_remove_node(BinarySortTree<_Key, _Value, _Compare>::Middle_iterator &it)
{ _remove_node(it.__get_current_node()); }

template<typename _Key, typename _Value, typename _Compare>
void BinarySortTree<_Key, _Value, _Compare>::_remove_node(BinarySortTree<_Key, _Value, _Compare>::After_iterator &it)
{ _remove_node(it.__get_current_node()); }

template<typename _Key, typename _Value, typename _Compare>
void BinarySortTree<_Key, _Value, _Compare>::_remove_node(BinarySortTree<_Key, _Value, _Compare>::Former_iterator &it)
{ _remove_node(it.__get_current_node()); }

template<typename _Key, typename _Value, typename _Compare>
bool BinarySortTree<_Key, _Value, _Compare>::_contains_node(const _Key &key)
{ return _get_node(key) != nullptr; }

template<typename _Key, typename _Value, typename _Compare>
void BinarySortTree<_Key, _Value, _Compare>::clear()
{
    for(iterator it = begin(); it != end();)
    {
        BinaryNode<_Key, _Value> *node = it.__get_current_node();
        it++;
        _release_node(node);
    }
}

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
{ _remove_node(std::forward<_Key&&>(key)); }

template<typename _Key, typename _Value, typename _Compare>
bool BinarySortTree<_Key, _Value, _Compare>::contains(const _Key &key)
{ return _contains_node(key); }

template<typename _Key, typename _Value, typename _Compare>
bool BinarySortTree<_Key, _Value, _Compare>::contains(_Key &&key)
{ return _contains_node(std::forward<_Key&&>(key)); }

template<typename _Key, typename _Value, typename _Compare>
BinaryNode<_Key, _Value>* BinarySortTree<_Key, _Value, _Compare>::_get_node(const _Key &key)
{
    _Compare com;
    BinaryNode<_Key, _Value> *temp = _Root;
    while(temp != nullptr)
    {
        if(com(temp->Data->First, key))
            temp = temp->Right;
        else if(com(key, temp->Data->First))
            temp = temp->Left;
        else return temp;
    }
    return nullptr;
}

template<typename _Key, typename _Value, typename _Compare>
_Value& BinarySortTree<_Key, _Value, _Compare>::operator[](const _Key &key)
{
    BinaryNode<_Key, _Value> *temp = _get_node(key);
    if(temp == nullptr) throw Exception("there is no this key!");
    return temp->Data->Second;
}

template<typename _Key, typename _Value, typename _Compare>
_Value& BinarySortTree<_Key, _Value, _Compare>::operator[](_Key &&key)
{
    BinaryNode<_Key, _Value> *temp = _get_node(std::forward<_Key&&>(key));
    if(temp == nullptr) throw Exception("there is no this key!");
    return temp->Data->Second;
}
#endif // BINARYTREE_H
