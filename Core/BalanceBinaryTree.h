#ifndef BALANCEBINARYTREE_H
#define BALANCEBINARYTREE_H

#include "Core/BinaryTree.h"

namespace rapid
{

template<typename _First, typename _Second>
class Pair
{
    _First First;
    _First Second;

    Pair() { }
    Pair(const _First &f, const _Second &s) : First(f), Second(s) { }
    Pair(const _First &f, _Second &&s) : First(f), Second(forward<_Second>(s)) { }
    Pair(_First &&f, const _Second &s) : First(forward<_First>(f)), Second(s) { }
    Pair(_First &&f, _Second &&s) : First(forward<_First>(f)), Second(forward<_Second>(s)) { }
};

template<typename _DataType, typename _Equal = Equal<_DataType>>
class BalanceBinaryTree
{
private:
    using DataType = _DataType;
    using TreeType = BinaryTree<DataType>;
    using EqualType = _Equal;

protected:
    BinaryTree<DataType> _M_tree;

public:
    using iterator = typename TreeType::iterator;
    using const_iterator = typename TreeType::const_iterator;

    BalanceBinaryTree() { }
    BalanceBinaryTree(const BalanceBinaryTree &tree)
        : _M_tree(BinaryTree<DataType>(tree._M_tree))
    { }
    BalanceBinaryTree(BalanceBinaryTree &&tree)
        : _M_tree(BinaryTree<DataType>(forward<BalanceBinaryTree>(tree)._M_tree))
    { }

    void swap(const BalanceBinaryTree &tree)
    { _M_tree.swap(tree._M_tree); }
    void swap(BalanceBinaryTree &&tree)
    { _M_tree.swap(forward<BalanceBinaryTree>(tree)._M_tree); }

    iterator begin()
    { return _M_tree->begin(); }
    iterator end()
    { return _M_tree->end(); }
    const_iterator cbegin()
    { return _M_tree->cbegin(); }
    const_iterator cend()
    { return _M_tree->cend(); }

};

}
#endif // BALANCEBINARYTREE_H
