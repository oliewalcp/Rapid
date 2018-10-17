#ifndef TYPEBASE_H
#define TYPEBASE_H
#include <string.h>

typedef unsigned int __uint32;

template<__uint32 __line, __uint32 __column, typename value_type>
class _M_Base
{
protected:
    value_type __data[__line][__column];
public:
    virtual __uint32 line() { return __line; }
    virtual __uint32 column() { return __column; }
    virtual value_type operator()(__uint32 line, __uint32 column)
    { return __data[line][column]; }
    virtual void swap_line(__uint32 line1, __uint32 line2)
    {
        value_type temp;
        for(__uint32 i = 0; i < __column; i++)
        {
            temp = __data[line1][i];
            __data[line1][i] = __data[line2][i];
            __data[line2][i] = temp;
        }
    }
    virtual void line_add(__uint32 src, __uint32 dst)
    {
        for(__uint32 i = 0; i < __column; i++)
            __data[src][i] += __data[dst][i];
    }
    virtual void line_sub(__uint32 src, __uint32 dst)
    {
        for(__uint32 i = 0; i < __column; i++)
            __data[src][i] -= __data[dst][i];
    }
};

#endif // TYPEBASE_H
