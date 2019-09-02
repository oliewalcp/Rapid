#include "Core/BinaryTree.h"
#include "Core/Stack.h"

template<typename T>
int rapid::Equal<T>::operator()(const T &arg1, const T &arg2) const
{
    if(arg1 < arg2) return 1;
    if(arg2 < arg1) return -1;
    return 0;
}

template<typename _DataType>
void rapid::BinaryTree<_DataType>::_F_copy(const BinaryTree &tree)
{
    using namespace rapid;
    TreeNode *visit = tree._M_root;
    Stack<TreeNode*> src, dst;
    while(visit != nullptr)
    {
        TreeNode *node = visit;
        TreeNode *new_node = new TreeNode(tree.root()->data());
        dst.push(new_node);
        src.push(node);
        if(parent(node) == nullptr)
        {
            _M_root = new_node;
        }
        while(parent(node) != src.top())
        {
            dst.pop();
            src.pop();
        }
        if(right_child(parent(node)) == node)
        {
            dst.top()->append_right(node->data());
        }
        else
        {
            dst.top()->append_left(node->data());
        }

        if(visit->_M_left != nullptr)
        {
            visit = visit->_M_left;
        }
        else if(visit->_M_right != nullptr)
        {
            visit = visit->_M_right;
        }
        else
        {
            visit = visit->_M_parent;
        }
    }
}

template<typename _DataType>
void rapid::BinaryTree<_DataType>::clear()
{
    for(auto it = abegin(); it != aend(); ++it)
    {
        TreeNode *p = parent(it._M_current);
        if(p != nullptr)
        {
            if(it._M_current == left_child(p))
            {
                p->set_left(nullptr);
            }
            else
            {
                p->set_right(nullptr);
            }
        }
        delete it._M_current;
    }
}

template<typename _DataType>
void rapid::BinaryTree<_DataType>::rotate_right(TreeNode *node)
{

}

template<typename _DataType>
void rapid::BinaryTree<_DataType>::rotate_left(TreeNode *node)
{

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
    using Node = typename BT::TreeNode;
    if(BT::left_child(current) != nullptr)
    {
        return BT::left_child(current);
    }
    if(BT::right_child(current) != nullptr)
    {
        return BT::right_child(current);
    }
    Node *temp = current;
    current = BT::parent(current);
    while(current != nullptr && (BT::right_child(current) == temp || BT::right_child(current) == nullptr))
    {
        temp = current;
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
    using Node = typename BT::TreeNode;
    if(BT::right_child(current) != nullptr)
    {
        return left_child_under<_DataType>(BT::right_child(current));
    }
    if(current != BT::right_child(BT::parent(current)))
    {
        return BT::parent(current);
    }
    Node *temp = current;
    current = BT::parent(current);
    while(current != nullptr && BT::right_child(current) == temp)
    {
        temp = current;
        current = BT::parent(current);
    }
    return current;
}

template<typename _DataType>
typename rapid::BinaryTree<_DataType>::TreeNode* after_next(
        typename rapid::BinaryTree<_DataType>::TreeNode *current)
{
    using namespace rapid;
    using BT = BinaryTree<_DataType>;
    if(current == BT::left_child(BT::parent(current)))
    {
        if(BT::right_child(BT::parent(current)) == nullptr)
        {
            return BT::parent(current);
        }
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
rapid::BinaryTree<_DataType>::FormerIterator::FormerIterator(TreeNode *root)
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
rapid::BinaryTree<_DataType>::ConstFormerIterator::ConstFormerIterator(TreeNode *root)
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
rapid::BinaryTree<_DataType>::MiddleIterator::MiddleIterator(TreeNode *root)
{
    _M_current = left_child_under<_DataType>(root);
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
rapid::BinaryTree<_DataType>::ConstMiddleIterator::ConstMiddleIterator(TreeNode *root)
{
    _M_current = left_child_under<_DataType>(root);
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
rapid::BinaryTree<_DataType>::AfterIterator::AfterIterator(TreeNode *root)
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
rapid::BinaryTree<_DataType>::ConstAfterIterator::ConstAfterIterator(TreeNode *root)
{
    _M_current = left_leaves<_DataType>(root);
}
//---------------------------------------------------------//
#include <iostream>
void rapid::test_BinaryTree_main()
{
    using Node = BinaryTree<int>::TreeNode;
    BinaryTree<int> bt;
    Node *root = bt.append_root(50);
    Node *temp = bt.append_left(root, 10);
    bt.append_left(temp, 20);
    temp = bt.append_right(temp, 100);
    temp = bt.append_right(root, 500);
    bt.append_right(temp, -1);
    temp = bt.append_left(temp, -200);
    temp = bt.append_left(temp, 1);
    temp = bt.append_left(temp, 2);
    std::cout << "middle: " << std::endl;
    for(int i : bt)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl << "former: " << std::endl;
    for(auto it = bt.fbegin(); it != bt.fend(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl << "after: " << std::endl;
    for(auto it = bt.abegin(); it != bt.aend(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << "----------------------------" << std::endl;
    BinaryTree<int> bt2(bt);
    std::cout << "middle: " << std::endl;
    for(int i : bt2)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl << "former: " << std::endl;
    for(auto it = bt2.fbegin(); it != bt2.fend(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl << "after: " << std::endl;
    for(auto it = bt2.abegin(); it != bt2.aend(); ++it)
    {
        std::cout << *it << " ";
    }
}
