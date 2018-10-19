#ifndef TYPEBASE_H
#define TYPEBASE_H
#include <string.h>

typedef unsigned int __uint32;

template<__uint32 __line, __uint32 __column, typename value_type>
class _M_Base
{
public:
    value_type __data[__line][__column];

    virtual __uint32 line() { return __line; }
    virtual __uint32 column() { return __column; }

    /* get the value at the position of the line and column
     * param[line]:position of the line, begin at 0
     * param[column]:position of the column, begin at 0
     */
    virtual value_type operator()(__uint32 line, __uint32 column)
    { return __data[line][column]; }
    virtual value_type get_value(__uint32 line, __uint32 column)
    { return __data[line][column];}
    /* swap the value of two line */
    virtual void swap_line(__uint32 line1, __uint32 line2)
    {
        value_type *temp = reinterpret_cast<value_type *>(::operator new(sizeof(value_type)));
        for(__uint32 i = 0; i < __column; i++)
        {
            memmove(temp, &__data[line1][i], sizeof(value_type));
            memmove(&__data[line1][i], &__data[line2][i], sizeof(value_type));
            memmove(&__data[line2][i], temp, sizeof(value_type));
        }
        ::operator delete(temp);
    }
    /* let all the value of src line plus the value of dst line
     * param[src]: the value of src line will change
     * param[dst]: the value of src line will not change
     */
    virtual void line_add(__uint32 src, __uint32 dst)
    {
        for(__uint32 i = 0; i < __column; i++)
            __data[src][i] += __data[dst][i];
    }
    /* let all the value of src line minus the value of dst line
     * param[src]: the value of src line will change
     * param[dst]: the value of src line will not change
     */
    virtual void line_sub(__uint32 src, __uint32 dst)
    {
        for(__uint32 i = 0; i < __column; i++)
            __data[src][i] -= __data[dst][i];
    }
    /* change the value at the position of the line and column
     * param[line]: the position of the line
     * param[column]: the position of the column
     */
    virtual void set_value(const __uint32 line, const __uint32 column, const value_type& value)
    {
        memcpy(&__data[line][column], &value, sizeof(value_type));
    }
};

#endif // TYPEBASE_H
