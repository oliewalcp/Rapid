#ifndef BINARYTREE_H
#define BINARYTREE_H

#include "Core/Memory.h"
#include "Core/TypeTraits.h"

namespace rapid
{

template<typename T>
struct Equal
{
    int operator()(const T &arg1, const T &arg2) const;
};

template<typename _DataType>
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

        BTreeNode* append_left(ConstReference data)
        { return set_left(new BTreeNode<DataNodeType>(data, _M_left, nullptr, this)); }
        BTreeNode* append_right(ConstReference data)
        { return set_right(new BTreeNode<DataNodeType>(data, _M_right, nullptr, this)); }

    public:
        BTreeNode(const DataNodeType &data, BTreeNode *left = nullptr,
                  BTreeNode *right = nullptr, BTreeNode *parent = nullptr)
            : _M_data(new NodeBase<DataNodeType>(data)), _M_left(left), _M_right(right), _M_parent(parent)
        { }
        ~BTreeNode()
        { delete _M_data; }
        DataNodeType& data()
        { return _M_data->ref_content(); }

        BTreeNode* set_left(BTreeNode *node)
        { return _M_left = node; }
        BTreeNode* set_right(BTreeNode *node)
        { return _M_right = node; }
        BTreeNode* set_parent(BTreeNode *node)
        { return _M_parent = node; }
    };
public:
    using TreeNode = BTreeNode<DataType>;
private:
    TreeNode *_M_root = nullptr;

    void _F_copy(const BinaryTree &tree);
public:
    BinaryTree() { }
    BinaryTree(const BinaryTree &tree)
    { _F_copy(tree); }
    BinaryTree(BinaryTree &&tree)
    { _F_copy(forward<BinaryTree>(tree)); }
    ~BinaryTree()
    { clear(); }

    TreeNode* root() const
    { return _M_root; }
    TreeNode* append_root(ConstReference data)
    { return _M_root = new TreeNode(data, _M_root); }
    TreeNode* append_root(RvalueReference data)
    { return _M_root = new TreeNode(forward<DataType>(data), _M_root); }
    void clear();

    void swap(const BinaryTree &tree)
    {
        TreeNode *node = tree._M_root;
        tree._M_root = _M_root;
        _M_root = node;
    }
    void swap(BinaryTree &&tree)
    {
        const BinaryTree &temp = forward<BinaryTree>(tree);
        TreeNode *node = temp._M_root;
        temp._M_root = _M_root;
        _M_root = node;
    }

    static TreeNode* left_child(const TreeNode *node)
    { return node == nullptr ? nullptr : node->_M_left; }
    static TreeNode* right_child(const TreeNode *node)
    { return node == nullptr ? nullptr : node->_M_right; }
    static TreeNode* parent(const TreeNode *node)
    { return node == nullptr ? nullptr : node->_M_parent; }
    static TreeNode* set_left(TreeNode *node, TreeNode *child)
    {
        if(node != nullptr)
        { return node->set_left(child); }
        return nullptr;
    }
    static TreeNode* set_right(TreeNode *node, TreeNode *child)
    {
        if(node != nullptr)
        { return node->set_right(child); }
        return nullptr;
    }
    static TreeNode* append_left(TreeNode *node, ConstReference data)
    {
        if(node != nullptr)
        { return node->append_left(data); }
        return nullptr;
    }
    static TreeNode* append_left(TreeNode *node, RvalueReference data)
    {
        if(node != nullptr)
        { return node->append_left(forward<DataType>(data)); }
        return nullptr;
    }
    static TreeNode* append_right(TreeNode *node, ConstReference data)
    {
        if(node != nullptr)
        { return node->append_right(data); }
        return nullptr;
    }
    static TreeNode* append_right(TreeNode *node, RvalueReference data)
    {
        if(node != nullptr)
        { return node->append_right(forward<DataType>(data)); }
        return nullptr;
    }
    static void remove(TreeNode *node)
    {
        if(node != nullptr)
        { delete node; }
    }
    static bool is_brother(const TreeNode *node1, const TreeNode *node2)
    { return parent(node1) == parent(node2) && node1 != node2; }

public:
    class FormerIterator
    {
    private:
        TreeNode *_M_current = nullptr;

        void _F_next();
        void _F_previous();

        friend class BinaryTree;

        FormerIterator(TreeNode *root);
    public:
        FormerIterator() { }
        FormerIterator(const FormerIterator &it) : _M_current(it._M_current) { }
        FormerIterator(FormerIterator &&it)
            : _M_current(forward<FormerIterator>(it)._M_current)
        { }

        FormerIterator operator++()
        {
            FormerIterator it = *this;
            _F_next();
            return it;
        }
        FormerIterator operator++(int)
        {
            _F_next();
            return *this;
        }
        FormerIterator operator--()
        {
            FormerIterator it = *this;
            _F_previous();
            return it;
        }
        FormerIterator operator--(int)
        {
            _F_previous();
            return *this;
        }
        DataType operator*() const
        { return _M_current->data(); }
        DataType* operator->() const
        { return &_M_current->data(); }
        bool operator==(const FormerIterator &it) const
        { return _M_current == it._M_current; }
        bool operator==(FormerIterator &&it) const
        { return _M_current == forward<FormerIterator>(it)._M_current; }
        bool operator!=(const FormerIterator &it) const
        { return _M_current != it._M_current; }
        bool operator!=(FormerIterator &&it) const
        { return _M_current != forward<FormerIterator>(it)._M_current; }
    };

    class ConstFormerIterator
    {
    private:
        TreeNode *_M_current = nullptr;

        void _F_next();
        void _F_previous();

        friend class BinaryTree;

        ConstFormerIterator(TreeNode *root);
    public:
        ConstFormerIterator() { }
        ConstFormerIterator(const ConstFormerIterator &it) : _M_current(it._M_current) { }
        ConstFormerIterator(ConstFormerIterator &&it)
            : _M_current(forward<ConstFormerIterator>(it)._M_current)
        { }

        ConstFormerIterator operator++()
        {
            ConstFormerIterator it = *this;
            _F_next();
            return it;
        }
        ConstFormerIterator operator++(int)
        {
            _F_next();
            return *this;
        }
        ConstFormerIterator operator--()
        {
            ConstFormerIterator it = *this;
            _F_previous();
            return it;
        }
        ConstFormerIterator operator--(int)
        {
            _F_previous();
            return *this;
        }
        DataType operator*() const
        { return _M_current->data(); }
        DataType* operator->() const
        { return &_M_current->data(); }
        bool operator==(const ConstFormerIterator &it) const
        { return _M_current == it._M_current; }
        bool operator==(ConstFormerIterator &&it) const
        { return _M_current == forward<ConstFormerIterator>(it)._M_current; }
        bool operator!=(const ConstFormerIterator &it) const
        { return _M_current != it._M_current; }
        bool operator!=(ConstFormerIterator &&it) const
        { return _M_current != forward<ConstFormerIterator>(it)._M_current; }
    };

    class MiddleIterator
    {
    private:
        TreeNode *_M_current = nullptr;

        void _F_next();
        void _F_previous();

        friend class BinaryTree;

        MiddleIterator(TreeNode *root);
    public:
        MiddleIterator() { }
        MiddleIterator(const MiddleIterator &it) : _M_current(it._M_current) { }
        MiddleIterator(MiddleIterator &&it)
            : _M_current(forward<MiddleIterator>(it)._M_current)
        { }

        MiddleIterator operator++()
        {
            MiddleIterator it = *this;
            _F_next();
            return it;
        }
        MiddleIterator operator++(int)
        {
            _F_next();
            return *this;
        }
        MiddleIterator operator--()
        {
            MiddleIterator it = *this;
            _F_previous();
            return it;
        }
        MiddleIterator operator--(int)
        {
            _F_previous();
            return *this;
        }
        DataType operator*() const
        { return _M_current->data(); }
        DataType* operator->() const
        { return &_M_current->data(); }
        bool operator==(const MiddleIterator &it) const
        { return _M_current == it._M_current; }
        bool operator==(MiddleIterator &&it) const
        { return _M_current == forward<MiddleIterator>(it)._M_current; }
        bool operator!=(const MiddleIterator &it) const
        { return _M_current != it._M_current; }
        bool operator!=(MiddleIterator &&it) const
        { return _M_current != forward<MiddleIterator>(it)._M_current; }
    };

    class ConstMiddleIterator
    {
    private:
        TreeNode *_M_current = nullptr;

        void _F_next();
        void _F_previous();

        friend class BinaryTree;

        ConstMiddleIterator(TreeNode *root);
    public:
        ConstMiddleIterator() { }
        ConstMiddleIterator(const ConstMiddleIterator &it) : _M_current(it._M_current) { }
        ConstMiddleIterator(ConstMiddleIterator &&it)
            : _M_current(forward<ConstMiddleIterator>(it)._M_current)
        { }

        ConstMiddleIterator operator++()
        {
            ConstMiddleIterator it = *this;
            _F_next();
            return it;
        }
        ConstMiddleIterator operator++(int)
        {
            _F_next();
            return *this;
        }
        ConstMiddleIterator operator--()
        {
            ConstMiddleIterator it = *this;
            _F_previous();
            return it;
        }
        ConstMiddleIterator operator--(int)
        {
            _F_previous();
            return *this;
        }
        DataType operator*() const
        { return _M_current->data(); }
        DataType* operator->() const
        { return &_M_current->data(); }
        bool operator==(const ConstMiddleIterator &it) const
        { return _M_current == it._M_current; }
        bool operator==(ConstMiddleIterator &&it) const
        { return _M_current == forward<ConstMiddleIterator>(it)._M_current; }
        bool operator!=(const ConstMiddleIterator &it) const
        { return _M_current != it._M_current; }
        bool operator!=(ConstMiddleIterator &&it) const
        { return _M_current != forward<ConstMiddleIterator>(it)._M_current; }
    };

    class AfterIterator
    {
    private:
        TreeNode *_M_current = nullptr;

        void _F_next();
        void _F_previous();

        friend class BinaryTree;

        AfterIterator(TreeNode *root);
    public:
        AfterIterator() { }
        AfterIterator(const AfterIterator &it) : _M_current(it._M_current) { }
        AfterIterator(AfterIterator &&it)
            : _M_current(forward<AfterIterator>(it)._M_current)
        { }

        AfterIterator operator++()
        {
            AfterIterator it = *this;
            _F_next();
            return it;
        }
        AfterIterator operator++(int)
        {
            _F_next();
            return *this;
        }
        AfterIterator operator--()
        {
            AfterIterator it = *this;
            _F_previous();
            return it;
        }
        AfterIterator operator--(int)
        {
            _F_previous();
            return *this;
        }
        DataType operator*() const
        { return _M_current->data(); }
        DataType* operator->() const
        { return &_M_current->data(); }
        bool operator==(const AfterIterator &it) const
        { return _M_current == it._M_current; }
        bool operator==(AfterIterator &&it) const
        { return _M_current == forward<AfterIterator>(it)._M_current; }
        bool operator!=(const AfterIterator &it) const
        { return _M_current != it._M_current; }
        bool operator!=(AfterIterator &&it) const
        { return _M_current != forward<AfterIterator>(it)._M_current; }
    };

    class ConstAfterIterator
    {
    private:
        TreeNode *_M_current = nullptr;

        void _F_next();
        void _F_previous();

        friend class BinaryTree;

        ConstAfterIterator(TreeNode *root);
    public:
        ConstAfterIterator() { }
        ConstAfterIterator(const ConstAfterIterator &it) : _M_current(it._M_current) { }
        ConstAfterIterator(ConstAfterIterator &&it)
            : _M_current(forward<ConstAfterIterator>(it)._M_current)
        { }

        ConstAfterIterator operator++()
        {
            ConstAfterIterator it = *this;
            _F_next();
            return it;
        }
        ConstAfterIterator operator++(int)
        {
            _F_next();
            return *this;
        }
        ConstAfterIterator operator--()
        {
            ConstAfterIterator it = *this;
            _F_previous();
            return it;
        }
        ConstAfterIterator operator--(int)
        {
            _F_previous();
            return *this;
        }
        DataType operator*() const
        { return _M_current->data(); }
        DataType* operator->() const
        { return &_M_current->data(); }
        bool operator==(const ConstAfterIterator &it) const
        { return _M_current == it._M_current; }
        bool operator==(ConstAfterIterator &&it) const
        { return _M_current == forward<ConstAfterIterator>(it)._M_current; }
        bool operator!=(const ConstAfterIterator &it) const
        { return _M_current != it._M_current; }
        bool operator!=(ConstAfterIterator &&it) const
        { return _M_current != forward<ConstAfterIterator>(it)._M_current; }
    };

    using iterator = MiddleIterator;
    using const_iterator = ConstMiddleIterator;

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
    const_iterator cbegin()
    { return const_iterator(_M_root); }
    const_iterator cend()
    { return const_iterator(); }

    fiterator fbegin()
    { return fiterator(_M_root); }
    fiterator fend()
    { return fiterator(); }
    const_fiterator cfbegin()
    { return const_fiterator(_M_root); }
    const_fiterator cfend()
    { return const_fiterator(); }

    miterator mbegin()
    { return miterator(_M_root); }
    miterator mend()
    { return miterator(); }
    const_miterator cmbegin()
    { return const_miterator(_M_root); }
    const_miterator cmend()
    { return const_miterator(); }

    aiterator abegin()
    { return aiterator(_M_root); }
    aiterator aend()
    { return aiterator(); }
    const_aiterator cabegin()
    { return const_aiterator(_M_root); }
    const_aiterator caend()
    { return const_aiterator(); }
};

void test_BinaryTree_main();

}
#endif // BINARYTREE_H
