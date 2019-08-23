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
