#ifndef BINARYTREE_H
#define BINARYTREE_H

#include "Core/Memory.h"

namespace rapid
{

template<typename T>
struct Equal
{
    int operator()(const T &arg1, const T &arg2);
};

template<typename _Key, typename _Value,
         typename _EqualTo = Equal<_Key>>
class BinaryTree
{
protected:
    using KeyType = _Key;
    using KeyReference = KeyType &;
    using KeyRvalueReference = KeyType &&;
    using ConstKeyReference = const KeyType &;
    using ValueType = _Value;
    using ValueReference = ValueType &;
    using ValueRvalueReference = ValueType &&;
    using ConstValueReference = const ValueType &;

    struct BTreeDataNode
    {
        NodeBase<ValueType> Key;
        NodeBase<ValueType> Value;
    };
    struct BTreeNode
    {
    private:
        BTreeDataNode *Data;
        BTreeNode *Left;
        BTreeNode *Right;
        BTreeNode *Parent;

        friend class BinaryTree;
    public:
        BTreeDataNode& data() { return *Data; }
    };

    BTreeNode *_M_root;

    void _F_ajust() { }

public:
    BinaryTree() { }
    BinaryTree(const BinaryTree &) { }

    BTreeNode& root() { return *_M_root; }
};

}
#endif // BINARYTREE_H
