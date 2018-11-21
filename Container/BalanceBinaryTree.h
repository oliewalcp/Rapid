#ifndef BALANCEBINARYTREE_H
#define BALANCEBINARYTREE_H

#include "BinarySortTree.h"

template <typename _Key, typename _Value, typename _Compare = Compare>
class BalanceBinaryTree : public BinarySortTree<_Key, _Value, _Compare>
{
protected:
    virtual void _adjust(BinaryNode<_Key, _Value> *node);
    virtual void _left_rotate(BinaryNode<_Key, _Value> *pivot);
    virtual void _right_rotate(BinaryNode<_Key, _Value> *pivot);
    virtual void _sub_depth(BinaryNode<_Key, _Value> *node);
};

template<typename _Key, typename _Value, typename _Compare>
void BalanceBinaryTree<_Key, _Value, _Compare>::_adjust(BinaryNode<_Key, _Value> *node)
{
    BinaryNode<_Key, _Value> *temp = node->Parent;
    while(temp != nullptr)
    {
        if(temp->LeftDepth > temp->RightDepth + 1)
            _right_rotate(temp);
        else if(temp->RightDepth > temp->LeftDepth + 1)
            _left_rotate(temp);
        temp = temp->Parent;
    }
}

template<typename _Key, typename _Value, typename _Compare>
void BalanceBinaryTree<_Key, _Value, _Compare>::_left_rotate(BinaryNode<_Key, _Value> *pivot)
{
    BinaryNode<_Key, _Value> *right_child = pivot->Right;
    if(pivot == nullptr || right_child == nullptr) return;
    BinaryNode<_Key, _Value> *parent = pivot->Parent;
    if(parent != nullptr)
    {
        if(pivot == parent->Left)
            parent->Left = right_child;
        else parent->Right = right_child;
    }
    else this->_Root = right_child;
    right_child->Parent = parent;

    pivot->RightDepth = right_child->LeftDepth;
    right_child->LeftDepth = pivot->LeftDepth + 1;

    pivot->Right = right_child->Left;
    if(right_child->Left != nullptr)
        right_child->Left->Parent = pivot;

    pivot->Parent = right_child;
    right_child->Left = pivot;

    _sub_depth(right_child);
}

template<typename _Key, typename _Value, typename _Compare>
void BalanceBinaryTree<_Key, _Value, _Compare>::_right_rotate(BinaryNode<_Key, _Value> *pivot)
{
    BinaryNode<_Key, _Value> *left_child = pivot->Left;
    if(pivot == nullptr || left_child == nullptr) return;
    BinaryNode<_Key, _Value> *parent = pivot->Parent;
    if(parent != nullptr)
    {
        if(pivot == parent->Left)
            parent->Left = left_child;
        else parent->Right = left_child;
    }
    else this->_Root = left_child;
    left_child->Parent = parent;

    pivot->LeftDepth = left_child->RightDepth;
    left_child->RightDepth = pivot->RightDepth + 1;

    pivot->Left = left_child->Right;
    if(left_child->Right != nullptr)
        left_child->Right->Parent = pivot;

    pivot->Parent = left_child;
    left_child->Right = pivot;

    _sub_depth(left_child);
}

template<typename _Key, typename _Value, typename _Compare>
void BalanceBinaryTree<_Key, _Value, _Compare>::_sub_depth(BinaryNode<_Key, _Value> *node)
{
    BinaryNode<_Key, _Value> *temp = node->Parent;
    while(temp != nullptr)
    {
        if(node == temp->Left) temp->LeftDepth--;
        else temp->RightDepth--;
        node = temp;
        temp = temp->Parent;
    }
}

#endif // BALANCEBINARYTREE_H
