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
};

template<typename _Key, typename _Value, typename _Compare>
void BalanceBinaryTree<_Key, _Value, _Compare>::_adjust(BinaryNode<_Key, _Value> *node)
{
    BinaryNode<_Key, _Value> *temp = node->Parent;
    if(temp == nullptr) return;
    if(temp->LeftChildSize > temp->RightChildSize + 1)
    {
        _right_rotate(temp);
    }
    else if(temp->RightChildSize > temp->LeftChildSize + 1)
    {
        _left_rotate(temp);
    }
}

template<typename _Key, typename _Value, typename _Compare>
void BalanceBinaryTree<_Key, _Value, _Compare>::_left_rotate(BinaryNode<_Key, _Value> *pivot)
{
    BinaryNode<_Key, _Value> *right_child = pivot->Right;
    BinaryNode<_Key, _Value> *left_child = pivot->Left;
    BinaryNode<_Key, _Value> *parent = pivot->Parent;
    if(parent != nullptr)
    {
        if(pivot == parent->Left)
            parent->Left = right_child;
        else parent->Right = right_child;
    }
    right_child->LeftChildSize += pivot->LeftChildSize + 1;
    pivot->RightChildSize += right_child->LeftChildSize;
    pivot->RightChildSize -= right_child->RightChildSize + 1;

    BinaryNode<_Key, _Value> *under_right = _get_rightchild(left_child);
    under_right->Right = right_child->Left;
    right_child->Left = pivot;
    pivot->Parent = right_child;
}

template<typename _Key, typename _Value, typename _Compare>
void BalanceBinaryTree<_Key, _Value, _Compare>::_right_rotate(BinaryNode<_Key, _Value> *pivot)
{
    BinaryNode<_Key, _Value> *right_child = pivot->Right;
    BinaryNode<_Key, _Value> *left_child = pivot->Left;
    BinaryNode<_Key, _Value> *parent = pivot->Parent;
    if(parent != nullptr)
    {
        if(pivot == parent->Left)
            parent->Left = left_child;
        else parent->Right = left_child;
    }
    right_child->LeftChildSize += pivot->LeftChildSize + 1;
    pivot->RightChildSize += right_child->LeftChildSize;
    pivot->RightChildSize -= right_child->RightChildSize + 1;

    BinaryNode<_Key, _Value> *under_right = _get_rightchild(left_child);
    under_right->Right = right_child->Left;
    right_child->Left = pivot;
    pivot->Parent = right_child;
}

#endif // BALANCEBINARYTREE_H
