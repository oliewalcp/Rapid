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
public:
    using DataType = _DataType;
    using Reference = DataType &;
    using RvalueReference = DataType &&;
    using ConstReference = const DataType &;
    using SizeType = size_type;

private:
    template<typename DataNodeType>
    struct BTreeNode
    {
    private:
        NodeBase<DataNodeType> *_M_data;
        BTreeNode *_M_left = nullptr;
        BTreeNode *_M_right = nullptr;
        BTreeNode *_M_parent = nullptr;
        SizeType _M_child_number = 0;

        friend class BinaryTree;

        void add_child_number(SizeType size)
        {
            _M_child_number += size;
            if(_M_parent != nullptr)
            { _M_parent->add_child_number(size); }
        }

        BTreeNode* append_left(ConstReference data)
        { return set_left(new BTreeNode<DataNodeType>(data, _M_left, nullptr)); }
        BTreeNode* append_right(ConstReference data)
        { return set_right(new BTreeNode<DataNodeType>(data, _M_right, nullptr)); }

        BTreeNode* set_parent(BTreeNode *node)
        { return _M_parent = node; }
        BTreeNode* set_left(BTreeNode *node)
        {
            add_child_number(- (_M_left == nullptr ? 0 : _M_left->_M_child_number + 1));
            if(node != nullptr)
            {
                node->set_parent(this);
                add_child_number(node->_M_child_number + 1);
            }
            return _M_left = node;
        }
        BTreeNode* set_right(BTreeNode *node)
        {
            add_child_number(- (_M_right == nullptr ? 0 : _M_right->_M_child_number + 1));
            if(node != nullptr)
            {
                node->set_parent(this);
                add_child_number(node->_M_child_number + 1);
            }
            return _M_right = node;
        }
        ~BTreeNode()
        { delete _M_data; }
    public:
        BTreeNode(const DataNodeType &data, BTreeNode *left = nullptr,
                  BTreeNode *right = nullptr, [[maybe_unused]] BTreeNode *parent = nullptr)
            : _M_data(new NodeBase<DataNodeType>(data))
        {
            set_left(left);
            set_right(right);
        }
        DataNodeType& data() const
        { return _M_data->ref_content(); }
        SizeType child_size() const
        { return _M_child_number; }
    };
public:
    using TreeNode = BTreeNode<DataType>;
private:
    TreeNode *_M_root = nullptr;

    void _F_copy(const BinaryTree &tree);

    static void _SF_release(TreeNode *node)
    {
        if(node == nullptr) return;
        _SF_release(node->_M_left);
        _SF_release(node->_M_right);
        delete node;
    }
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
    SizeType size() const
    { return _M_root == nullptr ? 0 : _M_root->child_size() + 1; }
    bool empty() const
    { return _M_root == nullptr; }
    void clear()
    {
        _SF_release(root());
        _M_root = nullptr;
    }

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
    void left_rotate(TreeNode *node);
    void right_rotate(TreeNode *node);

    static TreeNode* left_child(const TreeNode *node)
    { return node == nullptr ? nullptr : node->_M_left; }
    static TreeNode* right_child(const TreeNode *node)
    { return node == nullptr ? nullptr : node->_M_right; }
    static TreeNode* parent(const TreeNode *node)
    { return node == nullptr ? nullptr : node->_M_parent; }
    static TreeNode* set_left(TreeNode *node, TreeNode *child)
    {
        if(node == nullptr) return nullptr;
        _SF_release(left_child(node));
        return node->set_left(child);
    }
    static TreeNode* set_right(TreeNode *node, TreeNode *child)
    {
        if(node == nullptr) return nullptr;
        _SF_release(right_child(node));
        return node->set_right(child);
    }
    static TreeNode* append_left(TreeNode *node, ConstReference data)
    { return node == nullptr ? nullptr : node->append_left(data); }
    static TreeNode* append_left(TreeNode *node, RvalueReference data)
    { return node == nullptr ? nullptr : node->append_left(forward<DataType>(data)); }
    static TreeNode* append_right(TreeNode *node, ConstReference data)
    { return node == nullptr ? nullptr : node->append_right(data); }
    static TreeNode* append_right(TreeNode *node, RvalueReference data)
    { return node == nullptr ? nullptr : node->append_right(forward<DataType>(data)); }
    static void remove(TreeNode *node)
    {
        if(node != nullptr)
        { delete node; }
    }
    static bool is_brother(const TreeNode *node1, const TreeNode *node2)
    { return parent(node1) == parent(node2) && node1 != node2; }

private:
    class FormerIterator;
    class ConstFormerIterator;
    class MiddleIterator;
    class ConstMiddleIterator;
    class AfterIterator;
    class ConstAfterIterator;

    class FormerIterator
    {
    private:
        TreeNode *_M_current = nullptr;

        void _F_next();
        void _F_previous();

        friend class BinaryTree;
        ConstFormerIterator _F_const_cast()
        { return ConstFormerIterator(_M_current); }
        FormerIterator(TreeNode *root);
    public:
        FormerIterator() { }
        FormerIterator(const FormerIterator &it)
            : _M_current(it._M_current) { }
        FormerIterator(FormerIterator &&it)
            : _M_current(forward<FormerIterator>(it)._M_current) { }

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
        DataType operator*()
        { return _M_current->data(); }
        DataType* operator->()
        { return &_M_current->data(); }
        bool operator==(const FormerIterator &it) const
        { return _M_current == it._M_current; }
        bool operator!=(const FormerIterator &it) const
        { return _M_current != it._M_current; }
    };
    class ConstFormerIterator
    {
    private:
        const TreeNode *_M_current = nullptr;

        void _F_next();
        void _F_previous();

        friend class BinaryTree;
        FormerIterator _F_const_cast()
        { return FormerIterator(const_cast<TreeNode *>(_M_current)); }
        ConstFormerIterator(TreeNode *root);
    public:
        ConstFormerIterator() { }
        ConstFormerIterator(const ConstFormerIterator &it)
            : _M_current(it._M_current) { }
        ConstFormerIterator(ConstFormerIterator &&it)
            : _M_current(forward<ConstFormerIterator>(it)._M_current) { }
        ConstFormerIterator(const FormerIterator &it)
            : _M_current(it._M_current) { }
        ConstFormerIterator(FormerIterator &&it)
            : _M_current(forward<FormerIterator>(it)._M_current) { }

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
        bool operator!=(const ConstFormerIterator &it) const
        { return _M_current != it._M_current; }
    };
    class MiddleIterator
    {
    private:
        TreeNode *_M_current = nullptr;

        void _F_next();
        void _F_previous();

        friend class BinaryTree;
        ConstMiddleIterator _F_const_cast()
        { return ConstMiddleIterator(_M_current); }
        MiddleIterator(TreeNode *root);
    public:
        MiddleIterator() { }
        MiddleIterator(const MiddleIterator &it)
            : _M_current(it._M_current) { }
        MiddleIterator(MiddleIterator &&it)
            : _M_current(forward<MiddleIterator>(it)._M_current) { }

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
        DataType operator*()
        { return _M_current->data(); }
        DataType* operator->()
        { return &_M_current->data(); }
        bool operator==(const MiddleIterator &it) const
        { return _M_current == it._M_current; }
        bool operator!=(const MiddleIterator &it) const
        { return _M_current != it._M_current; }
    };
    class ConstMiddleIterator
    {
    private:
        const TreeNode *_M_current = nullptr;

        void _F_next();
        void _F_previous();

        friend class BinaryTree;
        MiddleIterator _F_const_cast()
        { return MiddleIterator(const_cast<TreeNode *>(_M_current)); }
        ConstMiddleIterator(TreeNode *root);
    public:
        ConstMiddleIterator() { }
        ConstMiddleIterator(const ConstMiddleIterator &it)
            : _M_current(it._M_current) { }
        ConstMiddleIterator(ConstMiddleIterator &&it)
            : _M_current(forward<ConstMiddleIterator>(it)._M_current) { }
        ConstMiddleIterator(const MiddleIterator &it)
            : _M_current(it._M_current) { }
        ConstMiddleIterator(MiddleIterator &&it)
            : _M_current(forward<MiddleIterator>(it)._M_current) { }

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
        ConstAfterIterator _F_const_cast()
        { return ConstAfterIterator(_M_current); }
        AfterIterator(TreeNode *root);
    public:
        AfterIterator() { }
        AfterIterator(const AfterIterator &it)
            : _M_current(it._M_current) { }
        AfterIterator(AfterIterator &&it)
            : _M_current(forward<AfterIterator>(it)._M_current) { }

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
        DataType operator*()
        { return _M_current->data(); }
        DataType* operator->()
        { return &_M_current->data(); }
        bool operator==(const AfterIterator &it) const
        { return _M_current == it._M_current; }
        bool operator!=(const AfterIterator &it) const
        { return _M_current != it._M_current; }
    };
    class ConstAfterIterator
    {
    private:
        const TreeNode *_M_current = nullptr;

        void _F_next();
        void _F_previous();

        friend class BinaryTree;
        AfterIterator _F_const_cast()
        { return AfterIterator(const_cast<TreeNode *>(_M_current)); }
        ConstAfterIterator(TreeNode *root);
    public:
        ConstAfterIterator() { }
        ConstAfterIterator(const ConstAfterIterator &it)
            : _M_current(it._M_current) { }
        ConstAfterIterator(ConstAfterIterator &&it)
            : _M_current(forward<ConstAfterIterator>(it)._M_current) { }
        ConstAfterIterator(const AfterIterator &it)
            : _M_current(it._M_current) { }
        ConstAfterIterator(AfterIterator &&it)
            : _M_current(forward<AfterIterator>(it)._M_current) { }

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

public:
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
    const_iterator begin() const
    { return const_iterator(_M_root); }
    const_iterator end() const
    { return const_iterator(); }
    const_iterator cbegin() const
    { return const_iterator(_M_root); }
    const_iterator cend() const
    { return const_iterator(); }

    fiterator fbegin()
    { return fiterator(_M_root); }
    fiterator fend()
    { return fiterator(); }
    const_fiterator fbegin() const
    { return const_fiterator(_M_root); }
    const_fiterator fend() const
    { return const_fiterator(); }
    const_fiterator cfbegin() const
    { return const_fiterator(_M_root); }
    const_fiterator cfend() const
    { return const_fiterator(); }

    miterator mbegin()
    { return miterator(_M_root); }
    miterator mend()
    { return miterator(); }
    const_miterator mbegin() const
    { return const_miterator(_M_root); }
    const_miterator mend() const
    { return const_miterator(); }
    const_miterator cmbegin() const
    { return const_miterator(_M_root); }
    const_miterator cmend() const
    { return const_miterator(); }

    aiterator abegin()
    { return aiterator(_M_root); }
    aiterator aend()
    { return aiterator(); }
    const_aiterator abegin() const
    { return const_aiterator(_M_root); }
    const_aiterator aend() const
    { return const_aiterator(); }
    const_aiterator cabegin() const
    { return const_aiterator(_M_root); }
    const_aiterator caend() const
    { return const_aiterator(); }
private:
    class ReverseFormerIterator;
    class ConstReverseFormerIterator;
    class ReverseMiddleIterator;
    class ConstReverseMiddleIterator;
    class ReverseAfterIterator;
    class ConstReverseAfterIterator;

    class ReverseFormerIterator
    {
    private:
        TreeNode *_M_current = nullptr;

        void _F_next();
        void _F_previous();
        ConstReverseFormerIterator _F_const_cast()
        { return ConstReverseFormerIterator(_M_current); }
        friend class BinaryTree;
        ReverseFormerIterator(TreeNode *root);
    public:
        ReverseFormerIterator() { }
        ReverseFormerIterator(const ReverseFormerIterator &it)
            : _M_current(it._M_current) { }
        ReverseFormerIterator(ReverseFormerIterator &&it)
            : _M_current(forward<ReverseFormerIterator>(it)._M_current) { }

        ReverseFormerIterator operator++()
        {
            ReverseFormerIterator it = *this;
            _F_next();
            return it;
        }
        ReverseFormerIterator operator++(int)
        {
            _F_next();
            return *this;
        }
        ReverseFormerIterator operator--()
        {
            ReverseFormerIterator it = *this;
            _F_previous();
            return it;
        }
        ReverseFormerIterator operator--(int)
        {
            _F_previous();
            return *this;
        }
        DataType operator*()
        { return _M_current->data(); }
        DataType* operator->()
        { return &_M_current->data(); }
        bool operator==(const ReverseFormerIterator &it) const
        { return _M_current == it._M_current; }
        bool operator!=(const ReverseFormerIterator &it) const
        { return _M_current != it._M_current; }
    };
    class ConstReverseFormerIterator
    {
    private:
        const TreeNode *_M_current = nullptr;

        void _F_next();
        void _F_previous();

        friend class BinaryTree;
        ReverseFormerIterator _F_const_cast()
        { return ReverseFormerIterator(const_cast<TreeNode *>(_M_current)); }
        ConstReverseFormerIterator(TreeNode *root);
    public:
        ConstReverseFormerIterator() { }
        ConstReverseFormerIterator(const ConstReverseFormerIterator &it)
            : _M_current(it._M_current) { }
        ConstReverseFormerIterator(ConstReverseFormerIterator &&it)
            : _M_current(forward<ConstReverseFormerIterator>(it)._M_current) { }
        ConstReverseFormerIterator(const ReverseFormerIterator &it)
            : _M_current(it._M_current) { }
        ConstReverseFormerIterator(ReverseFormerIterator &&it)
            : _M_current(forward<ReverseFormerIterator>(it)._M_current) { }

        ConstReverseFormerIterator operator++()
        {
            ConstReverseFormerIterator it = *this;
            _F_next();
            return it;
        }
        ConstReverseFormerIterator operator++(int)
        {
            _F_next();
            return *this;
        }
        ConstReverseFormerIterator operator--()
        {
            ConstReverseFormerIterator it = *this;
            _F_previous();
            return it;
        }
        ConstReverseFormerIterator operator--(int)
        {
            _F_previous();
            return *this;
        }
        DataType operator*() const
        { return _M_current->data(); }
        DataType* operator->() const
        { return &_M_current->data(); }
        bool operator==(const ConstReverseFormerIterator &it) const
        { return _M_current == it._M_current; }
        bool operator==(ConstReverseFormerIterator &&it) const
        { return _M_current == forward<ConstReverseFormerIterator>(it)._M_current; }
        bool operator!=(const ConstReverseFormerIterator &it) const
        { return _M_current != it._M_current; }
        bool operator!=(ConstReverseFormerIterator &&it) const
        { return _M_current != forward<ConstReverseFormerIterator>(it)._M_current; }
    };
    class ReverseMiddleIterator
    {
    private:
        TreeNode *_M_current = nullptr;

        void _F_next();
        void _F_previous();

        friend class BinaryTree;
        ConstReverseMiddleIterator _F_const_cast()
        { return ConstReverseMiddleIterator(_M_current); }
        ReverseMiddleIterator(TreeNode *root);
    public:
        ReverseMiddleIterator() { }
        ReverseMiddleIterator(const ReverseMiddleIterator &it)
            : _M_current(it._M_current) { }
        ReverseMiddleIterator(ReverseMiddleIterator &&it)
            : _M_current(forward<ReverseMiddleIterator>(it)._M_current) { }

        ReverseMiddleIterator operator++()
        {
            ReverseMiddleIterator it = *this;
            _F_next();
            return it;
        }
        ReverseMiddleIterator operator++(int)
        {
            _F_next();
            return *this;
        }
        ReverseMiddleIterator operator--()
        {
            ReverseMiddleIterator it = *this;
            _F_previous();
            return it;
        }
        ReverseMiddleIterator operator--(int)
        {
            _F_previous();
            return *this;
        }
        DataType operator*()
        { return _M_current->data(); }
        DataType* operator->()
        { return &_M_current->data(); }
        bool operator==(const ReverseMiddleIterator &it) const
        { return _M_current == it._M_current; }
        bool operator!=(const ReverseMiddleIterator &it) const
        { return _M_current != it._M_current; }
    };
    class ConstReverseMiddleIterator
    {
    private:
        const TreeNode *_M_current = nullptr;

        void _F_next();
        void _F_previous();

        friend class BinaryTree;
        ReverseMiddleIterator _F_const_cast()
        { return ReverseMiddleIterator(const_cast<TreeNode *>(_M_current)); }
        ConstReverseMiddleIterator(TreeNode *root);
    public:
        ConstReverseMiddleIterator() { }
        ConstReverseMiddleIterator(const ConstReverseMiddleIterator &it)
            : _M_current(it._M_current) { }
        ConstReverseMiddleIterator(ConstReverseMiddleIterator &&it)
            : _M_current(forward<ConstReverseMiddleIterator>(it)._M_current) { }
        ConstReverseMiddleIterator(const ReverseMiddleIterator &it)
            : _M_current(it._M_current) { }
        ConstReverseMiddleIterator(ReverseMiddleIterator &&it)
            : _M_current(forward<ReverseMiddleIterator>(it)._M_current) { }

        ConstReverseMiddleIterator operator++()
        {
            ConstReverseMiddleIterator it = *this;
            _F_next();
            return it;
        }
        ConstReverseMiddleIterator operator++(int)
        {
            _F_next();
            return *this;
        }
        ConstReverseMiddleIterator operator--()
        {
            ConstReverseMiddleIterator it = *this;
            _F_previous();
            return it;
        }
        ConstReverseMiddleIterator operator--(int)
        {
            _F_previous();
            return *this;
        }
        DataType operator*() const
        { return _M_current->data(); }
        DataType* operator->() const
        { return &_M_current->data(); }
        bool operator==(const ConstReverseMiddleIterator &it) const
        { return _M_current == it._M_current; }
        bool operator==(ConstReverseMiddleIterator &&it) const
        { return _M_current == forward<ConstReverseMiddleIterator>(it)._M_current; }
        bool operator!=(const ConstReverseMiddleIterator &it) const
        { return _M_current != it._M_current; }
        bool operator!=(ConstReverseMiddleIterator &&it) const
        { return _M_current != forward<ConstReverseMiddleIterator>(it)._M_current; }
    };
    class ReverseAfterIterator
    {
    private:
        TreeNode *_M_current = nullptr;

        void _F_next();
        void _F_previous();

        friend class BinaryTree;
        ConstReverseAfterIterator _F_const_cast()
        { return ConstReverseAfterIterator(_M_current); }
        ReverseAfterIterator(TreeNode *root);
    public:
        ReverseAfterIterator() { }
        ReverseAfterIterator(const ReverseAfterIterator &it)
            : _M_current(it._M_current) { }
        ReverseAfterIterator(ReverseAfterIterator &&it)
            : _M_current(forward<ReverseAfterIterator>(it)._M_current) { }

        ReverseAfterIterator operator++()
        {
            ReverseAfterIterator it = *this;
            _F_next();
            return it;
        }
        ReverseAfterIterator operator++(int)
        {
            _F_next();
            return *this;
        }
        ReverseAfterIterator operator--()
        {
            ReverseAfterIterator it = *this;
            _F_previous();
            return it;
        }
        ReverseAfterIterator operator--(int)
        {
            _F_previous();
            return *this;
        }
        DataType operator*()
        { return _M_current->data(); }
        DataType* operator->()
        { return &_M_current->data(); }
        bool operator==(const ReverseAfterIterator &it) const
        { return _M_current == it._M_current; }
        bool operator!=(const ReverseAfterIterator &it) const
        { return _M_current != it._M_current; }
    };
    class ConstReverseAfterIterator
    {
    private:
        const TreeNode *_M_current = nullptr;

        void _F_next();
        void _F_previous();

        friend class BinaryTree;
        ReverseAfterIterator _F_const_cast()
        { return ReverseAfterIterator(const_cast<TreeNode *>(_M_current)); }
        ConstReverseAfterIterator(TreeNode *root);
    public:
        ConstReverseAfterIterator() { }
        ConstReverseAfterIterator(const ConstReverseAfterIterator &it)
            : _M_current(it._M_current) { }
        ConstReverseAfterIterator(ConstReverseAfterIterator &&it)
            : _M_current(forward<ConstReverseAfterIterator>(it)._M_current) { }
        ConstReverseAfterIterator(const ReverseAfterIterator &it)
            : _M_current(it._M_current) { }
        ConstReverseAfterIterator(ReverseAfterIterator &&it)
            : _M_current(forward<ReverseAfterIterator>(it)._M_current) { }

        ConstReverseAfterIterator operator++()
        {
            ConstReverseAfterIterator it = *this;
            _F_next();
            return it;
        }
        ConstReverseAfterIterator operator++(int)
        {
            _F_next();
            return *this;
        }
        ConstReverseAfterIterator operator--()
        {
            ConstReverseAfterIterator it = *this;
            _F_previous();
            return it;
        }
        ConstReverseAfterIterator operator--(int)
        {
            _F_previous();
            return *this;
        }
        DataType operator*() const
        { return _M_current->data(); }
        DataType* operator->() const
        { return &_M_current->data(); }
        bool operator==(const ConstReverseAfterIterator &it) const
        { return _M_current == it._M_current; }
        bool operator!=(const ConstReverseAfterIterator &it) const
        { return _M_current != it._M_current; }
    };

public:
    using reverse_iterator = ReverseMiddleIterator;
    using const_reverse_iterator = ConstReverseMiddleIterator;

    using freverse_iterator = ReverseFormerIterator;
    using const_freverse_iterator = ConstReverseFormerIterator;
    using mreverse_iterator = ReverseMiddleIterator;
    using const_mreverse_iterator = ConstReverseMiddleIterator;
    using areverse_iterator = ReverseAfterIterator;
    using const_areverse_iterator = ConstReverseAfterIterator;

    reverse_iterator rbegin()
    { return reverse_iterator(_M_root); }
    reverse_iterator rend()
    { return reverse_iterator(); }
    const_reverse_iterator rbegin() const
    { return const_reverse_iterator(_M_root); }
    const_reverse_iterator rend() const
    { return const_reverse_iterator(); }

    freverse_iterator frbegin()
    { return freverse_iterator(_M_root); }
    freverse_iterator frend()
    { return freverse_iterator(); }
    const_freverse_iterator frbegin() const
    { return const_freverse_iterator(_M_root); }
    const_freverse_iterator frend() const
    { return const_freverse_iterator(); }
    const_freverse_iterator cfrbegin() const
    { return const_freverse_iterator(_M_root); }
    const_freverse_iterator cfrend() const
    { return const_freverse_iterator(); }

    mreverse_iterator mrbegin()
    { return mreverse_iterator(_M_root); }
    mreverse_iterator mrend()
    { return mreverse_iterator(); }
    const_mreverse_iterator mrbegin() const
    { return const_mreverse_iterator(_M_root); }
    const_mreverse_iterator mrend() const
    { return const_mreverse_iterator(); }
    const_mreverse_iterator cmrbegin() const
    { return const_mreverse_iterator(_M_root); }
    const_mreverse_iterator cmrend() const
    { return const_mreverse_iterator(); }

    areverse_iterator arbegin()
    { return areverse_iterator(_M_root); }
    areverse_iterator arend()
    { return areverse_iterator(); }
    const_areverse_iterator arbegin() const
    { return const_areverse_iterator(_M_root); }
    const_areverse_iterator arend() const
    { return const_areverse_iterator(); }
    const_areverse_iterator carbegin() const
    { return const_areverse_iterator(_M_root); }
    const_areverse_iterator carend() const
    { return const_areverse_iterator(); }

};

void test_BinaryTree_main();

}
#endif // BINARYTREE_H
