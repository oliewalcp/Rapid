#ifndef SET_H
#define SET_H

#include "RB_Tree.h"

template<typename _Type, typename _Compare = Compare>
class Set : public RB_Tree<_Type, _Type, _Compare>
{
};

#endif // SET_H
