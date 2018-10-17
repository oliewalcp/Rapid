#ifndef GRAYIMAGE_H
#define GRAYIMAGE_H
#include "../TypeBase.h"

typedef unsigned char __uint8;

template<__uint32 __line, __uint32 __column>
class GrayImage : public _M_Base<__line, __column, __uint8>
{
public:
    explicit GrayImage(__uint8 default_value = 0);
};

template<__uint32 __line, __uint32 __column>
GrayImage<__line, __column>::GrayImage(__uint8 default_value)
{
    for(__uint32 i = 0; i < __line; i++)
        for(__uint32 j = 0; j < __column; j++)
            this->__data[i][j] = default_value;
}


#endif // GRAYIMAGE_H
