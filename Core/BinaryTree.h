#ifndef BINARYTREE_H
#define BINARYTREE_H

#include "Core/TypeTraits.h"
#include "Core/Version.h"
#include "Core/TLNode.h"
//#include "Core/Stack.h"

namespace rapid
{

template<typename _DataType>
struct BTreeNode
{
    using ValueType = _DataType;
    using SizeType = size_type;
    using ConstReference = const ValueType &;

    NodeBase<ValueType> *_M_data;
    BTreeNode *_M_left = nullptr;
    BTreeNode *_M_right = nullptr;
    BTreeNode *_M_parent = nullptr;
    SizeType _M_child_number = 0;
    SizeType _M_depth = 1;

    ~BTreeNode()
    { delete _M_data; }
    void add_child_number(SizeType size)
    {
        _M_child_number += size;
        if(_M_parent != nullptr)
        { _M_parent->add_child_number(size); }
    }
    void update_depth()
    {
        _M_depth = 1;
        if(_M_left != nullptr)
        {
            _M_depth = _M_left->_M_depth + 1;
        }
        if(_M_right != nullptr)
        {
            SizeType r = _M_right->_M_depth + 1;
            _M_depth = r > _M_depth ? r : _M_depth;
        }
        if(_M_parent != nullptr)
        { _M_parent->update_depth(); }
    }
    template<typename ... Args>
    BTreeNode* append_left(const Args & ... args)
    { return set_left(new BTreeNode<ValueType>(_M_left, nullptr, args...)); }
    template<typename ... Args>
    BTreeNode* append_right(const Args & ... args)
    { return set_right(new BTreeNode<ValueType>(_M_right, nullptr, args...)); }

//    BTreeNode* append_left(ConstReference data)
//    { return set_left(new BTreeNode<ValueType>(_M_left, nullptr, data)); }
//    BTreeNode* append_right(ConstReference data)
//    { return set_right(new BTreeNode<ValueType>(_M_right, nullptr, data)); }

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
        _M_left = node;
        update_depth();
        return _M_left;
    }
    BTreeNode* set_right(BTreeNode *node)
    {
        add_child_number(- (_M_right == nullptr ? 0 : _M_right->_M_child_number + 1));
        if(node != nullptr)
        {
            node->set_parent(this);
            add_child_number(node->_M_child_number + 1);
        }
        _M_right = node;
        update_depth();
        return _M_right;
    }
    // [node] is not nullptr
    void swap(BTreeNode *node)
    {
        NodeBase<ValueType> *temp_data = node->_M_data;
        node->_M_data = _M_data;
        _M_data = temp_data;
    }
    template<typename ... Args>
    BTreeNode(BTreeNode *left, BTreeNode *right, const Args & ... args)
        : _M_data(new NodeBase<ValueType>(args...))
    {
        set_left(left);
        set_right(right);
    }
    ValueType& data()
    { return _M_data->ref_content(); }
    ValueType& data() const
    { return _M_data->ref_content(); }
    SizeType child_size() const
    { return _M_child_number; }
    SizeType depth() const
    { return _M_depth; }
    BTreeNode* left() const
    { return _M_left; }
    BTreeNode* right() const
    { return _M_right; }
    BTreeNode* parent() const
    { return _M_parent; }
};

template<typename _DataType, typename _Node = BTreeNode<_DataType>>
class BinaryTree
{
public:
    using DataType = _DataType;
    using Reference = DataType &;
    using RvalueReference = DataType &&;
    using ConstReference = const DataType &;
    using Pointer = DataType *;
    using SizeType = size_type;
public:
    using TreeNode = _Node;

    static TreeNode* left_child_under(TreeNode *node);
    static TreeNode* right_child_under(TreeNode *node);
    static TreeNode* left_leaves(TreeNode *node);
    static TreeNode* right_leaves(TreeNode *node);
    static TreeNode* former_next(TreeNode *node);
    static TreeNode* middle_next(TreeNode *node);
    static TreeNode* after_next(TreeNode *node);
    static TreeNode* former_previous(TreeNode *node);
    static TreeNode* middle_previous(TreeNode *node);
    static TreeNode* after_previous(TreeNode *node);
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

        friend class BinaryTree;
        ConstFormerIterator _F_const_cast()
        { return ConstFormerIterator(_M_current); }
    public:
        FormerIterator(TreeNode *root)
        { _M_current = root; }
        FormerIterator() { }
        FormerIterator(const FormerIterator &it)
            : _M_current(it._M_current) { }
        FormerIterator(FormerIterator &&it)
            : _M_current(forward<FormerIterator>(it)._M_current) { }

        FormerIterator operator++()
        {
            _M_current = former_next(_M_current);
            return *this;
        }
        FormerIterator operator++(int)
        {
            FormerIterator it = *this;
            _M_current = BinaryTree<_DataType>::former_next(_M_current);
            return it;
        }
        FormerIterator operator--()
        {
            _M_current = former_previous(_M_current);
            return *this;
        }
        FormerIterator operator--(int)
        {
            FormerIterator it = *this;
            _M_current = former_previous(_M_current);
            return it;
        }
        FormerIterator operator=(const FormerIterator &it)
        { return _M_current = it._M_current; }
        FormerIterator operator=(TreeNode *node)
        {
            _M_current = node;
            return *this;
        }
        Reference operator*()
        { return _M_current->data(); }
        Pointer operator->()
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

        friend class BinaryTree;
        FormerIterator _F_const_cast()
        { return FormerIterator(const_cast<TreeNode *>(_M_current)); }
    public:
        ConstFormerIterator(TreeNode *root)
        { _M_current = root; }
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
            _M_current = former_next(const_cast<TreeNode*>(_M_current));
            return *this;
        }
        ConstFormerIterator operator++(int)
        {
            ConstFormerIterator it = *this;
            _M_current = former_next(const_cast<TreeNode*>(_M_current));
            return it;
        }
        ConstFormerIterator operator--()
        {
            _M_current = former_previous(const_cast<TreeNode*>(_M_current));
            return *this;
        }
        ConstFormerIterator operator--(int)
        {
            ConstFormerIterator it = *this;
            _M_current = former_previous(const_cast<TreeNode*>(_M_current));
            return it;
        }
        ConstFormerIterator operator=(const ConstFormerIterator &it)
        { return _M_current = it._M_current; }
        ConstFormerIterator operator=(TreeNode *node)
        {
            _M_current = node;
            return *this;
        }
        Reference operator*() const
        { return _M_current->data(); }
        Pointer operator->() const
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

        friend class BinaryTree;
        ConstMiddleIterator _F_const_cast()
        { return ConstMiddleIterator(_M_current); }
    public:
        MiddleIterator(TreeNode *root)
        { _M_current = left_child_under(root); }
        MiddleIterator() { }
        MiddleIterator(const MiddleIterator &it)
            : _M_current(it._M_current) { }
        MiddleIterator(MiddleIterator &&it)
            : _M_current(forward<MiddleIterator>(it)._M_current) { }

        MiddleIterator operator++()
        {
            _M_current = middle_next(_M_current);
            return *this;
        }
        MiddleIterator operator++(int)
        {
            MiddleIterator it = *this;
            _M_current = middle_next(_M_current);
            return it;
        }
        MiddleIterator operator--()
        {
            _M_current = middle_previous(_M_current);
            return *this;
        }
        MiddleIterator operator--(int)
        {
            MiddleIterator it = *this;
            _M_current = middle_previous(_M_current);
            return it;
        }
        MiddleIterator operator=(const MiddleIterator &it)
        { return _M_current = it._M_current; }
        MiddleIterator operator=(TreeNode *node)
        {
            _M_current = node;
            return *this;
        }
        Reference operator*()
        { return _M_current->data(); }
        Pointer operator->()
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

        friend class BinaryTree;
        MiddleIterator _F_const_cast()
        { return MiddleIterator(const_cast<TreeNode *>(_M_current)); }
    public:
        ConstMiddleIterator(TreeNode *root)
        { _M_current = left_child_under(root); }
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
            _M_current = middle_next(const_cast<TreeNode*>(_M_current));
            return *this;
        }
        ConstMiddleIterator operator++(int)
        {
            ConstMiddleIterator it = *this;
            _M_current = middle_next(const_cast<TreeNode*>(_M_current));
            return it;
        }
        ConstMiddleIterator operator--()
        {
            _M_current = middle_previous(const_cast<TreeNode*>(_M_current));
            return *this;
        }
        ConstMiddleIterator operator--(int)
        {
            ConstMiddleIterator it = *this;
            _M_current = middle_previous(const_cast<TreeNode*>(_M_current));
            return it;
        }
        ConstMiddleIterator operator=(const ConstMiddleIterator &it)
        { return _M_current = it._M_current; }
        ConstMiddleIterator operator=(TreeNode *node)
        {
            _M_current = node;
            return *this;
        }
        Reference operator*() const
        { return _M_current->data(); }
        Pointer operator->() const
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

        friend class BinaryTree;
        ConstAfterIterator _F_const_cast()
        { return ConstAfterIterator(_M_current); }
    public:
        AfterIterator(TreeNode *root)
        { _M_current = left_leaves(root); }
        AfterIterator() { }
        AfterIterator(const AfterIterator &it)
            : _M_current(it._M_current) { }
        AfterIterator(AfterIterator &&it)
            : _M_current(forward<AfterIterator>(it)._M_current) { }

        AfterIterator operator++()
        {
            _M_current = after_next(_M_current);
            return *this;
        }
        AfterIterator operator++(int)
        {
            AfterIterator it = *this;
            _M_current = after_next(_M_current);
            return it;
        }
        AfterIterator operator--()
        {
            _M_current = after_previous(_M_current);
            return *this;
        }
        AfterIterator operator--(int)
        {
            AfterIterator it = *this;
            _M_current = after_previous(_M_current);
            return it;
        }
        AfterIterator operator=(const AfterIterator &it)
        { return _M_current = it._M_current; }
        AfterIterator operator=(TreeNode *node)
        {
            _M_current = node;
            return *this;
        }
        Reference operator*()
        { return _M_current->data(); }
        Pointer operator->()
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

        friend class BinaryTree;
        AfterIterator _F_const_cast()
        { return AfterIterator(const_cast<TreeNode *>(_M_current)); }
    public:
        ConstAfterIterator(TreeNode *root)
        { _M_current = left_leaves(root); }
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
            _M_current = after_next(const_cast<TreeNode*>(_M_current));
            return *this;
        }
        ConstAfterIterator operator++(int)
        {
            ConstAfterIterator it = *this;
            _M_current = after_next(const_cast<TreeNode*>(_M_current));
            return it;
        }
        ConstAfterIterator operator--()
        {
            _M_current = after_previous(const_cast<TreeNode*>(_M_current));
            return *this;
        }
        ConstAfterIterator operator--(int)
        {
            ConstAfterIterator it = *this;
            _M_current = after_previous(const_cast<TreeNode*>(_M_current));
            return it;
        }
        ConstAfterIterator operator=(const ConstAfterIterator &it)
        { return _M_current = it._M_current; }
        ConstAfterIterator operator=(TreeNode *node)
        {
            _M_current = node;
            return *this;
        }
        Reference operator*() const
        { return _M_current->data(); }
        Pointer operator->() const
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

        ConstReverseFormerIterator _F_const_cast()
        { return ConstReverseFormerIterator(_M_current); }
        friend class BinaryTree;
    public:
        ReverseFormerIterator(TreeNode *root)
        { _M_current = right_leaves(root); }
        ReverseFormerIterator() { }
        ReverseFormerIterator(const ReverseFormerIterator &it)
            : _M_current(it._M_current) { }
        ReverseFormerIterator(ReverseFormerIterator &&it)
            : _M_current(forward<ReverseFormerIterator>(it)._M_current) { }

        ReverseFormerIterator operator++()
        {
            _M_current = former_previous(_M_current);
            return *this;
        }
        ReverseFormerIterator operator++(int)
        {
            ReverseFormerIterator it = *this;
            _M_current = former_previous(_M_current);
            return it;
        }
        ReverseFormerIterator operator--()
        {
            _M_current = former_next(_M_current);
            return *this;
        }
        ReverseFormerIterator operator--(int)
        {
            ReverseFormerIterator it = *this;
            _M_current = former_next(_M_current);
            return it;
        }
        ReverseFormerIterator operator=(const ReverseFormerIterator &it)
        { return _M_current = it._M_current; }
        ReverseFormerIterator operator=(TreeNode *node)
        {
            _M_current = node;
            return *this;
        }
        Reference operator*()
        { return _M_current->data(); }
        Pointer operator->()
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
    public:
        ConstReverseFormerIterator(TreeNode *root)
        { _M_current = right_leaves(root); }
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
            _M_current = former_previous(const_cast<TreeNode*>(_M_current));
            return *this;
        }
        ConstReverseFormerIterator operator++(int)
        {
            ConstReverseFormerIterator it = *this;
            _M_current = former_previous(const_cast<TreeNode*>(_M_current));
            return it;
        }
        ConstReverseFormerIterator operator--()
        {
            _M_current = former_next(const_cast<TreeNode*>(_M_current));
            return *this;
        }
        ConstReverseFormerIterator operator--(int)
        {
            ConstReverseFormerIterator it = *this;
            _M_current = former_next(const_cast<TreeNode*>(_M_current));
            return it;
        }
        ConstReverseFormerIterator operator=(const ConstReverseFormerIterator &it)
        { return _M_current = it._M_current; }
        ConstReverseFormerIterator operator=(TreeNode *node)
        {
            _M_current = node;
            return *this;
        }
        Reference operator*() const
        { return _M_current->data(); }
        Pointer operator->() const
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

        friend class BinaryTree;
        ConstReverseMiddleIterator _F_const_cast()
        { return ConstReverseMiddleIterator(_M_current); }
    public:
        ReverseMiddleIterator(TreeNode *root)
        { _M_current = right_child_under(root); }
        ReverseMiddleIterator() { }
        ReverseMiddleIterator(const ReverseMiddleIterator &it)
            : _M_current(it._M_current) { }
        ReverseMiddleIterator(ReverseMiddleIterator &&it)
            : _M_current(forward<ReverseMiddleIterator>(it)._M_current) { }

        ReverseMiddleIterator operator++()
        {
            _M_current = middle_previous(_M_current);
            return *this;
        }
        ReverseMiddleIterator operator++(int)
        {
            ReverseMiddleIterator it = *this;
            _M_current = middle_previous(_M_current);
            return it;
        }
        ReverseMiddleIterator operator--()
        {
            _M_current = middle_next(_M_current);
            return *this;
        }
        ReverseMiddleIterator operator--(int)
        {
            ReverseMiddleIterator it = *this;
            _M_current = middle_next(_M_current);
            return it;
        }
        ReverseMiddleIterator operator=(const ReverseMiddleIterator &it)
        { return _M_current = it._M_current; }
        ReverseMiddleIterator operator=(TreeNode *node)
        {
            _M_current = node;
            return *this;
        }
        Reference operator*()
        { return _M_current->data(); }
        Pointer operator->()
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

        friend class BinaryTree;
        ReverseMiddleIterator _F_const_cast()
        { return ReverseMiddleIterator(const_cast<TreeNode *>(_M_current)); }
    public:
        ConstReverseMiddleIterator(TreeNode *root)
        { _M_current = right_child_under(root); }
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
            _M_current = middle_previous(const_cast<TreeNode*>(_M_current));
            return *this;
        }
        ConstReverseMiddleIterator operator++(int)
        {
            ConstReverseMiddleIterator it = *this;
            _M_current = middle_previous(const_cast<TreeNode*>(_M_current));
            return it;
        }
        ConstReverseMiddleIterator operator--()
        {
            _M_current = middle_next(const_cast<TreeNode*>(_M_current));
            return *this;
        }
        ConstReverseMiddleIterator operator--(int)
        {
            ConstReverseMiddleIterator it = *this;
            _M_current = middle_next(const_cast<TreeNode*>(_M_current));
            return it;
        }
        ConstReverseMiddleIterator operator=(const ConstReverseMiddleIterator &it)
        { return _M_current = it._M_current; }
        ConstReverseMiddleIterator operator=(TreeNode *node)
        {
            _M_current = node;
            return *this;
        }
        Reference operator*() const
        { return _M_current->data(); }
        Pointer operator->() const
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

        friend class BinaryTree;
        ConstReverseAfterIterator _F_const_cast()
        { return ConstReverseAfterIterator(_M_current); }
    public:
        ReverseAfterIterator(TreeNode *root)
        { _M_current = root; }
        ReverseAfterIterator() { }
        ReverseAfterIterator(const ReverseAfterIterator &it)
            : _M_current(it._M_current) { }
        ReverseAfterIterator(ReverseAfterIterator &&it)
            : _M_current(forward<ReverseAfterIterator>(it)._M_current) { }

        ReverseAfterIterator operator++()
        {
            _M_current = after_previous(_M_current);
            return *this;
        }
        ReverseAfterIterator operator++(int)
        {
            ReverseAfterIterator it = *this;
            _M_current = after_previous(_M_current);
            return it;
        }
        ReverseAfterIterator operator--()
        {
            _M_current = after_next(_M_current);
            return *this;
        }
        ReverseAfterIterator operator--(int)
        {
            ReverseAfterIterator it = *this;
            _M_current = after_next(_M_current);
            return it;
        }
        ReverseAfterIterator operator=(const ReverseAfterIterator &it)
        { return _M_current = it._M_current; }
        ReverseAfterIterator operator=(TreeNode *node)
        {
            _M_current = node;
            return *this;
        }
        Reference operator*()
        { return _M_current->data(); }
        Pointer operator->()
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

        friend class BinaryTree;
        ReverseAfterIterator _F_const_cast()
        { return ReverseAfterIterator(const_cast<TreeNode *>(_M_current)); }
    public:
        ConstReverseAfterIterator(TreeNode *root)
        { _M_current = root; }
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
            _M_current = after_previous(const_cast<TreeNode*>(_M_current));
            return *this;
        }
        ConstReverseAfterIterator operator++(int)
        {
            ConstReverseAfterIterator it = *this;
            _M_current = after_previous(const_cast<TreeNode*>(_M_current));
            return it;
        }
        ConstReverseAfterIterator operator--()
        {
            _M_current = after_next(const_cast<TreeNode*>(_M_current));
            return *this;
        }
        ConstReverseAfterIterator operator--(int)
        {
            ConstReverseAfterIterator it = *this;
            _M_current = after_next(const_cast<TreeNode*>(_M_current));
            return it;
        }
        ConstReverseAfterIterator operator=(const ConstReverseAfterIterator &it)
        { return _M_current = it._M_current; }
        ConstReverseAfterIterator operator=(TreeNode *node)
        {
            _M_current = node;
            return *this;
        }
        Reference operator*() const
        { return _M_current->data(); }
        Pointer operator->() const
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

private:
    TreeNode *_M_root = nullptr;

    void _F_copy(const BinaryTree &tree);

    void _F_copy_tree(TreeNode *src, TreeNode *dst);

    template<typename ... Args>
    TreeNode* _F_construct_node(TreeNode *left, TreeNode *right, const Args & ... args)
    { return new TreeNode(left, right, args...); }

public:
    BinaryTree() { }
    BinaryTree(const BinaryTree &tree)
    { _F_copy(tree); }
    BinaryTree(BinaryTree &&tree)
    { _F_copy(forward<BinaryTree>(tree)); }
    ~BinaryTree()
    { clear(); }

    BinaryTree& operator=(const BinaryTree &tree)
    {
        _F_copy(tree);
        return *this;
    }
    TreeNode* root() const
    { return _M_root; }
    template<typename ... Args>
    TreeNode* append_root(const Args & ... args)
    { return _M_root = _F_construct_node(_M_root, nullptr, args...); }
    template<typename ... Args>
    TreeNode* append_root(Args && ... args)
    { return _M_root = _F_construct_node(_M_root, nullptr, forward<Args>(args)...); }

    SizeType size() const
    { return _M_root == nullptr ? 0 : (_M_root->child_size() + 1); }
    bool empty() const
    { return _M_root == nullptr; }
    SizeType depth() const
    { return depth(root()); }
    void clear()
    {
        release(root());
        _M_root = nullptr;
    }
    TreeNode *tree_node(iterator it)
    { return it._M_current; }

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
    TreeNode* left_rotate(TreeNode *node);
    TreeNode* right_rotate(TreeNode *node);

    /* param[node]: node to be erased
     * return:
     *     1. [node]'s left depth > [node]'s right depth, return the parent of
     *        the "max"(in avl tree) of [node]'s left child
     *     2. [node]'s left depth <= [node]'s right depth, return the parent of
     *        the "min"(in avl tree) of [node]'s right child
     */
    TreeNode* erase(TreeNode *node);

    static void release(TreeNode *node)
    {
        if(node == nullptr) return;
        release(left_child(node));
        release(right_child(node));
        delete node;
    }
    static SizeType depth(TreeNode *node)
    { return node == nullptr ? 0 : node->depth(); }
    static TreeNode* left_child(const TreeNode *node)
    { return node == nullptr ? nullptr : node->left(); }
    static TreeNode* right_child(const TreeNode *node)
    { return node == nullptr ? nullptr : node->right(); }
    static TreeNode* parent(const TreeNode *node)
    { return node == nullptr ? nullptr : node->parent(); }
    static TreeNode* set_left(TreeNode *node, TreeNode *child)
    {
        if(node == nullptr) return nullptr;
        release(left_child(node));
        return node->set_left(child);
    }
    static TreeNode* set_right(TreeNode *node, TreeNode *child)
    {
        if(node == nullptr) return nullptr;
        release(right_child(node));
        return node->set_right(child);
    }
    template<typename ... Args>
    static TreeNode* append_left(TreeNode *node, const Args & ... args)
    { return node == nullptr ? nullptr : node->append_left(args...); }
    template<typename ... Args>
    static TreeNode* append_left(TreeNode *node, Args && ... args)
    { return node == nullptr ? nullptr : node->append_left(forward<Args>(args)...); }
    template<typename ... Args>
    static TreeNode* append_right(TreeNode *node, const Args & ... args)
    { return node == nullptr ? nullptr : node->append_right(args...); }
    template<typename ... Args>
    static TreeNode* append_right(TreeNode *node, Args && ... args)
    { return node == nullptr ? nullptr : node->append_right(forward<Args>(args)...); }

    static void remove(TreeNode *node)
    { release(node); }
    static void remove_left(TreeNode *node)
    { set_left(node, nullptr); }
    static void remove_right(TreeNode *node)
    { set_right(node, nullptr); }
    static TreeNode* erase_left(TreeNode *node)
    {
        if(node == nullptr) return nullptr;
        TreeNode *result = left_child(node);
        node->set_left(nullptr);
        return result;
    }
    static TreeNode* erase_right(TreeNode *node)
    {
        if(node == nullptr) return nullptr;
        TreeNode *result = right_child(node);
        node->set_right(nullptr);
        return result;
    }
    static bool is_brother(const TreeNode *node1, const TreeNode *node2)
    { return parent(node1) == parent(node2) && node1 != node2; }
    static SizeType left_child_size(TreeNode *node)
    { return node == nullptr ? 0 : (left_child(node) == nullptr ? 0 : (left_child(node)->child_size() + 1)); }
    static SizeType right_child_size(TreeNode *node)
    { return node == nullptr ? 0 : (right_child(node) == nullptr ? 0 : (right_child(node)->child_size() + 1)); }

};
//-----------------------impl-----------------------//
//-----------------------impl-----------------------//
//-----------------------impl-----------------------//
//-----------------------impl-----------------------//
//-----------------------impl-----------------------//

template<typename _DataType, typename _Node>
void BinaryTree<_DataType, _Node>::_F_copy_tree(TreeNode *src, TreeNode *dst)
{
    if(left_child(dst) != nullptr)
    {
        append_left(src, left_child(dst)->data());
        _F_copy_tree(left_child(src), left_child(dst));
    }
    if(right_child(dst) != nullptr)
    {
        append_right(src, right_child(dst)->data());
        _F_copy_tree(right_child(src), right_child(dst));
    }
}

template<typename _DataType, typename _Node>
void BinaryTree<_DataType, _Node>::_F_copy(const BinaryTree &tree)
{
    clear();
    if(tree.empty())
    { return; }
    TreeNode *root = const_cast<TreeNode *>(tree.root());
    append_root(root->data());
    _F_copy_tree(_M_root, root);
    //  the follow way has no problem
//    Stack<TreeNode*> src, dst;
//    for(auto it = tree.fbegin(); it != tree.fend(); ++it)
//    {
//        TreeNode *new_node = nullptr;
//        if(src.size() == 0)
//        {
//            new_node = append_root(*it);
//        }
//        if(src.size() > 0)
//        {
//            while(parent(it._M_current) != src.top())
//            {
//                src.pop();
//                dst.pop();
//            }
//            if(left_child(src.top()) == it._M_current)
//            {
//                new_node = append_left(dst.top(), *it);
//            }
//            else if(right_child(src.top()) == it._M_current)
//            {
//                new_node = append_right(dst.top(), *it);
//            }
//        }
//        src.push(const_cast<TreeNode*>(it._M_current));
//        dst.push(new_node);
//    }
}
template<typename _DataType, typename _Node>
typename BinaryTree<_DataType, _Node>::TreeNode*
    BinaryTree<_DataType, _Node>::right_rotate(TreeNode *node)
{
    TreeNode *left_node = left_child(node);
    if(left_node == nullptr) return nullptr;
    TreeNode *original_parent = parent(node);
    TreeNode *left_node_right_child = right_child(left_node);
    if(original_parent != nullptr)
    {
        if(right_child(original_parent) == node)
        {
            original_parent->_M_right = left_node;
        }
        else
        {
            original_parent->_M_left = left_node;
        }
    }
    node->_M_parent = nullptr;
    left_node->_M_parent = nullptr;

    SizeType left_node_right_child_size = left_node_right_child == nullptr ? 0 : (left_node_right_child->child_size() + 1);

    node->add_child_number(- (left_node->child_size() + 1));
    node->add_child_number(left_node_right_child_size);

    left_node->add_child_number(-left_node_right_child_size);
    left_node->add_child_number(node->child_size() + 1);

    node->_M_parent = left_node;
    node->_M_left = left_node_right_child;
    left_node->_M_right = node;
    left_node->_M_parent = original_parent;
    if(left_node_right_child != nullptr)
    { left_node_right_child->_M_parent = node; }

    if(node == root())
    { _M_root = left_node; }

    node->update_depth();
    return left_node;
}

template<typename _DataType, typename _Node>
typename BinaryTree<_DataType, _Node>::TreeNode*
    BinaryTree<_DataType, _Node>::left_rotate(TreeNode *node)
{
    TreeNode *right_node = right_child(node);
    if(right_node == nullptr) return nullptr;
    TreeNode *original_parent = parent(node);
    TreeNode *right_node_left_child = left_child(right_node);
    if(original_parent != nullptr)
    {
        if(right_child(original_parent) == node)
        {
            original_parent->_M_right = right_node;
        }
        else
        {
            original_parent->_M_left = right_node;
        }
    }
    node->_M_parent = nullptr;
    right_node->_M_parent = nullptr;

    SizeType left_node_right_child_size = right_node_left_child == nullptr ? 0 : (right_node_left_child->child_size() + 1);

    node->add_child_number(- (right_node->child_size() + 1));
    node->add_child_number(left_node_right_child_size);

    right_node->add_child_number(-left_node_right_child_size);
    right_node->add_child_number(node->child_size() + 1);

    node->_M_parent = right_node;
    node->_M_right = right_node_left_child;
    right_node->_M_left = node;
    right_node->_M_parent = original_parent;
    if(right_node_left_child != nullptr)
    { right_node_left_child->_M_parent = node; }

    if(node == root())
    { _M_root = right_node; }

    node->update_depth();
    return right_node;
}

//---------------------***************---------------------//
template<typename _DataType, typename _Node>
typename BinaryTree<_DataType, _Node>::TreeNode*
    BinaryTree<_DataType, _Node>::left_child_under(TreeNode *node)
{
    using BT = BinaryTree<_DataType>;
    while(node != nullptr && BT::left_child(node) != nullptr)
    {
        node = BT::left_child(node);
    }
    return node;
}
template<typename _DataType, typename _Node>
typename BinaryTree<_DataType, _Node>::TreeNode*
    BinaryTree<_DataType, _Node>::right_child_under(TreeNode *node)
{
    using BT = BinaryTree<_DataType>;
    while(node != nullptr && BT::right_child(node) != nullptr)
    {
        node = BT::right_child(node);
    }
    return node;
}
template<typename _DataType, typename _Node>
typename BinaryTree<_DataType, _Node>::TreeNode*
    BinaryTree<_DataType, _Node>::left_leaves(TreeNode *node)
{
    using BT = BinaryTree<_DataType>;
    node = left_child_under(node);
    while(BT::right_child(node) != nullptr)
    {
        node = left_child_under(BT::right_child(node));
    }
    return node;
}
template<typename _DataType, typename _Node>
typename BinaryTree<_DataType, _Node>::TreeNode*
    BinaryTree<_DataType, _Node>::right_leaves(TreeNode *node)
{
    using BT = BinaryTree<_DataType>;
    node = right_child_under(node);
    while(BT::left_child(node) != nullptr)
    {
        node = right_child_under(BT::left_child(node));
    }
    return node;
}

template<typename _DataType, typename _Node>
typename BinaryTree<_DataType, _Node>::TreeNode*
    BinaryTree<_DataType, _Node>::former_next(TreeNode *current)
{
    using namespace rapid;
    using BT = BinaryTree<_DataType>;
    using Node = typename BT::TreeNode;
    if(BT::left_child(current) != nullptr)
    {
        return BT::left_child(current);
    }
    if(BT::right_child(current) != nullptr)
    {
        return BT::right_child(current);
    }
    Node *temp = current;
    current = BT::parent(current);
    while(current != nullptr && (BT::right_child(current) == temp || BT::right_child(current) == nullptr))
    {
        temp = current;
        current = BT::parent(current);
    }
    return BT::right_child(current);
}
template<typename _DataType, typename _Node>
typename BinaryTree<_DataType, _Node>::TreeNode*
    BinaryTree<_DataType, _Node>::middle_next(TreeNode *current)
{
    using namespace rapid;
    using BT = BinaryTree<_DataType>;
    using Node = typename BT::TreeNode;
    if(BT::right_child(current) != nullptr)
    {
        return left_child_under(BT::right_child(current));
    }
    if(current != BT::right_child(BT::parent(current)))
    {
        return BT::parent(current);
    }
    Node *temp = current;
    current = BT::parent(current);
    while(current != nullptr && BT::right_child(current) == temp)
    {
        temp = current;
        current = BT::parent(current);
    }
    return current;
}

template<typename _DataType, typename _Node>
typename BinaryTree<_DataType, _Node>::TreeNode*
    BinaryTree<_DataType, _Node>::after_next(TreeNode *current)
{
    using namespace rapid;
    using BT = BinaryTree<_DataType>;
    if(current == BT::left_child(BT::parent(current)))
    {
        if(BT::right_child(BT::parent(current)) == nullptr)
        {
            return BT::parent(current);
        }
        return left_leaves(BT::right_child(BT::parent(current)));
    }
    if(current == BT::right_child(BT::parent(current)))
    {
        return BT::parent(current);
    }
    return nullptr;
}

template<typename _DataType, typename _Node>
typename BinaryTree<_DataType, _Node>::TreeNode*
    BinaryTree<_DataType, _Node>::former_previous(TreeNode *current)
{
    using namespace rapid;
    using BT = BinaryTree<_DataType>;
    if(current == BT::left_child(BT::parent(current)))
    {
        return BT::parent(current);
    }
    if(current == BT::right_child(BT::parent(current)))
    {
        current = BT::parent(current);
        if(BT::left_child(current) != nullptr)
        {
            return right_leaves(BT::left_child(current));
        }
        return current;
    }
    return nullptr;
}
template<typename _DataType, typename _Node>
typename BinaryTree<_DataType, _Node>::TreeNode*
    BinaryTree<_DataType, _Node>::middle_previous(TreeNode *current)
{
    using namespace rapid;
    using BT = BinaryTree<_DataType>;
    if(BT::left_child(current) != nullptr)
    {
        return right_child_under(BT::left_child(current));
    }
    if(current == BT::right_child(BT::parent(current)))
    {
        return BT::parent(current);
    }
    while(current != nullptr && current == BT::left_child(BT::parent(current)))
    {
        current = BT::parent(current);
    }
    return BT::parent(current);
}

template<typename _DataType, typename _Node>
typename BinaryTree<_DataType, _Node>::TreeNode*
    BinaryTree<_DataType, _Node>::after_previous(TreeNode *current)
{
    using namespace rapid;
    using BT = BinaryTree<_DataType>;
    if(BT::right_child(current) != nullptr)
    {
        return BT::right_child(current);
    }
    if(BT::right_child(BT::parent(current)) == current)
    {
        current = BT::parent(current);
        while(current != nullptr && BT::left_child(current) == nullptr)
        {
            current = BT::parent(current);
        }
        return BT::left_child(current);
    }
    if(BT::left_child(current) != nullptr)
    {
        return BT::left_child(current);
    }
    while(current != nullptr && BT::left_child(BT::parent(current)) == current)
    {
        current = BT::parent(current);
    }
    current = BT::parent(current);
    while(current != nullptr && BT::left_child(current) == nullptr)
    {
        current = BT::parent(current);
    }
    return BT::left_child(current);
}
//---------------------***************---------------------//
//---------------------------------------------------------//
template<typename _DataType, typename _Node>
typename BinaryTree<_DataType, _Node>::TreeNode*
    BinaryTree<_DataType, _Node>::erase(TreeNode *node)
{
    if(node == nullptr) return nullptr;

    TreeNode *left = left_child(node);
    TreeNode *right = right_child(node);
    if(right->depth() > left->depth())
    {
        if(left == nullptr)
        {
            TreeNode *node_parent = parent(node);
            if(node_parent == nullptr)
            {
                return _M_root = right;
            }
            if(left_child(node_parent) == node)
            {
                return node_parent->set_left(right);
            }
            return node_parent->set_right(right);
        }
        // get min
        TreeNode *reserve = left_child_under(right);
        TreeNode *reserve_parent = parent(reserve);
        TreeNode *temp = right_child(reserve);
        reserve_parent->set_left(temp);
        if(temp == nullptr)
        {
            temp = reserve_parent;
        }
        node->swap(reserve);
        delete reserve;
        return temp;
    }
    else
    {
        if(right == nullptr)
        {
            TreeNode *node_parent = parent(node);
            if(node_parent == nullptr)
            {
                return _M_root = left;
            }
            if(left_child(node_parent) == node)
            {
                return node_parent->set_left(left);
            }
            return node_parent->set_right(left);
        }
        // get max
        TreeNode *reserve = right_child_under(left);
        TreeNode *reserve_parent = parent(reserve);
        TreeNode *temp = left_child(reserve);
        reserve_parent->set_right(temp);
        if(temp == nullptr)
        {
            temp = reserve_parent;
        }
        node->swap(reserve);
        delete reserve;
        return temp;
    }
}

}
#endif // BINARYTREE_H
