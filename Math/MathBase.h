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
private:
    typedef std::vector<value_type> __Vector;
    typedef std::vector<value_type *> __DataContainer;
    typedef std::allocator<value_type> __Alloc;
protected:
    __DataContainer *__data = nullptr;//begin at 0
    __uint32 __column;

    virtual void _create_data_container(const __uint32 line, const __uint32 column, const value_type &default_value);
public:
    _M_Base() { __column = 0; }
    _M_Base(const __uint32 line, const __uint32 column, const value_type &default_value);
    _M_Base(const __uint32 line, const __uint32 column, value_type && default_value);
    virtual ~_M_Base();

    virtual __uint32 line() { return __data == nullptr ? 0 : __data->size(); }
    virtual inline __uint32 column() { return __column; }
    /* get the value at the position of the line and column
     * param[line]:position of the line, begin at 0
     * param[column]:position of the column, begin at 0
     */
    virtual value_type& get_value(const __uint32 line, const __uint32 column)
    {
        if(this->line() <= line || this->column() <= column) throw Exception("the arguement is bigger than the size");
        value_type *temp = __data->at(line);
        return temp[column];
    }
    virtual value_type* get_line_value(const __uint32 line)
    {
        if(this->line() <= line) throw Exception("the arguement is bigger than the size");
        return __data->at(line);
    }
    virtual void swap_line(const __uint32 line1, const __uint32 line2)
    {
        if(this->line() <= line1 || this->column() <= line2) throw Exception("the arguement is bigger than the size");
        value_type *temp1 = __data->at(line1);
        value_type *temp2 = __data->at(line2);
        value_type *temp = temp1;
        (*__data)[line1] = temp2;
        (*__data)[line2] = temp;
    }
    virtual void set_value(const __uint32 line, const __uint32 column, const value_type &value)
    {
        if(this->line() <= line || this->column() <= column) throw Exception("the arguement is bigger than the size");
        value_type *temp = __data->at(line);
        memcpy(temp + sizeof(value_type) * column, &value, sizeof(value_type));
    }
    virtual void set_line_value(const value_type* value, const __uint32 line)
    {
        if(this->line() <= line) throw Exception("the arguement is bigger than the size");
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

    virtual void set_size(const __uint32 line, const __uint32 column);
    virtual void set_default_value(const value_type &default_value);
    virtual void clear();
};

template<typename value_type>
void _M_Base<value_type>::_create_data_container(const __uint32 line, const __uint32 column, const value_type & default_value)
{
    __column = column;
    __uint8 byte_size = sizeof(default_value);
    __data = new __DataContainer;
    __Alloc alloc;
    for(__uint32 i = 0; i < line; i++)
    {
        value_type *temp = alloc.allocate(__column);
        for(__uint32 j = 0; j < __column; j++)
            memcpy(&(temp[j]), &default_value, byte_size);
        __data->push_back(temp);
    }
}

template<typename value_type>
_M_Base<value_type>::_M_Base(const __uint32 line, const __uint32 column, const value_type &default_value)
{
    _create_data_container(line, column, default_value);
}
template<typename value_type>
_M_Base<value_type>::_M_Base(const __uint32 line, const __uint32 column, value_type && default_value)
{
    _create_data_container(line, column, std::forward<value_type&&>(default_value));
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
void _M_Base<value_type>::set_size(const __uint32 line, const __uint32 column)
{
    __Alloc alloc;
    if(this->line() == 0)
    {
        value_type *default_value = alloc.allocate(1);
        memset(default_value, 0, sizeof(value_type));
        _create_data_container(line, column, *default_value);
        delete default_value;
    }
    else if(this->line() != line || this->column() != column)
    {
        __DataContainer *temp_con = __data;
        __uint32 length = sizeof(value_type) * (this->column() > column ? column : this->column());
        __data = new __DataContainer;
        for(__uint32 i = 0; i < line; i++)
        {
            value_type *temp = alloc.allocate(column);
            memcpy(temp, temp_con, length);
            __data->push_back(temp);
        }
        temp_con->clear();
        delete temp_con;
    }
}

template<typename value_type>
void _M_Base<value_type>::set_default_value(const value_type &default_value)
{
    __uint32 length = sizeof(value_type) * this->column();
    for(__uint32 i = 0; i < this->line(); i++)
    {
        value_type *temp = this->get_line_value(i);
        for(__uint32 j = 0; j < this->column(); j++)
            memcpy(temp, &default_value, length);
    }
}

template<typename value_type>
void _M_Base<value_type>::clear()
{
    __data->clear();
    __data = nullptr;
    __column = 0;
}

#endif // TYPEBASE_H
