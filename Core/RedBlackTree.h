#ifndef REDBLACKTREE_H
#define REDBLACKTREE_H

#include "Compare.h"
#include "BinaryTree.h"
#include <initializer_list>

namespace rapid
{

template<typename DataType>
struct RBDataNode
{
    enum class Color : bool
    {
        RED = false,
        BLACK = true
    };
    NodeBase<DataType> *_M_data;
    Color _M_color;

    using ValueType = DataType;
    using Reference = ValueType &;
    using ConstReference = const ValueType &;

    RBDataNode(const DataType &arg)
        : _M_data(new NodeBase<DataType>(arg)), _M_color(Color::RED)
    { }

    void dealloc()
    { delete _M_data; }

    Reference data()
    { return _M_data->ref_content(); }
    Reference data() const
    { return _M_data->ref_content(); }
    Color& color()
    { return _M_color; }

    bool operator<(const RBDataNode& arg)
    { return data() < arg.data(); }
};

template<typename _DataType, typename _Compare = Compare<_DataType>>
class RedBlackTree
{
public:
    using ValueType = _DataType;
    using Reference = ValueType &;
    using ConstReference = const ValueType &;
    using RvalueReference = ValueType &&;
    using Pointer = ValueType *;
    using Self = RedBlackTree;
    using SizeType = size_type;

    using DataNode = RBDataNode<ValueType>;
    using TreeType = BinaryTree<DataNode>;
    using TreeNode = typename TreeType::TreeNode;
    using CompareType = _Compare;

private:
    using FormerIteratorImpl = typename TreeType::fiterator;
    using ConstFormerIteratorImpl = typename TreeType::const_fiterator;
    using MiddleIteratorImpl = typename TreeType::miterator;
    using ConstMiddleIteratorImpl = typename TreeType::const_miterator;
    using AfterIteratorImpl = typename TreeType::aiterator;
    using ConstAfterIteratorImpl = typename TreeType::const_aiterator;

    using IteratorImpl = typename TreeType::iterator;
    using ConstIteratorImpl = typename TreeType::const_iterator;
    using ReverseIteratorImpl = typename TreeType::reverse_iterator;
    using ConstReverseIteratorImpl = typename TreeType::const_reverse_iterator;

public:
    class iterator;
    class const_iterator;
    class reverse_iterator;
    class const_reverse_iterator;

    using miterator = iterator;
    using const_miterator = const_iterator;

    class iterator
    {
    private:
        IteratorImpl _M_it;

        friend class RedBlackTree;

        iterator(IteratorImpl it) : _M_it(it) { }
    public:
        iterator() { }
        iterator(const iterator &it)
            : _M_it(it._M_it) { }
        iterator(iterator &&it)
            : _M_it(forward<iterator>(it)._M_it) { }

        iterator operator++()
        {
            ++_M_it;
            return *this;
        }
        iterator operator++(int)
        {
            iterator it = *this;
            ++_M_it;
            return it;
        }
        iterator operator--()
        {
            --_M_it;
            return *this;
        }
        iterator operator--(int)
        {
            iterator it = *this;
            --_M_it;
            return it;
        }
        iterator operator=(const iterator &it)
        { return _M_it = it._M_it; }
        Reference operator*()
        { return _M_it->data(); }
        Pointer operator->()
        { return &_M_it->data(); }
        bool operator==(const iterator &it) const
        { return _M_it == it._M_it; }
        bool operator!=(const iterator &it) const
        { return _M_it != it._M_it; }
    };
    class const_iterator
    {
    private:
        ConstIteratorImpl _M_it;

        friend class RedBlackTree;

        const_iterator(ConstIteratorImpl it) : _M_it(it) { }
    public:
        const_iterator() { }
        const_iterator(const const_iterator &it)
            : _M_it(it._M_it) { }
        const_iterator(const_iterator &&it)
            : _M_it(forward<const_iterator>(it)._M_it) { }

        const_iterator operator++()
        {
            ++_M_it;
            return *this;
        }
        const_iterator operator++(int)
        {
            const_iterator it = *this;
            ++_M_it;
            return it;
        }
        const_iterator operator--()
        {
            --_M_it;
            return *this;
        }
        const_iterator operator--(int)
        {
            const_iterator it = *this;
            --_M_it;
            return it;
        }
        const_iterator operator=(const const_iterator &it)
        { return _M_it = it._M_it; }
        Reference operator*() const
        { return _M_it->data(); }
        Pointer operator->() const
        { return &_M_it->data(); }
        bool operator==(const const_iterator &it) const
        { return _M_it == it._M_it; }
        bool operator!=(const const_iterator &it) const
        { return _M_it != it._M_it; }
    };
    class reverse_iterator
    {
    private:
        ReverseIteratorImpl _M_it;

        friend class RedBlackTree;

        reverse_iterator(ReverseIteratorImpl it) : _M_it(it) { }
    public:
        reverse_iterator() { }
        reverse_iterator(const reverse_iterator &it)
            : _M_it(it._M_it) { }
        reverse_iterator(reverse_iterator &&it)
            : _M_it(forward<reverse_iterator>(it)._M_it) { }

        reverse_iterator operator++()
        {
            ++_M_it;
            return *this;
        }
        reverse_iterator operator++(int)
        {
            reverse_iterator it = *this;
            ++_M_it;
            return it;
        }
        reverse_iterator operator--()
        {
            --_M_it;
            return *this;
        }
        reverse_iterator operator--(int)
        {
            reverse_iterator it = *this;
            --_M_it;
            return it;
        }
        reverse_iterator operator=(const reverse_iterator &it)
        { return _M_it = it._M_it; }
        Reference operator*()
        { return _M_it->data(); }
        Pointer operator->()
        { return &_M_it->data(); }
        bool operator==(const reverse_iterator &it) const
        { return _M_it == it._M_it; }
        bool operator!=(const reverse_iterator &it) const
        { return _M_it != it._M_it; }
    };
    class const_reverse_iterator
    {
    private:
        ConstReverseIteratorImpl _M_it;

        friend class RedBlackTree;

        const_reverse_iterator(ConstReverseIteratorImpl it) : _M_it(it) { }
    public:
        const_reverse_iterator() { }
        const_reverse_iterator(const const_reverse_iterator &it)
            : _M_it(it._M_it) { }
        const_reverse_iterator(const_reverse_iterator &&it)
            : _M_it(forward<const_reverse_iterator>(it)._M_it) { }

        const_reverse_iterator operator++()
        {
            ++_M_it;
            return *this;
        }
        const_reverse_iterator operator++(int)
        {
            const_reverse_iterator it = *this;
            ++_M_it;
            return it;
        }
        const_reverse_iterator operator--()
        {
            --_M_it;
            return *this;
        }
        const_reverse_iterator operator--(int)
        {
            const_reverse_iterator it = *this;
            --_M_it;
            return it;
        }
        const_reverse_iterator operator=(const const_reverse_iterator &it)
        { return _M_it = it._M_it; }
        Reference operator*() const
        { return _M_it->data(); }
        Pointer operator->() const
        { return &_M_it->data(); }
        bool operator==(const const_reverse_iterator &it) const
        { return _M_it == it._M_it; }
        bool operator!=(const const_reverse_iterator &it) const
        { return _M_it != it._M_it; }
    };

    class fiterator
    {
    private:
        FormerIteratorImpl _M_it;

        friend class RedBlackTree;

        fiterator(FormerIteratorImpl it) : _M_it(it) { }
    public:
        fiterator() { }
        fiterator(const fiterator &it)
            : _M_it(it._M_it) { }
        fiterator(fiterator &&it)
            : _M_it(forward<fiterator>(it)._M_it) { }

        fiterator operator++()
        {
            ++_M_it;
            return *this;
        }
        fiterator operator++(int)
        {
            fiterator it = *this;
            ++_M_it;
            return it;
        }
        fiterator operator--()
        {
            --_M_it;
            return *this;
        }
        fiterator operator--(int)
        {
            fiterator it = *this;
            --_M_it;
            return it;
        }
        fiterator operator=(const fiterator &it)
        { return _M_it = it._M_it; }
        Reference operator*()
        { return _M_it->data(); }
        Pointer operator->()
        { return &_M_it->data(); }
        bool operator==(const fiterator &it) const
        { return _M_it == it._M_it; }
        bool operator!=(const fiterator &it) const
        { return _M_it != it._M_it; }
    };
    class aiterator
    {
    private:
        AfterIteratorImpl _M_it;

        friend class RedBlackTree;

        aiterator(AfterIteratorImpl it) : _M_it(it) { }
    public:
        aiterator() { }
        aiterator(const aiterator &it)
            : _M_it(it._M_it) { }
        aiterator(aiterator &&it)
            : _M_it(forward<aiterator>(it)._M_it) { }

        aiterator operator++()
        {
            ++_M_it;
            return *this;
        }
        aiterator operator++(int)
        {
            aiterator it = *this;
            ++_M_it;
            return it;
        }
        aiterator operator--()
        {
            --_M_it;
            return *this;
        }
        aiterator operator--(int)
        {
            aiterator it = *this;
            --_M_it;
            return it;
        }
        aiterator operator=(const aiterator &it)
        { return _M_it = it._M_it; }
        Reference operator*()
        { return _M_it->data(); }
        Pointer operator->()
        { return &_M_it->data(); }
        bool operator==(const aiterator &it) const
        { return _M_it == it._M_it; }
        bool operator!=(const aiterator &it) const
        { return _M_it != it._M_it; }
    };
    class const_aiterator
    {
    private:
        ConstAfterIteratorImpl _M_it;

        friend class RedBlackTree;

        const_aiterator(ConstAfterIteratorImpl it) : _M_it(it) { }
    public:
        const_aiterator() { }
        const_aiterator(const const_aiterator &it)
            : _M_it(it._M_it) { }
        const_aiterator(const_aiterator &&it)
            : _M_it(forward<const_aiterator>(it)._M_it) { }

        const_aiterator operator++()
        {
            ++_M_it;
            return *this;
        }
        const_aiterator operator++(int)
        {
            const_aiterator it = *this;
            ++_M_it;
            return it;
        }
        const_aiterator operator--()
        {
            --_M_it;
            return *this;
        }
        const_aiterator operator--(int)
        {
            const_aiterator it = *this;
            --_M_it;
            return it;
        }
        const_aiterator operator=(const const_aiterator &it)
        { return _M_it = it._M_it; }
        Reference operator*() const
        { return _M_it->data(); }
        Pointer operator->() const
        { return &_M_it->data(); }
        bool operator==(const const_aiterator &it) const
        { return _M_it == it._M_it; }
        bool operator!=(const const_aiterator &it) const
        { return _M_it != it._M_it; }
    };
    class const_fiterator
    {
    private:
        ConstFormerIteratorImpl _M_it;

        friend class RedBlackTree;

        const_fiterator(ConstFormerIteratorImpl it) : _M_it(it) { }
    public:
        const_fiterator() { }
        const_fiterator(const const_fiterator &it)
            : _M_it(it._M_it) { }
        const_fiterator(const_fiterator &&it)
            : _M_it(forward<const_fiterator>(it)._M_it) { }

        const_fiterator operator++()
        {
            ++_M_it;
            return *this;
        }
        const_fiterator operator++(int)
        {
            const_fiterator it = *this;
            ++_M_it;
            return it;
        }
        const_fiterator operator--()
        {
            --_M_it;
            return *this;
        }
        const_fiterator operator--(int)
        {
            const_fiterator it = *this;
            --_M_it;
            return it;
        }
        const_fiterator operator=(const const_fiterator &it)
        { return _M_it = it._M_it; }
        Reference operator*() const
        { return _M_it->data(); }
        Pointer operator->() const
        { return &_M_it->data(); }
        bool operator==(const const_fiterator &it) const
        { return _M_it == it._M_it; }
        bool operator!=(const const_fiterator &it) const
        { return _M_it != it._M_it; }
    };

private:
    TreeType _M_tree;

    void _F_exchange_color(TreeNode *node1, TreeNode *node2)
    {
        using Color = typename DataNode::Color;
        Color c = node1->data()._M_color;
        node1->data()._M_color = node2->data()._M_color;
        node2->data()._M_color = c;
    }
    void _F_exchange_data(TreeNode *node1, TreeNode *node2)
    {
        auto temp = node1->data()._M_data;
        node1->data()._M_data = node2->data()._M_data;
        node2->data()._M_data = temp;
    }
    // implement other's
    // param[node]: node to be added
    void _F_insert_adjust(TreeNode *node);
    // if node's color is red, do not enter
    // param[node]: node to be deleted
    void _F_erase_adjust(TreeNode *node);

    iterator _F_insert(ConstReference arg);

    template<typename _InputType, typename _CompareType>
    iterator _F_find(const _InputType &arg) const;

    void _F_erase(TreeNode *node);

    DataNode _F_construct_node(ConstReference arg)
    { return DataNode(arg); }
    Reference _F_node_data(TreeNode *node) const
    { return node->data().data(); }
    typename DataNode::Color& _F_node_color(TreeNode *node)
    { return node->data().color(); }
    void _F_release_node(TreeNode *node)
    {
        node->data().dealloc();
        TreeNode *parent = _M_tree.parent(node);
        if(parent == nullptr)
        {
            _M_tree.release(node);
        }
        else if(_M_tree.left_child(parent) == node)
        {
            _M_tree.set_left(parent, nullptr);
        }
        else
        {
            _M_tree.set_right(parent, nullptr);
        }
    }
public:
    RedBlackTree() { }

    RedBlackTree(const Self &tree)
        : _M_tree(tree._M_tree) { }
    RedBlackTree(Self &&tree)
        : _M_tree(forward<Self>(tree)._M_tree) { }
    RedBlackTree(std::initializer_list<ValueType> arg_list)
    { insert(arg_list); }

    bool empty() const
    { return _M_tree.empty(); }
    SizeType size() const
    { return _M_tree.size(); }
    SizeType depth() const
    { return _M_tree.depth(); }

    iterator find(ConstReference arg) const
    { return _F_find<ValueType, CompareType>(arg); }
    iterator find(RvalueReference arg) const
    { return _F_find<ValueType, CompareType>(forward<ValueType>(arg)); }

    template<typename _InputType, typename _CompareType>
    iterator find(_InputType &&arg) const
    { return _F_find<_InputType, _CompareType>(forward<_InputType>(arg)); }
    template<typename _InputType, typename _CompareType>
    iterator find(const _InputType &arg) const
    { return _F_find<_InputType, _CompareType>(arg); }

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
    void erase(ConstReference arg)
    { erase(find(arg)); }
    void erase(RvalueReference arg)
    { erase(find(forward<ValueType>(arg))); }
    void erase(iterator it)
    { _F_erase(_M_tree.tree_node(it._M_it)); }

    iterator insert(ConstReference arg)
    { return _F_insert(arg); }
    iterator insert(RvalueReference arg)
    { return _F_insert(forward<ValueType>(arg)); }

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

    TreeType to_ordinary_tree() const
    { return _M_tree; }
};

template<typename _DataType, typename _Compare>
typename RedBlackTree<_DataType, _Compare>::iterator
    RedBlackTree<_DataType, _Compare>::_F_insert(ConstReference arg)
{
    iterator result = find_and_insert(arg);
    TreeNode *node = _M_tree.tree_node(result._M_it);
    _F_node_data(node) = arg;
    return result;
}

template<typename _DataType, typename _Compare>
template<typename _InputType, typename _CompareType>
typename RedBlackTree<_DataType, _Compare>::iterator
    RedBlackTree<_DataType, _Compare>::_F_find(const _InputType &arg) const
{
    TreeNode *node = _M_tree.root();
    IteratorImpl result;
    while(node != nullptr)
    {
        int res = _CompareType()(arg, _F_node_data(node));
        if(res == 0)
        {
            return iterator(result = node);
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
    return iterator(result);
}


template<typename _DataType, typename _Compare>
void RedBlackTree<_DataType, _Compare>::_F_erase(TreeNode *node)
{
    using Color = typename DataNode::Color;
    while(node != nullptr)
    {
        TreeNode *left_node = _M_tree.left_child(node);
        TreeNode *right_node = _M_tree.right_child(node);
        TreeNode *replace_node;
        if(node->child_size() == 0)
        {
            if(_M_tree.root() == node)
            {
                _M_tree.clear();
            }
            else
            {
                if(_F_node_color(node) == Color::BLACK)
                {
                    _F_erase_adjust(node);
                }
                _F_release_node(node);
            }
            break;
        }
        else if(left_node == nullptr || right_node == nullptr)
        {
            replace_node = left_node == nullptr ? right_node : left_node;
        }
        else
        {
            // find the subsequent node
            replace_node = _M_tree.left_child_under(_M_tree.right_child(node));
        }
        _F_exchange_data(replace_node, node);
        node = replace_node;
    }
}

template<typename _DataType, typename _Compare>
template<typename _InputType, typename _CompareType>
typename RedBlackTree<_DataType, _Compare>::iterator
    RedBlackTree<_DataType, _Compare>::find_and_insert(const _InputType &input)
{
    IteratorImpl result;
    if(empty())
    {
        using Color = typename DataNode::Color;
        TreeNode *node = _M_tree.append_root(input);
        _F_node_color(node) = Color::BLACK;
        return iterator(result = node);
    }
    TreeNode *node = _M_tree.root();
    while(true)
    {
        int res = _CompareType()(input, _F_node_data(node));
        if(res == 0)
        {
            return iterator(result = node);
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
    _F_insert_adjust(node);
    return iterator(result = node);
}



template<typename _DataType, typename _Compare>
void RedBlackTree<_DataType, _Compare>::_F_insert_adjust(TreeNode *node)
{
    using Color = typename DataNode::Color;
    while(node != nullptr)
    {
        TreeNode *parent = _M_tree.parent(node);
        TreeNode *grand_parent = _M_tree.parent(parent);
        TreeNode *uncle = nullptr;
        if(_M_tree.left_child(grand_parent) == parent)
        {
            uncle = _M_tree.right_child(grand_parent);
        }
        else
        {
            uncle = _M_tree.left_child(grand_parent);
        }
        if(parent != nullptr && _F_node_color(parent) == Color::RED)
        {
            if(uncle != nullptr && _F_node_color(uncle) == Color::RED)
            {
                _F_node_color(parent) = Color::BLACK;
                _F_node_color(uncle) = Color::BLACK;
                _F_node_color(grand_parent) = Color::RED;
                node = grand_parent;
                continue;
            }
            if(_M_tree.left_child(parent) == node)
            {
                if(_M_tree.left_child(grand_parent) == parent)
                {
                    _F_exchange_color(parent, grand_parent);
                    _M_tree.right_rotate(grand_parent);
                    break;
                }
                else
                {
                    _M_tree.right_rotate(parent);
                    node = parent;
                    continue;
                }
            }
            else
            {
                if(_M_tree.left_child(grand_parent) == parent)
                {
                    _M_tree.left_rotate(parent);
                    node = parent;
                    continue;
                }
                else
                {
                    _F_exchange_color(parent, grand_parent);
                    _M_tree.left_rotate(grand_parent);
                    continue;
                }
            }
        }
        break;
    }
    _F_node_color(_M_tree.root()) = Color::BLACK;
}

template<typename _DataType, typename _Compare>
void RedBlackTree<_DataType, _Compare>::_F_erase_adjust(TreeNode *node)
{
    using Color = typename DataNode::Color;
    while(node != nullptr)
    {
        TreeNode *parent = _M_tree.parent(node);
        TreeNode *brother, *far_nephew, *near_nephew;
        if(_M_tree.left_child(parent) == node)
        {
            brother = _M_tree.right_child(parent);
            far_nephew = _M_tree.right_child(brother);
            near_nephew = _M_tree.left_child(brother);
        }
        else
        {
            brother = _M_tree.left_child(parent);
            far_nephew = _M_tree.left_child(brother);
            near_nephew = _M_tree.right_child(brother);
        }
        if(brother != nullptr && _F_node_color(brother) == Color::RED)
        {
            _F_node_color(brother) = Color::BLACK;
            _F_node_color(parent) = Color::RED;
            if(_M_tree.left_child(parent) == node)
            {
                _M_tree.left_rotate(parent);
            }
            else
            {
                _M_tree.right_rotate(parent);
            }
        }
        else if((far_nephew == nullptr || _F_node_color(far_nephew) == Color::BLACK) &&
                (near_nephew == nullptr || _F_node_color(near_nephew) == Color::BLACK))
        {
            if(brother != nullptr)
            {
                _F_node_color(brother) = Color::RED;
            }
            node = parent;
        }
        else if(far_nephew == nullptr || _F_node_color(far_nephew) == Color::BLACK)
        {
            _F_exchange_color(brother, near_nephew);
            if(_M_tree.left_child(parent) == node)
            {
                _M_tree.right_rotate(brother);
            }
            else
            {
                _M_tree.left_rotate(brother);
            }
        }
        else
        {
            _F_node_color(brother) = _F_node_color(parent);
            _F_node_color(parent) = Color::BLACK;
            _F_node_color(far_nephew) = Color::BLACK;
            if(_M_tree.left_child(parent) == node)
            {
                _M_tree.left_rotate(parent);
            }
            else
            {
                _M_tree.right_rotate(parent);
            }
            break;
        }
    }
}


};

#endif // REDBLACKTREE_H
