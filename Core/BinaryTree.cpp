#ifndef BINARYTREE_CPP
#define BINARYTREE_CPP

#include "BinaryTree.h"
#include "Stack.h"

template<typename _DataType>
void rapid::BinaryTree<_DataType>::_F_copy(const BinaryTree &tree)
{
    clear();
    if(tree.empty())
    { return; }
    Stack<TreeNode*> src, dst;
    for(auto it = tree.fbegin(); it != tree.fend(); ++it)
    {
        TreeNode *new_node = nullptr;
        if(src.size() == 0)
        {
            new_node = append_root(*it);
        }
        if(src.size() > 0)
        {
            while(parent(it._M_current) != src.top())
            {
                src.pop();
                dst.pop();
            }
            if(left_child(src.top()) == it._M_current)
            {
                new_node = append_left(dst.top(), *it);
            }
            else if(right_child(src.top()) == it._M_current)
            {
                new_node = append_right(dst.top(), *it);
            }
        }
        src.push(const_cast<TreeNode*>(it._M_current));
        dst.push(new_node);
    }
}

template<typename _DataType>
typename rapid::BinaryTree<_DataType>::TreeNode*
    rapid::BinaryTree<_DataType>::right_rotate(TreeNode *node)
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

template<typename _DataType>
typename rapid::BinaryTree<_DataType>::TreeNode*
    rapid::BinaryTree<_DataType>::left_rotate(TreeNode *node)
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
template<typename _DataType>
typename rapid::BinaryTree<_DataType>::TreeNode*
    rapid::BinaryTree<_DataType>::left_child_under(TreeNode *node)
{
    using BT = rapid::BinaryTree<_DataType>;
    while(node != nullptr && BT::left_child(node) != nullptr)
    {
        node = BT::left_child(node);
    }
    return node;
}
template<typename _DataType>
typename rapid::BinaryTree<_DataType>::TreeNode*
    rapid::BinaryTree<_DataType>::right_child_under(TreeNode *node)
{
    using BT = rapid::BinaryTree<_DataType>;
    while(node != nullptr && BT::right_child(node) != nullptr)
    {
        node = BT::right_child(node);
    }
    return node;
}
template<typename _DataType>
typename rapid::BinaryTree<_DataType>::TreeNode*
    rapid::BinaryTree<_DataType>::left_leaves(TreeNode *node)
{
    using BT = rapid::BinaryTree<_DataType>;
    node = left_child_under(node);
    while(BT::right_child(node) != nullptr)
    {
        node = left_child_under(BT::right_child(node));
    }
    return node;
}
template<typename _DataType>
typename rapid::BinaryTree<_DataType>::TreeNode*
    rapid::BinaryTree<_DataType>::right_leaves(TreeNode *node)
{
    using BT = rapid::BinaryTree<_DataType>;
    node = right_child_under(node);
    while(BT::left_child(node) != nullptr)
    {
        node = right_child_under(BT::left_child(node));
    }
    return node;
}
template<typename _DataType>
typename rapid::BinaryTree<_DataType>::TreeNode*
    rapid::BinaryTree<_DataType>::former_next(TreeNode *current)
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
template<typename _DataType>
typename rapid::BinaryTree<_DataType>::TreeNode*
    rapid::BinaryTree<_DataType>::middle_next(TreeNode *current)
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

template<typename _DataType>
typename rapid::BinaryTree<_DataType>::TreeNode*
    rapid::BinaryTree<_DataType>::after_next(TreeNode *current)
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

template<typename _DataType>
typename rapid::BinaryTree<_DataType>::TreeNode*
    rapid::BinaryTree<_DataType>::former_previous(TreeNode *current)
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
template<typename _DataType>
typename rapid::BinaryTree<_DataType>::TreeNode*
    rapid::BinaryTree<_DataType>::middle_previous(TreeNode *current)
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

template<typename _DataType>
typename rapid::BinaryTree<_DataType>::TreeNode*
    rapid::BinaryTree<_DataType>::after_previous(TreeNode *current)
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
template<typename _DataType>
typename rapid::BinaryTree<_DataType>::TreeNode*
    rapid::BinaryTree<_DataType>::erase(TreeNode *node)
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

//---------------------------------------------------------//
#include <iostream>
#include <iomanip>
#include <cmath>
#include "DoubleLinkedList.h"
#include "DoubleLinkedList.cpp"
#define WORD_WIDTH 5

template<typename T>
static std::string to_string(const T &arg)
{
    std::string result, temp = std::to_string(arg);
    std::size_t s = result.size();
    std::size_t left = (WORD_WIDTH - s) / 2;
    std::size_t right = WORD_WIDTH - left;
    for(std::size_t i = 0; i < left; ++i)
    {
        result.push_back(' ');
    }
    result += temp;
    for(std::size_t i = 0; i < right; ++i)
    {
        result.push_back(' ');
    }
    return result;
}

static void print_space(int size)
{
    for(int i = 0; i < size; ++i)
    {
        std::cout << ' ';
    }
}

template<typename T>
void rapid::print_tree(const rapid::BinaryTree<T> &tree)
{
    using BT = rapid::BinaryTree<T>;
    using TreeNode = typename BT::TreeNode;
    DoubleLinkedList<TreeNode *> s;
    s.push_back(tree.root());
    int total_element = pow(2, tree.depth()) - 1;
    for(int i = 1; i <= tree.depth(); ++i)
    {
        int temp_size = s.size();
        for(int i = 0; i < temp_size; ++i)
        {
            TreeNode *node = s.front();
            s.pop_front();
            s.push_back(BT::left_child(node));
            s.push_back(BT::right_child(node));
            print_space(total_element / (i + 1) * WORD_WIDTH);
            std::cout << to_string(node->data());
        }
    }
}

void rapid::test_BinaryTree_main()
{
    std::cout << "------------" << __func__ << "------------" << std::endl;
    using Node = BinaryTree<int>::TreeNode;
    BinaryTree<int> bt;
    Node *not_need;
    Node *root = bt.append_root(50);
    Node *temp = bt.append_left(root, 10);
    bt.append_left(temp, 20);
    temp = bt.append_right(temp, 100);
    not_need = temp = bt.append_right(root, 500);
    bt.append_right(temp, -1);
    temp = bt.append_left(temp, -200);
    temp = bt.append_left(temp, 1);
    temp = bt.append_left(temp, 2);
    std::cout << "size = " << bt.size() << std::endl;
    print_tree(bt);
//    std::cout << "middle: " << std::endl;
//    for(int i : bt)
//    {
//        std::cout << i << " ";
//    }
//    std::cout << std::endl << "former: " << std::endl;
//    for(auto it = bt.fbegin(); it != bt.fend(); ++it)
//    {
//        std::cout << *it << " ";
//    }
//    std::cout << std::endl << "after: " << std::endl;
//    for(auto it = bt.abegin(); it != bt.aend(); ++it)
//    {
//        std::cout << *it << " ";
//    }
//    std::cout << std::endl;
//    std::cout << "depth: ----->  " << bt.depth() << std::endl;
//    std::cout << std::endl << "----------------------------" << std::endl;
//    BinaryTree<int> bt2(bt);
//    std::cout << "size = " << bt2.size() << std::endl;
//    std::cout << "middle: " << std::endl;
//    for(int i : bt2)
//    {
//        std::cout << i << " ";
//    }
//    std::cout << std::endl << "former: " << std::endl;
//    for(auto it = bt2.fbegin(); it != bt2.fend(); ++it)
//    {
//        std::cout << *it << " ";
//    }
//    std::cout << std::endl << "after: " << std::endl;
//    for(auto it = bt2.abegin(); it != bt2.aend(); ++it)
//    {
//        std::cout << *it << " ";
//    }
//    std::cout << std::endl;
//    std::cout << std::endl << "----------------------------" << std::endl;
//    bt.right_rotate(root);
//    std::cout << "size = " << bt.size() << std::endl;
//    for(int i : bt)
//    {
//        std::cout << i << " ";
//    }
//    std::cout << std::endl << "after: " << std::endl;
//    for(auto it = bt.abegin(); it != bt.aend(); ++it)
//    {
//        std::cout << *it << " ";
//    }
//    std::cout << std::endl;
//    bt.left_rotate(root);
//    bt.left_rotate(bt.root());
//    std::cout << std::endl;
//    std::cout << "size = " << bt.size() << std::endl;
//    for(int i : bt)
//    {
//        std::cout << i << " ";
//    }
//    std::cout << std::endl << "after: " << std::endl;
//    for(auto it = bt.abegin(); it != bt.aend(); ++it)
//    {
//        std::cout << *it << " ";
//    }
//    std::cout << std::endl << "------------reverse----------------" << std::endl;
//    std::cout << "former reverse" << std::endl;
//    for(auto it = bt2.frbegin(); it != bt2.frend(); ++it)
//    {
//        std::cout << *it << " ";
//    }
//    std::cout << std::endl;
//    std::cout << "middle reverse" << std::endl;
//    for(auto it = bt2.mrbegin(); it != bt2.mrend(); ++it)
//    {
//        std::cout << *it << " ";
//    }
//    std::cout << std::endl;
//    std::cout << "after reverse" << std::endl;
//    for(auto it = bt2.arbegin(); it != bt2.arend(); ++it)
//    {
//        std::cout << *it << " ";
//    }
//    std::cout << std::endl;
//    std::cout << "--------------erase--------------" << std::endl;
//    bt.erase(not_need);
//    std::cout << "size = " << bt.size() << std::endl;
//    std::cout << "middle: " << std::endl;
//    for(int i : bt)
//    {
//        std::cout << i << " ";
//    }
//    std::cout << std::endl << "former: " << std::endl;
//    for(auto it = bt.fbegin(); it != bt.fend(); ++it)
//    {
//        std::cout << *it << " ";
//    }
//    std::cout << std::endl << "after: " << std::endl;
//    for(auto it = bt.abegin(); it != bt.aend(); ++it)
//    {
//        std::cout << *it << " ";
//    }
//    std::cout << std::endl;
}

#endif // end BINARYTREE_CPP
