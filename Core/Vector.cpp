#include "Core/Vector.h"

template<typename T>
void rapid::Vector<T>::__initialize(SizeType size)
{
    NodeBase<Type> *temp = __Data;
    __Data = new NodeBase<Type>[size];
    if(temp != nullptr)
    { mem_copy(__Data, temp, __Capacity); }
    __Capacity = size;
}

template<typename T>
rapid::Vector<T>::Vector(const Vector &arg)
{

}
