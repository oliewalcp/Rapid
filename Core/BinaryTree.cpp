#include "BinaryTree.h"

template<typename T>
int rapid::Equal<T>::operator()(const T &arg1, const T &arg2)
{
    if(arg1 < arg2)
        return 1;
    if(arg2 < arg1)
        return -1;
    return 0;
}

template<typename _DataType, typename _EqualTo>
void rapid::BinaryTree<_DataType, _EqualTo>::append_left(TreeNode *node, ConstReference data)
{
    if(node != nullptr)
    {
        node->Left = new BTreeNode(data);
        node->Left->Parent = node;
    }
}
