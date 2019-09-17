#ifndef AVLTREE_H
#define AVLTREE_H

#include "BinaryTree.h"
#include "Tree.h"
#include <initializer_list>

namespace rapid
{

template<typename _DataType,
         typename _Compare = Compare<_DataType>,
         size_type _BalanceFactor = 2>
class AVLTree
{
public:
    using DataType = _DataType;
    using CompareType = _Compare;
    using TreeType = BinaryTree<DataType>;
    using Self = AVLTree;
    using TreeNode = typename BinaryTree<DataType>::TreeNode;
public:
    using ValueType = DataType;
    using Reference = ValueType &;
    using ConstReference = const ValueType &;
    using RvalueReference = ValueType &&;
    using SizeType = size_type;

    using iterator = typename TreeType::iterator;
    using const_iterator = typename TreeType::const_iterator;
    using reverse_iterator = typename TreeType::reverse_iterator;
    using const_reverse_iterator = typename TreeType::const_reverse_iterator;

    using fiterator = typename TreeType::fiterator;
    using const_fiterator = typename TreeType::const_fiterator;
    using miterator = typename TreeType::miterator;
    using const_miterator = typename TreeType::const_miterator;
    using aiterator = typename TreeType::aiterator;
    using const_aiterator = typename TreeType::const_aiterator;

private:
    BinaryTree<DataType> _M_tree;

    void _F_adjust(TreeNode *node);
    iterator _F_insert(ConstReference arg);
    template<typename _InputType, typename _CompareType>
    iterator _F_find(const _InputType &arg) const;

    void _F_erase(TreeNode *node)
    { _F_adjust(_M_tree.erase(node)); }
public:
    AVLTree() { }
    AVLTree(const Self &tree)
        : _M_tree(TreeType(tree._M_tree)) { }
    AVLTree(Self &&tree)
        : _M_tree(TreeType(forward<Self>(tree)._M_tree)) { }
    AVLTree(std::initializer_list<ValueType> arg_list)
    { insert(arg_list); }

    void swap(const Self &tree)
    { _M_tree.swap(tree._M_tree); }
    void swap(Self &&tree)
    { _M_tree.swap(forward<Self>(tree)._M_tree); }

    iterator begin()
    { return _M_tree.begin(); }
    iterator end()
    { return _M_tree.end(); }
    const_iterator begin() const
    { return _M_tree.begin(); }
    const_iterator end() const
    { return _M_tree.end(); }
    const_iterator cbegin() const
    { return _M_tree.cbegin(); }
    const_iterator cend() const
    { return _M_tree.cend(); }

    reverse_iterator rbegin()
    { return _M_tree.rbegin(); }
    reverse_iterator rend()
    { return _M_tree.rend(); }
    const_reverse_iterator rbegin() const
    { return _M_tree.rbegin(); }
    const_reverse_iterator rend() const
    { return _M_tree.rend(); }
    const_reverse_iterator crbegin() const
    { return _M_tree.crbegin(); }
    const_reverse_iterator crend() const
    { return _M_tree.crend(); }

    fiterator fbegin()
    { return _M_tree.fbegin(); }
    fiterator fend()
    { return _M_tree.fend(); }
    const_fiterator fbegin() const
    { return _M_tree.fbegin(); }
    const_fiterator fend() const
    { return _M_tree.fend(); }
    const_fiterator cfbegin() const
    { return _M_tree.cfbegin(); }
    const_fiterator cfend() const
    { return _M_tree.cfend(); }

    aiterator abegin()
    { return _M_tree.abegin(); }
    aiterator aend()
    { return _M_tree.aend(); }
    const_aiterator abegin() const
    { return _M_tree.abegin(); }
    const_aiterator aend() const
    { return _M_tree.aend(); }
    const_aiterator cabegin() const
    { return _M_tree.cabegin(); }
    const_aiterator caend() const
    { return _M_tree.caend(); }

    SizeType size() const
    { return _M_tree.size(); }
    bool empty() const
    { return size() == 0; }
    SizeType depth() const
    { return _M_tree.depth(); }

    iterator find(ConstReference arg) const
    { return _F_find<ValueType, CompareType>(arg); }
    iterator find(RvalueReference arg) const
    { return _F_find<ValueType, CompareType>(forward<ValueType>(arg)); }
    template<typename _InputType, typename _CompareType>
    iterator find(const _InputType &arg) const
    { return _F_find<_InputType, _CompareType>(arg); }
    template<typename _InputType, typename _CompareType>
    iterator find(_InputType &&arg) const
    { return _F_find<_InputType, _CompareType>(forward<_InputType>(arg)); }

    template<typename _InputType, typename _CompareType>
    iterator find_and_insert(const _InputType &input);

    iterator find_and_insert(ConstReference arg)
    { return find_and_insert<ValueType, CompareType>(arg); }

    iterator insert(std::initializer_list<ValueType> arg_list)
    {
        iterator result;
        for(auto it = arg_list.begin(); true; )
        {
            result = _F_insert(*it);
            if(++it == arg_list.end())
            {
                return result;
            }
        }
    }
    iterator insert(ConstReference arg)
    { return _F_insert(arg); }
    iterator insert(RvalueReference arg)
    { return _F_insert(forward<ValueType>(arg)); }
    void erase(ConstReference arg)
    { erase(find(arg)); }
    void erase(RvalueReference arg)
    { erase(find(forward<ValueType>(arg))); }
    void erase(iterator it)
    { _F_erase(_M_tree.tree_node(it)); }

    TreeType to_ordinary_tree() const
    { return _M_tree; }
};

//-----------------------impl-----------------------//
//-----------------------impl-----------------------//
//-----------------------impl-----------------------//
//-----------------------impl-----------------------//
//-----------------------impl-----------------------//
template<typename _DataType, typename _Compare, size_type _BalanceFactor>
    void AVLTree<_DataType, _Compare, _BalanceFactor>::_F_adjust(TreeNode *node)
{
    TreeNode *visit = node;
    while(visit != nullptr)
    {
        SizeType left_dep = _M_tree.depth(_M_tree.left_child(visit));
        SizeType right_dep = _M_tree.depth(_M_tree.right_child(visit));
        if(left_dep > right_dep && left_dep - right_dep >= _BalanceFactor)
        {
            TreeNode *temp = _M_tree.left_child(visit);
            if(_M_tree.depth(_M_tree.left_child(temp)) < _M_tree.depth(_M_tree.right_child(temp)))
            {
                _M_tree.left_rotate(temp);
            }
            visit = _M_tree.right_rotate(visit);
        }
        else if(right_dep > left_dep && right_dep - left_dep >= _BalanceFactor)
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

template<typename _DataType, typename _Compare, size_type _BalanceFactor>
template<typename _InputType, typename _CompareType>
typename AVLTree<_DataType, _Compare, _BalanceFactor>::iterator
    AVLTree<_DataType, _Compare, _BalanceFactor>::find_and_insert(const _InputType &input)
{
    iterator result;
    if(empty())
    {
        return result = _M_tree.append_root(input);
    }
    TreeNode *node = _M_tree.root();
    while(true)
    {
        int res = _CompareType()(input, node->data());
        if(res == 0)
        {
            return result = node;
        }
        if(res > 0)
        {
            if(_M_tree.left_child(node) == nullptr)
            {
                node = _M_tree.append_left(node, input);
                break;
            }
            node = _M_tree.left_child(node);
        }
        else
        {
            if(_M_tree.right_child(node) == nullptr)
            {
                node = _M_tree.append_right(node, input);
                break;
            }
            node = _M_tree.right_child(node);
        }
    }
    _F_adjust(node);
    return result = node;
}



template<typename _DataType, typename _Compare, size_type _BalanceFactor>
template<typename _InputType, typename _CompareType>
typename AVLTree<_DataType, _Compare, _BalanceFactor>::iterator
    AVLTree<_DataType, _Compare, _BalanceFactor>::_F_find(const _InputType &arg) const
{
    TreeNode *node = _M_tree.root();
    iterator result;
    while(node != nullptr)
    {
        int res = _CompareType()(arg, node->data());
        if(res == 0)
        {
            return result = node;
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
    return result;
}



template<typename _DataType, typename _Compare, size_type _BalanceFactor>
typename AVLTree<_DataType, _Compare, _BalanceFactor>::iterator
    AVLTree<_DataType, _Compare, _BalanceFactor>::_F_insert(ConstReference arg)
{
    iterator result = find_and_insert(arg);
    TreeNode *node = _M_tree.tree_node(result);
    node->data() = arg;
    return result;
//    iterator result;
//    if(empty())
//    {
//        return result = _M_tree.append_root(arg);
//    }
//    TreeNode *node = _M_tree.root();
//    while(true)
//    {
//        int res = CompareType()(arg, node->data());
//        if(res == 0)
//        {
//            node->data() = arg;
//            return result = node;
//        }
//        if(res > 0)
//        {
//            if(_M_tree.left_child(node) == nullptr)
//            {
//                node = _M_tree.append_left(node, arg);
//                break;
//            }
//            node = _M_tree.left_child(node);
//        }
//        else
//        {
//            if(_M_tree.right_child(node) == nullptr)
//            {
//                node = _M_tree.append_right(node, arg);
//                break;
//            }
//            node = _M_tree.right_child(node);
//        }
//    }
//    _F_adjust(node);
    //    return result = node;
}


}; // end namespace rapid
#endif // AVLTREE_H
