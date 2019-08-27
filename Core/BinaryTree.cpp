#include "BinaryTree.h"

template<typename T>
int rapid::Equal<T>::operator()(const T &arg1, const T &arg2) const
{
    if(arg1 < arg2) return 1;
    if(arg2 < arg1) return -1;
    return 0;
}

template<typename _DataType>
void rapid::BinaryTree<_DataType>::clear()
{
    for(auto it = begin(); it != end(); ++it)
    {
        TreeNode *parent = parent(it._M_current);
        if(parent != nullptr)
        {
            if(it._M_current == left_child(parent))
            {
                parent->set_left(nullptr);
            }
            else
            {
                parent->set_right(nullptr);
            }
        }
        delete it._M_current;
    }
}
//---------------------***********---------------------//
template<typename _DataType>
typename rapid::BinaryTree<_DataType>::TreeNode* left_child_under(
        typename rapid::BinaryTree<_DataType>::TreeNode *node)
{
    using BT = rapid::BinaryTree<_DataType>;
    using Node = typename BT::TreeNode;
    Node *result = node;
    while(result != nullptr && BT::left_child(result) != nullptr)
    {
        result = BT::left_child(result);
    }
    return result;
}
template<typename _DataType>
typename rapid::BinaryTree<_DataType>::TreeNode* left_leaves(
        typename rapid::BinaryTree<_DataType>::TreeNode *node)
{
    using BT = rapid::BinaryTree<_DataType>;
    using Node = typename BT::TreeNode;
    Node *result = left_child_under<_DataType>(node);
    while(BT::right_child(result) != nullptr)
    {
        result = left_child_under<_DataType>(node);
    }
    return result;
}
template<typename _DataType>
typename rapid::BinaryTree<_DataType>::TreeNode* former_next(
        typename rapid::BinaryTree<_DataType>::TreeNode *current,
        typename rapid::BinaryTree<_DataType>::TreeNode *next)
{
    using namespace rapid;
    using BT = BinaryTree<_DataType>;
    using Node = typename BT::TreeNode;
    if(next == nullptr)
    {
        return nullptr;
    }
    if(BT::left_child(next) != nullptr)
    {
        return BT::left_child(next);
    }
    if(BT::right_child(next) != nullptr)
    {
        return BT::right_child(next);
    }
    if(next == BT::left_child(current))
    {
        if(BT::right_child(current) == nullptr)
        {
            if(BT::parent(current) != nullptr)
            {
                return BT::right_child(BT::parent(current));
            }
            return nullptr;
        }
        return BT::right_child(current);
    }
    if(BT::is_brother(next, current))
    {
        Node *node = BT::parent(BT::parent(next));
        while(node != nullptr && BT::right_child(node) == nullptr)
        {
            node = BT::parent(node);
        }
        if(node == nullptr || BT::right_child(node) == nullptr)
        {
            return nullptr;
        }
        return BT::right_child(node);
    }
    return nullptr;
}
template<typename _DataType>
typename rapid::BinaryTree<_DataType>::TreeNode* middle_next(
        typename rapid::BinaryTree<_DataType>::TreeNode *current,
        typename rapid::BinaryTree<_DataType>::TreeNode *next)
{
    using namespace rapid;
    using BT = BinaryTree<_DataType>;
    if(next == nullptr)
    {
        return nullptr;
    }
    if(BT::parent(next) == current)
    {
        if(BT::parent(current) == nullptr)
        {
            return nullptr;
        }
        next = BT::parent(current);
        while(BT::parent(next) != nullptr && BT::right_child(BT::parent(next)) == next)
        {
            next = BT::parent(next);
        }
        if(BT::parent(next) == nullptr)
        {
            return nullptr;
        }
        return left_child_under<_DataType>(BT::right_child(next));
    }
    if(BT::right_child(next) != nullptr)
    {
        return left_child_under<_DataType>(BT::right_child(next));
    }
    return BT::parent(next);
}

template<typename _DataType>
typename rapid::BinaryTree<_DataType>::TreeNode* after_next(
        typename rapid::BinaryTree<_DataType>::TreeNode *current,
        typename rapid::BinaryTree<_DataType>::TreeNode *next)
{
    using namespace rapid;
    using BT = BinaryTree<_DataType>;
    using Node = typename BT::TreeNode;
    if(next == nullptr)
    {
        return nullptr;
    }
}
//---------------------***********---------------------//
//---------------------------------------------------------//
template<typename _DataType>
void rapid::BinaryTree<_DataType>::FormerIterator::_F_next()
{
    if(_M_current == nullptr) return;
    TreeNode *temp = _M_next;
    _M_next = former_next<_DataType>(_M_current, _M_next);
    _M_current = temp;
}

template<typename _DataType>
void rapid::BinaryTree<_DataType>::FormerIterator::_F_previous()
{

}

template<typename _DataType>
rapid::BinaryTree<_DataType>::FormerIterator::FormerIterator(const TreeNode *root)
{
    _M_current = root;
    if(left_child(_M_current) != nullptr)
    {
        _M_next = left_child(_M_current);
    }
    else
    {
        _M_next = right_child(_M_current);
    }
}
//---------------------------------------------------------//
template<typename _DataType>
void rapid::BinaryTree<_DataType>::ConstFormerIterator::_F_next()
{
    if(_M_current == nullptr) return;
    TreeNode *temp = _M_next;
    _M_next = former_next<_DataType>(_M_current, _M_next);
    _M_current = temp;
}

template<typename _DataType>
void rapid::BinaryTree<_DataType>::ConstFormerIterator::_F_previous()
{
    if(_M_current == nullptr) return;
}

template<typename _DataType>
rapid::BinaryTree<_DataType>::ConstFormerIterator::ConstFormerIterator(const TreeNode *root)
{
    _M_current = root;
    if(left_child(_M_current) != nullptr)
    {
        _M_next = left_child(_M_current);
    }
    else
    {
        _M_next = right_child(_M_current);
    }
}
//---------------------------------------------------------//
template<typename _DataType>
void rapid::BinaryTree<_DataType>::MiddleIterator::_F_next()
{
    if(_M_current == nullptr) return;
    TreeNode *temp = _M_next;
    _M_next = middle_next<_DataType>(_M_current, _M_next);
    _M_current = temp;
}

template<typename _DataType>
void rapid::BinaryTree<_DataType>::MiddleIterator::_F_previous()
{

}

template<typename _DataType>
rapid::BinaryTree<_DataType>::MiddleIterator::MiddleIterator(const TreeNode *root)
{
    _M_current = root;
    while(_M_current != nullptr && left_child(_M_current) != nullptr)
    {
        _M_current = left_child(_M_current);
    }
    if(parent(_M_current) == nullptr)
    {
        _M_next = left_child_under(right_child(_M_current));
    }
    else
    {
        _M_next = parent(_M_current);
    }
}

//---------------------------------------------------------//
template<typename _DataType>
void rapid::BinaryTree<_DataType>::ConstMiddleIterator::_F_next()
{
    if(_M_current == nullptr) return;
    TreeNode *temp = _M_next;
    _M_next = middle_next<_DataType>(_M_current, _M_next);
    _M_current = temp;
}

template<typename _DataType>
void rapid::BinaryTree<_DataType>::ConstMiddleIterator::_F_previous()
{

}

template<typename _DataType>
rapid::BinaryTree<_DataType>::ConstMiddleIterator::ConstMiddleIterator(const TreeNode *root)
{
    _M_current = root;
    while(_M_current != nullptr && left_child(_M_current) != nullptr)
    {
        _M_current = left_child(_M_current);
    }
    if(parent(_M_current) == nullptr)
    {
        _M_next = left_child_under(right_child(_M_current));
    }
    else
    {
        _M_next = parent(_M_current);
    }
}
//---------------------------------------------------------//
template<typename _DataType>
void rapid::BinaryTree<_DataType>::AfterIterator::_F_next()
{
    if(_M_current == nullptr) return;
    TreeNode *temp = _M_next;
    _M_next = after_next<_DataType>(_M_current, _M_next);
    _M_current = temp;
}

template<typename _DataType>
void rapid::BinaryTree<_DataType>::AfterIterator::_F_previous()
{

}

template<typename _DataType>
rapid::BinaryTree<_DataType>::AfterIterator::AfterIterator(const TreeNode *root)
{
    _M_current = left_leaves<_DataType>(root);
    if(parent(_M_current) == nullptr)
    {
        _M_next = nullptr;
    }
    else
    {
        _M_next = parent(_M_current);
        while(right_child(_M_next) == nullptr)
        {
            _M_next = parent(_M_next);
        }
        _M_next = left_leaves<_DataType>(right_child(_M_next));
    }
}
//---------------------------------------------------------//
template<typename _DataType>
void rapid::BinaryTree<_DataType>::ConstAfterIterator::_F_next()
{
    if(_M_current == nullptr) return;
    TreeNode *temp = _M_next;
    _M_next = after_next<_DataType>(_M_current, _M_next);
    _M_current = temp;
}

template<typename _DataType>
void rapid::BinaryTree<_DataType>::ConstAfterIterator::_F_previous()
{

}

template<typename _DataType>
rapid::BinaryTree<_DataType>::ConstAfterIterator::ConstAfterIterator(const TreeNode *root)
{
    _M_current = left_leaves<_DataType>(root);
    if(parent(_M_current) == nullptr)
    {
        _M_next = nullptr;
    }
    else
    {
        _M_next = parent(_M_current);
        while(right_child(_M_next) == nullptr)
        {
            _M_next = parent(_M_next);
        }
        _M_next = left_leaves<_DataType>(right_child(_M_next));
    }
}
//---------------------------------------------------------//
