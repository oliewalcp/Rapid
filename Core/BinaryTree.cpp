#include "Core/BinaryTree.h"
#include "Core/Stack.h"
#include <iostream>

template<typename T>
int rapid::Equal<T>::operator()(const T &arg1, const T &arg2) const
{
    if(arg1 < arg2) return 1;
    if(arg2 < arg1) return -1;
    return 0;
}

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
        src.push(it._M_current);
        dst.push(new_node);
    }
}

template<typename _DataType>
void rapid::BinaryTree<_DataType>::right_rotate(TreeNode *node)
{
    TreeNode *left_node = left_child(node);
    if(left_node == nullptr) return;
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
}

template<typename _DataType>
void rapid::BinaryTree<_DataType>::left_rotate(TreeNode *node)
{
    TreeNode *right_node = right_child(node);
    if(right_node == nullptr) return;
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
}

//---------------------***************---------------------//
template<typename _DataType>
typename rapid::BinaryTree<_DataType>::TreeNode* left_child_under(
        typename rapid::BinaryTree<_DataType>::TreeNode *node)
{
    using BT = rapid::BinaryTree<_DataType>;
    while(node != nullptr && BT::left_child(node) != nullptr)
    {
        node = BT::left_child(node);
    }
    return node;
}
template<typename _DataType>
typename rapid::BinaryTree<_DataType>::TreeNode* right_child_under(
        typename rapid::BinaryTree<_DataType>::TreeNode *node)
{
    using BT = rapid::BinaryTree<_DataType>;
    while(node != nullptr && BT::right_child(node) != nullptr)
    {
        node = BT::right_child(node);
    }
    return node;
}
template<typename _DataType>
typename rapid::BinaryTree<_DataType>::TreeNode* left_leaves(
        typename rapid::BinaryTree<_DataType>::TreeNode *node)
{
    using BT = rapid::BinaryTree<_DataType>;
    node = left_child_under<_DataType>(node);
    while(BT::right_child(node) != nullptr)
    {
        node = left_child_under<_DataType>(BT::right_child(node));
    }
    return node;
}
template<typename _DataType>
typename rapid::BinaryTree<_DataType>::TreeNode* right_leaves(
        typename rapid::BinaryTree<_DataType>::TreeNode *node)
{
    using BT = rapid::BinaryTree<_DataType>;
    node = right_child_under<_DataType>(node);
    while(BT::left_child(node) != nullptr)
    {
        node = right_child_under<_DataType>(BT::left_child(node));
    }
    return node;
}
template<typename _DataType>
typename rapid::BinaryTree<_DataType>::TreeNode* former_next(
        typename rapid::BinaryTree<_DataType>::TreeNode *current)
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
typename rapid::BinaryTree<_DataType>::TreeNode* middle_next(
        typename rapid::BinaryTree<_DataType>::TreeNode *current)
{
    using namespace rapid;
    using BT = BinaryTree<_DataType>;
    using Node = typename BT::TreeNode;
    if(BT::right_child(current) != nullptr)
    {
        return left_child_under<_DataType>(BT::right_child(current));
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
typename rapid::BinaryTree<_DataType>::TreeNode* after_next(
        typename rapid::BinaryTree<_DataType>::TreeNode *current)
{
    using namespace rapid;
    using BT = BinaryTree<_DataType>;
    if(current == BT::left_child(BT::parent(current)))
    {
        if(BT::right_child(BT::parent(current)) == nullptr)
        {
            return BT::parent(current);
        }
        return left_leaves<_DataType>(BT::right_child(BT::parent(current)));
    }
    if(current == BT::right_child(BT::parent(current)))
    {
        return BT::parent(current);
    }
    return nullptr;
}

template<typename _DataType>
typename rapid::BinaryTree<_DataType>::TreeNode* former_previous(
        typename rapid::BinaryTree<_DataType>::TreeNode *current)
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
            return right_leaves<_DataType>(BT::left_child(current));
        }
        return current;
    }
    return nullptr;
}
template<typename _DataType>
typename rapid::BinaryTree<_DataType>::TreeNode* middle_previous(
        typename rapid::BinaryTree<_DataType>::TreeNode *current)
{
    using namespace rapid;
    using BT = BinaryTree<_DataType>;
    if(BT::left_child(current) != nullptr)
    {
        return right_child_under<_DataType>(BT::left_child(current));
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
typename rapid::BinaryTree<_DataType>::TreeNode* after_previous(
        typename rapid::BinaryTree<_DataType>::TreeNode *current)
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
void rapid::BinaryTree<_DataType>::FormerIterator::_F_next()
{
    _M_current = former_next<_DataType>(_M_current);
}

template<typename _DataType>
void rapid::BinaryTree<_DataType>::FormerIterator::_F_previous()
{
    _M_current = former_previous<_DataType>(_M_current);
}

template<typename _DataType>
rapid::BinaryTree<_DataType>::FormerIterator::FormerIterator(TreeNode *root)
{
    _M_current = root;
}
//---------------------------------------------------------//
template<typename _DataType>
void rapid::BinaryTree<_DataType>::ConstFormerIterator::_F_next()
{
    _M_current = former_next<_DataType>(_M_current);
}

template<typename _DataType>
void rapid::BinaryTree<_DataType>::ConstFormerIterator::_F_previous()
{
    _M_current = former_previous<_DataType>(_M_current);
}

template<typename _DataType>
rapid::BinaryTree<_DataType>::ConstFormerIterator::ConstFormerIterator(TreeNode *root)
{
    _M_current = root;
}
//---------------------------------------------------------//
template<typename _DataType>
void rapid::BinaryTree<_DataType>::MiddleIterator::_F_next()
{
    _M_current = middle_next<_DataType>(_M_current);
}

template<typename _DataType>
void rapid::BinaryTree<_DataType>::MiddleIterator::_F_previous()
{
    _M_current = middle_previous<_DataType>(_M_current);
}

template<typename _DataType>
rapid::BinaryTree<_DataType>::MiddleIterator::MiddleIterator(TreeNode *root)
{
    _M_current = left_child_under<_DataType>(root);
}

//---------------------------------------------------------//
template<typename _DataType>
void rapid::BinaryTree<_DataType>::ConstMiddleIterator::_F_next()
{
    _M_current = middle_next<_DataType>(_M_current);
}

template<typename _DataType>
void rapid::BinaryTree<_DataType>::ConstMiddleIterator::_F_previous()
{
    _M_current = middle_previous<_DataType>(_M_current);
}

template<typename _DataType>
rapid::BinaryTree<_DataType>::ConstMiddleIterator::ConstMiddleIterator(TreeNode *root)
{
    _M_current = left_child_under<_DataType>(root);
}
//---------------------------------------------------------//
template<typename _DataType>
void rapid::BinaryTree<_DataType>::AfterIterator::_F_next()
{
    _M_current = after_next<_DataType>(_M_current);
}

template<typename _DataType>
void rapid::BinaryTree<_DataType>::AfterIterator::_F_previous()
{
    _M_current = after_previous<_DataType>(_M_current);
}

template<typename _DataType>
rapid::BinaryTree<_DataType>::AfterIterator::AfterIterator(TreeNode *root)
{
    _M_current = left_leaves<_DataType>(root);
}
//---------------------------------------------------------//
template<typename _DataType>
void rapid::BinaryTree<_DataType>::ConstAfterIterator::_F_next()
{
    _M_current = after_next<_DataType>(_M_current);
}

template<typename _DataType>
void rapid::BinaryTree<_DataType>::ConstAfterIterator::_F_previous()
{
    _M_current = after_previous<_DataType>(_M_current);
}

template<typename _DataType>
rapid::BinaryTree<_DataType>::ConstAfterIterator::ConstAfterIterator(TreeNode *root)
{
    _M_current = left_leaves<_DataType>(root);
}
//---------------------------------------------------------//
template<typename _DataType>
void rapid::BinaryTree<_DataType>::ReverseFormerIterator::_F_next()
{
    _M_current = former_previous<_DataType>(_M_current);
}

template<typename _DataType>
void rapid::BinaryTree<_DataType>::ReverseFormerIterator::_F_previous()
{
    _M_current = former_next<_DataType>(_M_current);
}

template<typename _DataType>
rapid::BinaryTree<_DataType>::ReverseFormerIterator::ReverseFormerIterator(TreeNode *root)
{
    _M_current = right_leaves<_DataType>(root);
}
//---------------------------------------------------------//
template<typename _DataType>
void rapid::BinaryTree<_DataType>::ConstReverseFormerIterator::_F_next()
{
    _M_current = former_previous<_DataType>(_M_current);
}

template<typename _DataType>
void rapid::BinaryTree<_DataType>::ConstReverseFormerIterator::_F_previous()
{
    _M_current = former_next<_DataType>(_M_current);
}

template<typename _DataType>
rapid::BinaryTree<_DataType>::ConstReverseFormerIterator::ConstReverseFormerIterator(TreeNode *root)
{
    _M_current = right_leaves<_DataType>(root);
}

//---------------------------------------------------------//
template<typename _DataType>
void rapid::BinaryTree<_DataType>::ReverseMiddleIterator::_F_next()
{
    _M_current = middle_previous<_DataType>(_M_current);
}

template<typename _DataType>
void rapid::BinaryTree<_DataType>::ReverseMiddleIterator::_F_previous()
{
    _M_current = middle_next<_DataType>(_M_current);
}

template<typename _DataType>
rapid::BinaryTree<_DataType>::ReverseMiddleIterator::ReverseMiddleIterator(TreeNode *root)
{
    _M_current = right_child_under<_DataType>(root);
}

//---------------------------------------------------------//
template<typename _DataType>
void rapid::BinaryTree<_DataType>::ConstReverseMiddleIterator::_F_next()
{
    _M_current = middle_previous<_DataType>(_M_current);
}

template<typename _DataType>
void rapid::BinaryTree<_DataType>::ConstReverseMiddleIterator::_F_previous()
{
    _M_current = middle_next<_DataType>(_M_current);
}

template<typename _DataType>
rapid::BinaryTree<_DataType>::ConstReverseMiddleIterator::ConstReverseMiddleIterator(TreeNode *root)
{
    _M_current = right_child_under<_DataType>(root);
}

//---------------------------------------------------------//
template<typename _DataType>
void rapid::BinaryTree<_DataType>::ReverseAfterIterator::_F_next()
{
    _M_current = after_previous<_DataType>(_M_current);
}

template<typename _DataType>
void rapid::BinaryTree<_DataType>::ReverseAfterIterator::_F_previous()
{
    _M_current = after_next<_DataType>(_M_current);
}

template<typename _DataType>
rapid::BinaryTree<_DataType>::ReverseAfterIterator::ReverseAfterIterator(TreeNode *root)
{
    _M_current = root;
}

//---------------------------------------------------------//
template<typename _DataType>
void rapid::BinaryTree<_DataType>::ConstReverseAfterIterator::_F_next()
{
    _M_current = after_previous<_DataType>(_M_current);
}

template<typename _DataType>
void rapid::BinaryTree<_DataType>::ConstReverseAfterIterator::_F_previous()
{
    _M_current = after_next<_DataType>(_M_current);
}

template<typename _DataType>
rapid::BinaryTree<_DataType>::ConstReverseAfterIterator::ConstReverseAfterIterator(TreeNode *root)
{
    _M_current = root;
}
//---------------------------------------------------------//
void rapid::test_BinaryTree_main()
{
    using Node = BinaryTree<int>::TreeNode;
    BinaryTree<int> bt;
    Node *root = bt.append_root(50);
    Node *temp = bt.append_left(root, 10);
    bt.append_left(temp, 20);
    temp = bt.append_right(temp, 100);
    temp = bt.append_right(root, 500);
    bt.append_right(temp, -1);
    temp = bt.append_left(temp, -200);
    temp = bt.append_left(temp, 1);
    temp = bt.append_left(temp, 2);
    std::cout << "size = " << bt.size() << std::endl;
    std::cout << "middle: " << std::endl;
    for(int i : bt)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl << "former: " << std::endl;
    for(auto it = bt.fbegin(); it != bt.fend(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl << "after: " << std::endl;
    for(auto it = bt.abegin(); it != bt.aend(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl << "----------------------------" << std::endl;
    BinaryTree<int> bt2(bt);
    std::cout << "size = " << bt2.size() << std::endl;
    std::cout << "middle: " << std::endl;
    for(int i : bt2)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl << "former: " << std::endl;
    for(auto it = bt2.fbegin(); it != bt2.fend(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl << "after: " << std::endl;
    for(auto it = bt2.abegin(); it != bt2.aend(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl << "----------------------------" << std::endl;
    bt.right_rotate(root);
    std::cout << "size = " << bt.size() << std::endl;
    for(int i : bt)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl << "after: " << std::endl;
    for(auto it = bt.abegin(); it != bt.aend(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    bt.left_rotate(root);
    bt.left_rotate(bt.root());
    std::cout << std::endl;
    std::cout << "size = " << bt.size() << std::endl;
    for(int i : bt)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl << "after: " << std::endl;
    for(auto it = bt.abegin(); it != bt.aend(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl << "------------reverse----------------" << std::endl;
    std::cout << "former reverse" << std::endl;
    for(auto it = bt2.frbegin(); it != bt2.frend(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    std::cout << "middle reverse" << std::endl;
    for(auto it = bt2.mrbegin(); it != bt2.mrend(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    std::cout << "after reverse" << std::endl;
    for(auto it = bt2.arbegin(); it != bt2.arend(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}
