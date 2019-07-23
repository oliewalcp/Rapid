#ifndef ARRAY_H
#define ARRAY_H

#include "Version.h"
#include "Preoperation.h"

namespace rapid
{

template<typename T, unsigned long Size>
class Array
{
private:
    using SizeType = unsigned long;

    T *__Data;
public:
    Array()
    { __Data = new T[Size]; }
};

};

#endif // ARRAY_H
