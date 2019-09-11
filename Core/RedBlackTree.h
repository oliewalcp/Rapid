#ifndef REDBLACKTREE_H
#define REDBLACKTREE_H

#include "Tree.h"
#include "BinaryTree.h"
#include <initializer_list>

namespace rapid
{

template<typename _DataType, typename _Compare = Compare<_DataType>>
class RedBlackTree;

template<typename DataType>
struct RBDataNode
{
private:
    enum class Color : bool
    {
        RED = false,
        BLACK = true
    };
    NodeBase<DataType> *_M_data;
    Color _M_color;

    friend class RedBlackTree<DataType>;

public:
    using ValueType = DataType;
    using Reference = ValueType &;
    using ConstReference = const ValueType &;

    RBDataNode(const DataType &arg)
        : _M_data(new NodeBase<DataType>(arg)), _M_color(Color::RED)
    { }

    void dealloc()
    { delete _M_data; }

    Reference data() const
    { return _M_data->ref_content(); }
    Color& color() const
    { return _M_color; }

    bool operator<(const RBDataNode& arg)
    { return data() < arg.data(); }
};

template<typename _DataType, typename _Compare>
class RedBlackTree
{
public:
    using ValueType = _DataType;
    using Reference = ValueType &;
    using ConstReference = const ValueType &;
    using RvalueReference = ValueType &&;
    using Self = RedBlackTree;
    using SizeType = size_type;
private:
    using DataNode = RBDataNode<ValueType>;
    using TreeNode = BTreeNode<DataNode>;
    using TreeType = BinaryTree<ValueType, TreeNode>;
    using CompareType = _Compare;
public:
    using iterator = typename TreeType::iterator;
    using const_iterator = typename TreeType::const_iterator;
    using reverse_iterator = typename TreeType::reverse_iterator;
    using const_reverse_iterator = typename TreeType::const_reverse_iterator;
private:
    TreeType _M_tree;

    // implement other's
    // param[node]: node to be added
    void _F_insert_adjust(TreeNode *node);
    // if node's color is red, do not enter
    // param[node]: node to be deleted
    void _F_erase_adjust(TreeNode *node);

    iterator _F_insert(ConstReference arg);
    iterator _F_find(ConstReference arg);
    void _F_erase(TreeNode *node);

    DataNode _F_construct_node(ConstReference arg)
    { return DataNode(arg); }
    Reference _F_node_data(TreeNode *node)
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
        : _M_tree(TreeType(tree._M_tree)) { }
    RedBlackTree(Self &&tree)
        : _M_tree(TreeType(forward<Self>(tree)._M_tree)) { }
    RedBlackTree(std::initializer_list<ValueType> arg_list)
    { insert(arg_list); }

    bool empty() const
    { return _M_tree.empty(); }
    SizeType size() const
    { return _M_tree.size(); }
    SizeType depth() const
    { return _M_tree.depth(); }

    iterator find(ConstReference arg) const
    { return _F_find(arg); }
    iterator find(RvalueReference arg) const
    { return _F_find(forward<ValueType>(arg)); }

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
    { _F_erase(_M_tree.tree_node(it)); }

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

};

template<typename _DataType, typename _Compare>
typename RedBlackTree<_DataType, _Compare>::iterator
    RedBlackTree<_DataType, _Compare>::_F_insert(ConstReference arg)
{
    iterator result;
    DataNode dn = _F_construct_node(arg);
    if(empty())
    {
        return result = _M_tree.append_root(dn);
    }
    TreeNode *node = _M_tree.root();
    while(true)
    {
        int res = CompareType()(arg, _F_node_data(node));
        if(res == 0)
        {
            _F_node_data(node) = arg;
            return result = node;
        }
        if(res > 0)
        {
            if(_M_tree.left_child(node) == nullptr)
            {
                node = _M_tree.append_left(node, dn);
                break;
            }
            node = _M_tree.left_child(node);
        }
        else
        {
            if(_M_tree.right_child(node) == nullptr)
            {
                node = _M_tree.append_right(node, dn);
                break;
            }
            node = _M_tree.right_child(node);
        }
    }
    _F_insert_adjust(node);
    return result = node;
}

template<typename _DataType, typename _Compare>
typename RedBlackTree<_DataType, _Compare>::iterator
    RedBlackTree<_DataType, _Compare>::_F_find(ConstReference arg)
{
    TreeNode *node = _M_tree.root();
    iterator result;
    while(true)
    {
        int res = CompareType()(arg, _F_node_data(node));
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

template<typename _DataType, typename _Compare>
void RedBlackTree<_DataType, _Compare>::_F_erase(TreeNode *node)
{
    using Color = typename DataNode::Color;
    TreeNode *replace_node = _M_tree.left_child_under(_M_tree.right_child(node));
    node->swap(replace_node);
    if(_F_node_color(replace_node) != Color::RED)
    {
        _F_erase_adjust(replace_node);
    }
    _F_release_node(replace_node);
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
            if(_M_tree.right_child(parent) == node)
            {
                _F_node_color(parent) = Color::BLACK;
                _F_node_color(grand_parent) = Color::RED;
                if(_M_tree.left_child(grand_parent) == parent)
                {
                    _M_tree.right_rotate(grand_parent);
                }
                else
                {
                    _M_tree.left_rotate(grand_parent);
                }
                break;
            }
            else
            {
                if(_M_tree.left_child(grand_parent) == parent)
                {
                    _M_tree.left_rotate(parent);
                }
                else
                {
                    _M_tree.right_rotate(parent);
                }
                node = parent;
                continue;
            }
        }
        break;
    }
}

template<typename _DataType, typename _Compare>
void RedBlackTree<_DataType, _Compare>::_F_erase_adjust(TreeNode *node)
{
    using Color = typename DataNode::Color;
    while(node != nullptr)
    {
        TreeNode *parent = _M_tree.parent(node);
        TreeNode *brother = nullptr;
        if(node == _M_tree.left_child(parent))
        {
            brother = _M_tree.right_child(parent);
        }
        else
        {
            brother = _M_tree.left_child(parent);
        }
        if(_M_tree.left_child(parent) == node)
        {
            if(_F_node_color(brother) == Color::RED)
            {
                _F_node_color(brother) = Color::BLACK;
                _F_node_color(parent) = Color::RED;
                _M_tree.left_rotate(parent);
                continue;
            }
            else
            {
                TreeNode *br = _M_tree.right_child(brother);
                TreeNode *bl = _M_tree.left_child(brother);
                if(_F_node_color(br) == Color::RED)
                {
                    _F_node_color(brother) = _F_node_color(parent);
                    _F_node_color(parent) = Color::BLACK;
                    _F_node_color(br) = Color::BLACK;
                    _M_tree.left_rotate(parent);
                    break;
                }
                if(_F_node_color(bl) == Color::RED)
                {
                    _F_node_color(brother) = Color::RED;
                    _F_node_color(bl) = Color::BLACK;
                    _M_tree.right_rotate(brother);
                    continue;
                }
                else
                {
                    _F_node_color(brother) = Color::RED;
                    node = parent;
                    continue;
                }
            }
        }
        else
        {
            if(_F_node_color(brother) == Color::RED)
            {
                _F_node_color(brother) = Color::BLACK;
                _F_node_color(parent) = Color::RED;
                _M_tree.right_rotate(parent);
                continue;
            }
            else
            {
                TreeNode *br = _M_tree.right_child(brother);
                TreeNode *bl = _M_tree.left_child(brother);
                if(_F_node_color(bl) == Color::RED)
                {
                    _F_node_color(brother) = _F_node_color(parent);
                    _F_node_color(parent) = Color::BLACK;
                    _F_node_color(bl) = Color::BLACK;
                    _M_tree.right_rotate(parent);
                    break;
                }
                if(_F_node_color(br) == Color::RED)
                {
                    _F_node_color(brother) = Color::RED;
                    _F_node_color(br) = Color::BLACK;
                    _M_tree.left_rotate(brother);
                    continue;
                }
                else
                {
                    _F_node_color(brother) = Color::RED;
                    node = parent;
                    continue;
                }
            }
        }
    }
}

};

#endif // REDBLACKTREE_H
