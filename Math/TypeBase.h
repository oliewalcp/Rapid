#ifndef TYPEBASE_H
#define TYPEBASE_H
#include <cstring>
#include <vector>
#include "Exception.h"

typedef unsigned int __uint32;
typedef unsigned char __uint8;

// support .bmp file currently
enum class Format : __uint8
{
    BMP,
    JPG,
    JPEG,
    PNG,
    CUSTOM  //custom format
};

template<typename value_type>
class _M_Base
{
protected:
    std::vector<value_type *> *__data;//begin at 0
    __uint32 __column;
public:
    _M_Base()
    {
        __data = nullptr;
        __column = 0;
    }
    _M_Base(const __uint32 line, const __uint32 column, const value_type &default_value);
    _M_Base(const __uint32 line, const __uint32 column, value_type && default_value);
    virtual ~_M_Base();

    virtual __uint32 line() { return __data->size(); }
    virtual __uint32 column() { return __column; }
    /* get the value at the position of the line and column
     * param[line]:position of the line, begin at 0
     * param[column]:position of the column, begin at 0
     */
    virtual inline value_type& get_value(const __uint32 line, const __uint32 column)
    {
        value_type *temp = __data->at(line);
        return temp[column];
    }
    virtual inline value_type* get_line_value(const __uint32 line)
    {
        return __data->at(line);
    }
    virtual inline void swap_line(const __uint32 line1, const __uint32 line2)
    {
        value_type *temp1 = __data->at(line1);
        value_type *temp2 = __data->at(line2);
        value_type *temp = temp1;
        (*__data)[line1] = temp2;
        (*__data)[line2] = temp;
    }
    virtual inline void set_value(const __uint32 line, const __uint32 column, const value_type &value)
    {
        value_type *temp = __data->at(line);
        memcpy(temp + sizeof(value_type) * column, &value, sizeof(value_type));
    }
    virtual inline void set_line_value(const value_type* value, const __uint32 line)
    {
        value_type *temp = __data->at(line);
        memcpy(temp, value, column());
    }
    /* let all the value of src line plus the value of dst line
     * param[src]: the value of src line will change
     * param[dst]: the value of src line will not change
     */
    virtual void line_add(const __uint32 src, const __uint32 dst);
    /* let all the value of src line minus the value of dst line
     * param[src]: the value of src line will change
     * param[dst]: the value of src line will not change
     */
    virtual void line_sub(const __uint32 src, const __uint32 dst);
    /* let all the value of src column plus the value of dst column
     * param[src]: the value of src line will change
     * param[dst]: the value of src line will not change
     */
    virtual void column_add(const __uint32 src, const __uint32 dst);
    /* let all the value of src column minus the value of dst column
     * param[src]: the value of src line will change
     * param[dst]: the value of src line will not change
     */
    virtual void column_sub(const __uint32 src, const __uint32 dst);

    virtual std::string to_std_string();
};

template<typename value_type>
_M_Base<value_type>::_M_Base(const __uint32 line, const __uint32 column, const value_type &default_value)
{
    __column = column;
    __data = new std::vector<value_type *>;
    for(__uint32 i = 0; i < line; i++)
    {
        value_type *temp = new value_type[column];
        for(__uint32 j = 0; j < column; j++)
            temp[j] = default_value;
        __data->push_back(temp);
    }
}
template<typename value_type>
_M_Base<value_type>::_M_Base(const __uint32 line, const __uint32 column, value_type && default_value)
{
    __column = column;
    __data = new std::vector<value_type *>;
    for(__uint32 i = 0; i < line; i++)
    {
        value_type *temp = new value_type[column];
        for(__uint32 j = 0; j < column; j++)
            temp[j] = default_value;
        __data->push_back(temp);
    }
}

template<typename value_type>
_M_Base<value_type>::~_M_Base()
{
    __data->clear();
}

template<typename value_type>
void _M_Base<value_type>::line_add(const __uint32 src, const __uint32 dst)
{
    value_type *line1 = __data->at(src);
    value_type *line2 = __data->at(dst);
    for(__uint32 i = 0; i < __column; i++)
        line1[i] += line2[i];
}

template<typename value_type>
void _M_Base<value_type>::line_sub(const __uint32 src, const __uint32 dst)
{
    value_type *line1 = __data->at(src);
    value_type *line2 = __data->at(dst);
    for(__uint32 i = 0; i < __column; i++)
        line1[i] -= line2[i];
}

template<typename value_type>
void _M_Base<value_type>::column_add(const __uint32 src, const __uint32 dst)
{
    __uint32 line_num = __data->size();
    for(__uint32 i = 0; i < line_num; i++)
    {
        value_type *temp = __data->at(i);
        temp[src] += temp[dst];
    }
}

template<typename value_type>
void _M_Base<value_type>::column_sub(const __uint32 src, const __uint32 dst)
{
    __uint32 line_num = __data->size();
    for(__uint32 i = 0; i < line_num; i++)
    {
        value_type *temp = __data->at(i);
        temp[src] -= temp[dst];
    }
}

template<typename value_type>
std::string _M_Base<value_type>::to_std_string()
{
    std::string result = "";
    __uint32 line_num = line();
    for(__uint32 i = 0; i < line_num; i++)
    {
        for(__uint32 j = 0; j < column(); j++)
        {
            result += std::to_string((int)(get_value(i, j))) + " ";
        }
        result += "\n";
    }
    return result;
}


#endif // TYPEBASE_H
