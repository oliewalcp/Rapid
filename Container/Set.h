#ifndef SET_H
#define SET_H

#include "RB_Tree.h"

template<typename T>
class Set
{
    RB_Tree<T, T> tree;
public:
    void insert(T t){tree.insert_node(t, t);}
};

#endif // SET_H
