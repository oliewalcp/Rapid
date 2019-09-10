#ifndef AVLTREE_CPP
#define AVLTREE_CPP

#include "AVLTree.h"

template<typename _DataType, typename _CompareType>
void rapid::AVLTree<_DataType, _CompareType>::_F_adjust(TreeNode *node)
{
    TreeNode *visit = node;
    while(visit != nullptr)
    {
        SizeType left_dep = _M_tree.depth(_M_tree.left_child(visit));
        SizeType right_dep = _M_tree.depth(_M_tree.right_child(visit));
        if(left_dep > right_dep && left_dep - right_dep > 1)
        {
            TreeNode *temp = _M_tree.left_child(visit);
            if(_M_tree.depth(_M_tree.left_child(temp)) < _M_tree.depth(_M_tree.right_child(temp)))
            {
                _M_tree.left_rotate(temp);
            }
            visit = _M_tree.right_rotate(visit);
        }
        else if(right_dep > left_dep && right_dep - left_dep > 1)
        {
            TreeNode *temp = _M_tree.right_child(visit);
            if(_M_tree.depth(_M_tree.right_child(temp)) < _M_tree.depth(_M_tree.left_child(temp)))
            {
                _M_tree.right_rotate(temp);
            }
            visit = _M_tree.left_rotate(visit);
        }
        visit = _M_tree.parent(visit);
    }
}

template<typename _DataType, typename _CompareType>
typename rapid::AVLTree<_DataType, _CompareType>::iterator
    rapid::AVLTree<_DataType, _CompareType>::_F_insert(ConstReference arg)
{
    if(empty())
    {
        return iterator(_M_tree.append_root(arg));
    }
    TreeNode *node = _M_tree.root();
    while(true)
    {
        int res = CompareType()(arg, node->data());
        if(res == 0)
        {
            node->data() = arg;
            return iterator() = node;
        }
        if(res > 0)
        {
            if(_M_tree.left_child(node) == nullptr)
            {
                node = _M_tree.append_left(node, arg);
                break;
            }
            node = _M_tree.left_child(node);
        }
        else
        {
            if(_M_tree.right_child(node) == nullptr)
            {
                node = _M_tree.append_right(node, arg);
                break;
            }
            node = _M_tree.right_child(node);
        }
    }
    _F_adjust(node);
    return iterator() = node;
}

template<typename _DataType, typename _CompareType>
typename rapid::AVLTree<_DataType, _CompareType>::iterator
    rapid::AVLTree<_DataType, _CompareType>::_F_find(ConstReference arg) const
{
    TreeNode *node = _M_tree.root();
    while(true)
    {
        int res = CompareType()(arg, node->data());
        if(res == 0)
        {
            return iterator() = node;
        }
        if(res > 0)
        {
            node = _M_tree.left_child(node);
        }
        else
        {
            node = _M_tree.right_child(node);
        }
    }
    return iterator();
}

#include <iostream>

void rapid::test_AVLTree_main()
{
    std::cout << "------------" << __func__ << "------------" << std::endl;
    AVLTree<int> avl;
    avl.insert({50, 45, 40, 48, 39, 43, 47, 49, 38, 42, 44, 46});
    avl.insert({100, 90, 200, 60, 95, 199, 202, 80, 94, 96, 201, 300});
    std::cout << "former iterator" << std::endl;
    for(auto it = avl.fbegin(); it != avl.fend(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    std::cout << "middle iterator" << std::endl;
    for(int i : avl)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    std::cout << "after iterator" << std::endl;
    for(auto it = avl.abegin(); it != avl.aend(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    std::cout << "---------------erase-------------" << std::endl;
    avl.erase(100);
    std::cout << "former iterator" << std::endl;
    for(auto it = avl.fbegin(); it != avl.fend(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    std::cout << "middle iterator" << std::endl;
    for(int i : avl)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    std::cout << "after iterator" << std::endl;
    for(auto it = avl.abegin(); it != avl.aend(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    std::cout << "---------------------------------" << std::endl;
    avl.erase(48);
    std::cout << "former iterator" << std::endl;
    for(auto it = avl.fbegin(); it != avl.fend(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    std::cout << "middle iterator" << std::endl;
    for(int i : avl)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    std::cout << "after iterator" << std::endl;
    for(auto it = avl.abegin(); it != avl.aend(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    std::cout << "------------end------------" << std::endl;
}

#endif // end AVLTREE_CPP
