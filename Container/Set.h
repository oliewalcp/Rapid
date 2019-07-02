#ifndef SET_H
#define SET_H

#include "RB_Tree.h"

template<typename _Type, typename _Compare = Compare>
class Set : public RB_Tree<_Type, _Type, _Compare>
{
public:
    void insert(const _Type& arg) { this->insert(arg, arg); }
    void insert(_Type && arg) { this->insert(std::forward<_Type&&>(arg), std::forward<_Type&&>(arg)); }
};

#endif // SET_H
