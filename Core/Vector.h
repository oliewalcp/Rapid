#ifndef VECTOR_H
#define VECTOR_H

#include "Core/Version.h"
#include "Core/TypeTraits.h"
#include "Core/Memory.h"

namespace rapid
{
template <typename T>
class Vector
{
private:
    using Type = T;
    using Pointer = Type*;
    using Reference = Type&;
    using ConstReference = const Type &;
    using RvalueReference = Type&&;
    using SizeType = long;

    SizeType __Size;
    SizeType __Capacity;
    NodeBase<Type> *__Data;

    void __initialize(SizeType size);
public:
    Vector() : __Size(0)
    { __initialize(1); }
    Vector(SizeType size) : __Size(0)
    { __initialize(size); }
    Vector(const Vector &arg);

    void push_back(ConstReference arg);
    void push_back(RvalueReference arg);
    void push_front(ConstReference arg);
    void push_front(RvalueReference arg);
};

};

#endif // VECTOR_H
