#include "Core/BalanceBinaryTree.h"

template<typename _SizeType>
_SizeType abs(_SizeType s)
{ return s > 0 ? s : -s; }

template<typename _SizeType>
_SizeType distance(_SizeType s1, _SizeType s2)
{ return abs(s1 - s2); }

template<typename _DataType, typename _CompareType>
void rapid::BalanceBinaryTree<_DataType, _CompareType>::_F_adjust(TreeNode *node)
{
    TreeNode *visit = _M_tree.parent(node);
    while(visit != nullptr)
    {
        SizeType left_dep = _M_tree.depth(_M_tree.left_child(visit));
        SizeType right_dep = _M_tree.depth(_M_tree.right_child(visit));
        if(left_dep > right_dep && right_dep + 2 <= left_dep)
        {
            visit = _M_tree.right_rotate(visit);
        }
        else if(right_dep > left_dep && left_dep + 2 <= right_dep)
        {
            visit = _M_tree.left_rotate(visit);
        }
        break;
    }
}

template<typename _DataType, typename _CompareType>
typename rapid::BalanceBinaryTree<_DataType, _CompareType>::iterator
    rapid::BalanceBinaryTree<_DataType, _CompareType>::_F_insert(ConstReference arg)
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
            return iterator(node);
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
    return iterator(node);
}

#include <iostream>

void rapid::test_BalanceBinayTree_main()
{
    BalanceBinaryTree<int> bbt;
    bbt.insert(50);
    bbt.insert(10);
    bbt.insert(20);
    bbt.insert(40);
    bbt.insert(30);
    bbt.insert(60);
    bbt.insert(70);
    std::cout << "former iterator" << std::endl;
    for(auto it = bbt.fbegin(); it != bbt.fend(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    std::cout << "middle iterator" << std::endl;
    for(int i : bbt)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    std::cout << "after iterator" << std::endl;
    for(auto it = bbt.abegin(); it != bbt.aend(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}
