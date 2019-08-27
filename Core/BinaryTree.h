#ifndef BINARYTREE_H
#define BINARYTREE_H

#include "Core/Memory.h"
#include "Core/TypeTraits.h"
#include "Core/IteratorBase.h"

namespace rapid
{

template<typename T>
struct Equal
{
    int operator()(const T &arg1, const T &arg2) const;
};

template<typename _DataType,
         typename _EqualTo = Equal<_DataType>>
class BinaryTree
{
private:
    using DataType = _DataType;
    using Reference = DataType &;
    using RvalueReference = DataType &&;
    using ConstReference = const DataType &;

    template<typename DataNodeType>
    struct BTreeNode
    {
    private:
        NodeBase<DataNodeType> *_M_data;
        BTreeNode *_M_left;
        BTreeNode *_M_right;
        BTreeNode *_M_parent;

        friend class BinaryTree;

        void set_left(BTreeNode *node)
        { _M_left = node; }
        void set_right(BTreeNode *node)
        { _M_right = node; }

        void append_left(ConstReference data)
        { set_left(new BTreeNode<DataNodeType>(data, _M_left, nullptr, this)); }
        void append_right(ConstReference data)
        { set_right(new BTreeNode<DataNodeType>(data, _M_right, nullptr, this)); }

    public:
        BTreeNode(const DataNodeType &data, const BTreeNode *left = nullptr,
                  const BTreeNode *right = nullptr, const BTreeNode *parent = nullptr)
            : _M_data(new NodeBase<DataNodeType>(data)), _M_left(left), _M_right(right), _M_parent(parent)
        { }
        ~BTreeNode()
        { delete _M_data; }
        DataNodeType& data()
        { return _M_data->ref_content(); }
    };

    using TreeNode = BTreeNode<DataType>;

    TreeNode *_M_root = nullptr;

public:
    BinaryTree() { }
    BinaryTree(const BinaryTree &) { }
    ~BinaryTree()
    { clear(); }

    TreeNode* root() const
    { return _M_root; }
    void append_root(ConstReference data)
    { _M_root = new TreeNode(data, _M_root); }
    void append_root(RvalueReference data)
    { _M_root = new TreeNode(forward<DataType>(data), _M_root); }
    void clear();

    static TreeNode* left_child(const TreeNode *node)
    { return node == nullptr ? nullptr : node->_M_left; }
    static TreeNode* right_child(const TreeNode *node)
    { return node == nullptr ? nullptr : node->_M_right; }
    static TreeNode* parent(const TreeNode *node)
    { return node == nullptr ? nullptr : node->_M_parent; }
    static void set_left(TreeNode *node, TreeNode *child)
    {
        if(node != nullptr)
        { node->set_left(child); }
    }
    static void set_right(TreeNode *node, TreeNode *child)
    {
        if(node != nullptr)
        { node->set_right(child); }
    }
    static void append_left(TreeNode *node, ConstReference data)
    {
        if(node != nullptr)
        { node->append_left(data); }
    }
    static void append_left(TreeNode *node, RvalueReference data)
    {
        if(node != nullptr)
        { node->append_left(forward<DataType>(data)); }
    }
    static void append_right(TreeNode *node, ConstReference data)
    {
        if(node != nullptr)
        { node->append_right(data); }
    }
    static void append_right(TreeNode *node, RvalueReference data)
    {
        if(node != nullptr)
        { node->append_right(forward<DataType>(data)); }
    }
    static void remove(TreeNode *node)
    {
        if(node != nullptr)
        { delete node; }
    }
    static bool is_brother(const TreeNode *node1, const TreeNode *node2)
    { return parent(node1) == parent(node2); }

public:

    TreeIterator(FormerIterator)
    TreeIterator(ConstFormerIterator)
    TreeIterator(MiddleIterator)
    TreeIterator(ConstMiddleIterator)
    TreeIterator(AfterIterator)
    TreeIterator(ConstAfterIterator)

    using iterator = AfterIterator;
    using const_iterator = ConstAfterIterator;

    using fiterator = FormerIterator;
    using const_fiterator = ConstFormerIterator;
    using miterator = MiddleIterator;
    using const_miterator = ConstMiddleIterator;
    using aiterator = AfterIterator;
    using const_aiterator = ConstAfterIterator;

    iterator begin()
    { return iterator(_M_root); }
    iterator end()
    { return iterator(); }
    fiterator fbegin()
    { return fiterator(_M_root); }
    fiterator fend()
    { return iterator(); }
};

}
#endif // BINARYTREE_H
