#ifndef BALANCEBINARYTREE_H
#define BALANCEBINARYTREE_H

#include "Core/BinaryTree.h"

namespace rapid
{

template<typename T>
struct Compare
{
    int operator()(const T &arg1, const T &arg2) const
    {
        if(arg1 < arg2) return 1;
        if(arg2 < arg1) return -1;
        return 0;
    }
};

template<typename _DataType, typename _CompareType = Compare<_DataType>>
class BalanceBinaryTree
{
private:
    using DataType = _DataType;
    using TreeType = BinaryTree<DataType>;
    using CompareType = _CompareType;
    using Self = BalanceBinaryTree;
    using TreeNode = typename BinaryTree<DataType>::TreeNode;
public:
    using ValueType = DataType;
    using Reference = ValueType &;
    using ConstReference = const ValueType &;
    using RvalueReference = ValueType &&;
    using SizeType = size_type;

    using iterator = typename TreeType::iterator;
    using const_iterator = typename TreeType::const_iterator;

private:
    BinaryTree<DataType> _M_tree;

    void _F_adjust(TreeNode *node);
    iterator _F_insert(ConstReference arg);
public:
    BalanceBinaryTree() { }
    BalanceBinaryTree(const Self &tree)
        : _M_tree(TreeType(tree._M_tree)) { }
    BalanceBinaryTree(Self &&tree)
        : _M_tree(TreeType(forward<Self>(tree)._M_tree)) { }

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

    typename TreeType::fiterator fbegin()
    { return _M_tree.fbegin(); }
    typename TreeType::fiterator fend()
    { return _M_tree.fend(); }
    typename TreeType::aiterator abegin()
    { return _M_tree.abegin(); }
    typename TreeType::aiterator aend()
    { return _M_tree.aend(); }

    SizeType size() const
    { return _M_tree.size(); }
    bool empty() const
    { return size() == 0; }

    iterator insert(ConstReference arg)
    { return _F_insert(arg); }
    iterator insert(RvalueReference arg)
    { return _F_insert(forward<ValueType>(arg)); }
};

void test_BalanceBinayTree_main();

}
#endif // BALANCEBINARYTREE_H
