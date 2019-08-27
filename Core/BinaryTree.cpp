#include "BinaryTree.h"

template<typename T>
int rapid::Equal<T>::operator()(const T &arg1, const T &arg2) const
{
    if(arg1 < arg2) return 1;
    if(arg2 < arg1) return -1;
    return 0;
}

template<typename _DataType, typename _EqualTo>
void rapid::BinaryTree<_DataType, _EqualTo>::clear()
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
//---------------------------------------------------------//
template<typename _DataType, typename _EqualTo>
void rapid::BinaryTree<_DataType, _EqualTo>::FormerIterator::_F_next()
{
    if(_M_current == nullptr) return;
    TreeNode *temp = _M_next;
    if(_M_next == nullptr);
    else if(left_child(_M_next) != nullptr)
    {
        _M_next = left_child(_M_next);
    }
    else if(right_child(_M_next) != nullptr)
    {
        _M_next = right_child(_M_next);
    }
    else if(_M_next == left_child(_M_current))
    {
        if(right_child(_M_current) == nullptr)
        {
            if(parent(_M_current) != nullptr)
            {
                _M_next = right_child(parent(_M_current));
            }
            else
            {
                _M_next = nullptr;
            }
        }
        else
        {
            _M_next = right_child(_M_current);
        }
    }
    else if(is_brother(_M_next, _M_current))
    {
        TreeNode *node = parent(parent(_M_next));
        while(node != nullptr && right_child(node) == nullptr)
        {
            node = parent(node);
        }
        if(node != nullptr)
        {
            if(right_child(node) != nullptr)
            {
                _M_next = right_child(node);
            }
        }
    }
    _M_current = temp;
}

template<typename _DataType, typename _EqualTo>
void rapid::BinaryTree<_DataType, _EqualTo>::FormerIterator::_F_previous()
{

}

template<typename _DataType, typename _EqualTo>
rapid::BinaryTree<_DataType, _EqualTo>::FormerIterator::FormerIterator(const TreeNode *root)
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
template<typename _DataType, typename _EqualTo>
void rapid::BinaryTree<_DataType, _EqualTo>::ConstFormerIterator::_F_next()
{

}

template<typename _DataType, typename _EqualTo>
void rapid::BinaryTree<_DataType, _EqualTo>::ConstFormerIterator::_F_previous()
{

}

template<typename _DataType, typename _EqualTo>
rapid::BinaryTree<_DataType, _EqualTo>::ConstFormerIterator::ConstFormerIterator(const TreeNode *root)
{

}
//---------------------------------------------------------//
template<typename _DataType, typename _EqualTo>
void rapid::BinaryTree<_DataType, _EqualTo>::MiddleIterator::_F_next()
{

}

template<typename _DataType, typename _EqualTo>
void rapid::BinaryTree<_DataType, _EqualTo>::MiddleIterator::_F_previous()
{

}

template<typename _DataType, typename _EqualTo>
rapid::BinaryTree<_DataType, _EqualTo>::MiddleIterator::MiddleIterator(const TreeNode *root)
{

}

//---------------------------------------------------------//
template<typename _DataType, typename _EqualTo>
void rapid::BinaryTree<_DataType, _EqualTo>::ConstMiddleIterator::_F_next()
{

}

template<typename _DataType, typename _EqualTo>
void rapid::BinaryTree<_DataType, _EqualTo>::ConstMiddleIterator::_F_previous()
{

}

template<typename _DataType, typename _EqualTo>
rapid::BinaryTree<_DataType, _EqualTo>::ConstMiddleIterator::ConstMiddleIterator(const TreeNode *root)
{

}
//---------------------------------------------------------//
template<typename _DataType, typename _EqualTo>
void rapid::BinaryTree<_DataType, _EqualTo>::AfterIterator::_F_next()
{

}

template<typename _DataType, typename _EqualTo>
void rapid::BinaryTree<_DataType, _EqualTo>::AfterIterator::_F_previous()
{

}

template<typename _DataType, typename _EqualTo>
rapid::BinaryTree<_DataType, _EqualTo>::AfterIterator::AfterIterator(const TreeNode *root)
{

}
//---------------------------------------------------------//
template<typename _DataType, typename _EqualTo>
void rapid::BinaryTree<_DataType, _EqualTo>::ConstAfterIterator::_F_next()
{

}

template<typename _DataType, typename _EqualTo>
void rapid::BinaryTree<_DataType, _EqualTo>::ConstAfterIterator::_F_previous()
{

}

template<typename _DataType, typename _EqualTo>
rapid::BinaryTree<_DataType, _EqualTo>::ConstAfterIterator::ConstAfterIterator(const TreeNode *root)
{

}
//---------------------------------------------------------//
