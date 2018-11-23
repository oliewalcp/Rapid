#ifndef RB_TREE_H
#define RB_TREE_H
#include "BalanceBinaryTree.h"

/* 1. root is black
 * 2. node is black or red
 * 3. empty node is black
 * 4. red node doesn't have red children
 * 5. all paths from any node to its leaves contain the same number of black nodes.
 */

template<typename _Key, typename _Value, typename _Compare = Compare>
class RB_Tree : public BalanceBinaryTree<_Key, _Value, _Compare>
{
private:
    inline void __uncle_red_adjust(BinaryNode<_Key, _Value> *node, BinaryNode<_Key, _Value> *uncle)
    {
        node->Parent->Color = BinarySortTree<_Key, _Value, _Compare>::BLACK;
        uncle->Color = BinarySortTree<_Key, _Value, _Compare>::BLACK;
        uncle->Parent->Color = BinarySortTree<_Key, _Value, _Compare>::RED;
    }
protected:
    virtual void _adjust(BinaryNode<_Key, _Value> *node);// will be run after inserting operator
};

/* will be called after inserting operator
 * param[node]: new inserting node
 *
 * while [node]'s parent is a red node:
 *     if [node]'s parent is a left child:
 *         if [node]'s uncle is a red node:
 *             set [node]'s parent to black
 *             set [node]'s uncle to black
 *             set [node]'s grandparent to red
 *             set [node] to [node]'s grandparent
 *         else if [node] is a right child:
 *             set [node] to [node]'s parent
 *             left rotate with [node] as pivot
 *         else if [node]'s uncle is a black node:
 *             set [node]'s parent to black
 *             set [node]'s grandparent to red
 *             right rotate with [node]'s grandparent as pivot
 *     else if [node]'s parent is a right child:
 *         same operator with when [node]'s parent is a left child
 * set [_Root] to black
 */
template<typename _Key, typename _Value, typename _Compare>
void RB_Tree<_Key, _Value, _Compare>::_adjust(BinaryNode<_Key, _Value> *node)
{
    //if [node]'s parent is a red node
    while(node->Parent != nullptr && node->Parent->Parent != nullptr && node->Parent->Color == BinarySortTree<_Key, _Value, _Compare>::RED)
    {
        //if [node]'s parent is a left child
        if(node->Parent == node->Parent->Parent->Left)
        {
            BinaryNode<_Key, _Value> *uncle = node->Parent->Parent->Right;//get uncle
            //if [node]'s uncle is a red node
            if(uncle != nullptr && uncle->Color == BinarySortTree<_Key, _Value, _Compare>::RED)
            {
                __uncle_red_adjust(node, uncle);
                node = uncle->Parent;
            }
            //if [node] is a right child
            else if(node == node->Parent->Right)
            {
                node = node->Parent;
                this->_left_rotate(node);
            }
            //if [node] is a left child
            else
            {
                node->Parent->Color = BinarySortTree<_Key, _Value, _Compare>::BLACK;
                uncle->Parent->Color = BinarySortTree<_Key, _Value, _Compare>::RED;
                this->_right_rotate(uncle->Parent);
            }
        }
        else
        {
            BinaryNode<_Key, _Value> *uncle = node->Parent->Parent->Left;//get uncle
            //if [node]'s uncle is a red node
            if(uncle != nullptr && uncle->Color == BinarySortTree<_Key, _Value, _Compare>::RED)
            {
                __uncle_red_adjust(node, uncle);
                node = uncle->Parent;
            }
            //if [node] is a right child
            else if(node == node->Parent->Left)
            {
                node = node->Parent;
                this->_right_rotate(node);
            }
            //if [node] is a left child
            else
            {
                node->Parent->Color = BinarySortTree<_Key, _Value, _Compare>::BLACK;
                uncle->Parent->Color = BinarySortTree<_Key, _Value, _Compare>::RED;
                this->_left_rotate(node->Parent->Parent);
            }
        }
    }
    this->_Root->Color = BinarySortTree<_Key, _Value, _Compare>::BLACK;
}

#endif // RB_TREE_H
