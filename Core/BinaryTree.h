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

template<typename _DataType,
         typename _EqualTo = Equal<_DataType>>
class BinaryTree
{
protected:
    using Type = _DataType;
    using Reference = Type &;
    using RvalueReference = Type &&;
    using ConstReference = const Type &;

    template<typename DataNodeType>
    struct BTreeNode
    {
    private:
        NodeBase<DataNodeType> *Data = nullptr;
        BTreeNode *Left = nullptr;
        BTreeNode *Right = nullptr;
        BTreeNode *Parent = nullptr;

        friend class BinaryTree;
    public:
        BTreeNode(const DataNodeType &data) : Data(new NodeBase<DataNodeType>(data)) { }
        DataNodeType& data() { return Data->ref_content(); }
    };

    using TreeNode = BTreeNode<_DataType>;

    TreeNode *_M_root = nullptr;

public:
    BinaryTree() { }
    BinaryTree(const BinaryTree &) { }

    TreeNode* root() { return _M_root; }
    TreeNode* left_child(const TreeNode *node)
    { return node == nullptr ? nullptr : node->Left; }
    TreeNode* right_child(const TreeNode *node)
    { return node == nullptr ? nullptr : node->Right; }
    TreeNode* parent(const TreeNode *node)
    { return node == nullptr ? nullptr : node->Parent; }
    void append_left(TreeNode *node, ConstReference data);
    void remove(TreeNode *node);
};

}
#endif // BINARYTREE_H
