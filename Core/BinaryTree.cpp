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
//---------------------***************---------------------//
template<typename _DataType>
typename rapid::BinaryTree<_DataType>::TreeNode* left_child_under(
        typename rapid::BinaryTree<_DataType>::TreeNode *node)
{
    using BT = rapid::BinaryTree<_DataType>;
    while(node != nullptr && BT::left_child(node) != nullptr)
    {
        node = BT::left_child(node);
    }
    return node;
}
template<typename _DataType>
typename rapid::BinaryTree<_DataType>::TreeNode* left_leaves(
        typename rapid::BinaryTree<_DataType>::TreeNode *node)
{
    using BT = rapid::BinaryTree<_DataType>;
    node = left_child_under<_DataType>(node);
    while(BT::right_child(node) != nullptr)
    {
        node = left_child_under<_DataType>(BT::right_child(node));
    }
    return node;
}
template<typename _DataType>
typename rapid::BinaryTree<_DataType>::TreeNode* former_next(
        typename rapid::BinaryTree<_DataType>::TreeNode *current)
{
    using namespace rapid;
    using BT = BinaryTree<_DataType>;
    if(BT::left_child(current) != nullptr)
    {
        return BT::left_child(current);
    }
    if(BT::right_child(current) != nullptr)
    {
        return BT::right_child(current);
    }
    while(current != nullptr && BT::right_child(current) == nullptr)
    {
        current = BT::parent(current);
    }
    return BT::right_child(current);
}
template<typename _DataType>
typename rapid::BinaryTree<_DataType>::TreeNode* middle_next(
        typename rapid::BinaryTree<_DataType>::TreeNode *current)
{
    using namespace rapid;
    using BT = BinaryTree<_DataType>;
    if(BT::right_child(current) != nullptr)
    {
        return left_child_under<_DataType>(BT::right_child(current));
    }
    if(current != BT::right_child(BT::parent(current)))
    {
        return BT::parent(current);
    }
    return BT::parent(BT::parent(current));
}

template<typename _DataType>
typename rapid::BinaryTree<_DataType>::TreeNode* after_next(
        typename rapid::BinaryTree<_DataType>::TreeNode *current)
{
    using namespace rapid;
    using BT = BinaryTree<_DataType>;
    if(current == BT::left_child(BT::parent(current)))
    {
        return left_leaves<_DataType>(BT::right_child(BT::parent(current)));
    }
    if(current == BT::right_child(BT::parent(current)))
    {
        return BT::parent(current);
    }
    return nullptr;
}

template<typename _DataType>
typename rapid::BinaryTree<_DataType>::TreeNode* former_previous(
        typename rapid::BinaryTree<_DataType>::TreeNode *current)
{
    using namespace rapid;
    using BT = BinaryTree<_DataType>;
    if(current == BT::left_child(BT::parent(current)))
    {
        return BT::parent(current);
    }
    if(current == BT::right_child(BT::parent(current)))
    {
        current = BT::left_child(BT::parent(current));
        while(current != nullptr && BT::right_child(current) == nullptr)
        {
            current = BT::left_child(current);
        }
        if(BT::right_child(current) == nullptr)
        {
            return current;
        }
        return BT::right_child(current);
    }
    return nullptr;
}
template<typename _DataType>
typename rapid::BinaryTree<_DataType>::TreeNode* middle_previous(
        typename rapid::BinaryTree<_DataType>::TreeNode *current)
{
    using namespace rapid;
    using BT = BinaryTree<_DataType>;
    if(BT::left_child(current) != nullptr)
    {
        return BT::left_child(current);
    }
    if(current == BT::right_child(BT::parent(current)))
    {
        return BT::parent(current);
    }
    return nullptr;
}

template<typename _DataType>
typename rapid::BinaryTree<_DataType>::TreeNode* after_previous(
        typename rapid::BinaryTree<_DataType>::TreeNode *current)
{
    using namespace rapid;
    using BT = BinaryTree<_DataType>;
    if(BT::right_child(current) != nullptr)
    {
        return BT::right_child(current);
    }
    if(BT::right_child(BT::parent(current)) == current)
    {
        current = BT::parent(current);
        while(current != nullptr && BT::left_child(current) == nullptr)
        {
            current = BT::parent(current);
        }
        if(BT::left_child(current) != nullptr)
        {
            return BT::left_child(current);
        }
        return nullptr;
    }
    return BT::left_child(current);
}
//---------------------***************---------------------//
//---------------------------------------------------------//
template<typename _DataType>
void rapid::BinaryTree<_DataType>::FormerIterator::_F_next()
{
    _M_current = former_next<_DataType>(_M_current);
}

template<typename _DataType>
void rapid::BinaryTree<_DataType>::FormerIterator::_F_previous()
{
    _M_current = former_previous<_DataType>(_M_current);
}

template<typename _DataType>
rapid::BinaryTree<_DataType>::FormerIterator::FormerIterator(const TreeNode *root)
{
    _M_current = root;
}
//---------------------------------------------------------//
template<typename _DataType>
void rapid::BinaryTree<_DataType>::ConstFormerIterator::_F_next()
{
    _M_current = former_next<_DataType>(_M_current);
}

template<typename _DataType>
void rapid::BinaryTree<_DataType>::ConstFormerIterator::_F_previous()
{
    _M_current = former_previous<_DataType>(_M_current);
}

template<typename _DataType>
rapid::BinaryTree<_DataType>::ConstFormerIterator::ConstFormerIterator(const TreeNode *root)
{
    _M_current = root;
}
//---------------------------------------------------------//
template<typename _DataType>
void rapid::BinaryTree<_DataType>::MiddleIterator::_F_next()
{
    _M_current = middle_next<_DataType>(_M_current);
}

template<typename _DataType>
void rapid::BinaryTree<_DataType>::MiddleIterator::_F_previous()
{
    _M_current = middle_previous<_DataType>(_M_current);
}

template<typename _DataType>
rapid::BinaryTree<_DataType>::MiddleIterator::MiddleIterator(const TreeNode *root)
{
    _M_current = left_child_under(root);
}

//---------------------------------------------------------//
template<typename _DataType>
void rapid::BinaryTree<_DataType>::ConstMiddleIterator::_F_next()
{
    _M_current = middle_next<_DataType>(_M_current);
}

template<typename _DataType>
void rapid::BinaryTree<_DataType>::ConstMiddleIterator::_F_previous()
{
    _M_current = middle_previous<_DataType>(_M_current);
}

template<typename _DataType>
rapid::BinaryTree<_DataType>::ConstMiddleIterator::ConstMiddleIterator(const TreeNode *root)
{
    _M_current = left_child_under(root);
}
//---------------------------------------------------------//
template<typename _DataType>
void rapid::BinaryTree<_DataType>::AfterIterator::_F_next()
{
    _M_current = after_next<_DataType>(_M_current);
}

template<typename _DataType>
void rapid::BinaryTree<_DataType>::AfterIterator::_F_previous()
{
    _M_current = after_previous<_DataType>(_M_current);
}

template<typename _DataType>
rapid::BinaryTree<_DataType>::AfterIterator::AfterIterator(const TreeNode *root)
{
    _M_current = left_leaves<_DataType>(root);
}
//---------------------------------------------------------//
template<typename _DataType>
void rapid::BinaryTree<_DataType>::ConstAfterIterator::_F_next()
{
    _M_current = after_next<_DataType>(_M_current);
}

template<typename _DataType>
void rapid::BinaryTree<_DataType>::ConstAfterIterator::_F_previous()
{
    _M_current = after_previous<_DataType>(_M_current);
}

template<typename _DataType>
rapid::BinaryTree<_DataType>::ConstAfterIterator::ConstAfterIterator(const TreeNode *root)
{
    _M_current = left_leaves<_DataType>(root);
}
//---------------------------------------------------------//
