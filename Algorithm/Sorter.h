#ifndef SORTER_H
#define SORTER_H

#include "Core/Compare.h"
#include "Core/Version.h"
#include <type_traits>
#include <cmath>
#include <iostream>

namespace rapid
{

template<typename T>
void swap(T &arg1, T &arg2)
{
    T temp = arg1;
    arg1 = arg2;
    arg2 = temp;
}

template<typename T>
constexpr size_type distance(const T &_1, const T &_2)
{ return _1 < _2 ? _2 - _1 : _1 - _2; }

// bubble_sort
// not contain [end]
template<typename _BothIter,
         typename _Compare = Compare<decltype(*std::declval<_BothIter>())>>
void bsort(_BothIter beg, _BothIter end, _Compare c = _Compare())
{
    _BothIter max_pos = end;
    _BothIter min_pos = beg;
    for(_BothIter v = beg; v != end; ++v)
    {
        _BothIter current_pos = end;
        _BothIter b = min_pos, temp = b;
        for(++temp; temp != max_pos; ++b, ++temp)
        {
            if(c(*temp, *b) > 0)
            {
                swap(*temp, *b);
                current_pos = temp;
            }
        }
        if(current_pos == end)
        {
            return;
        }
        max_pos = current_pos;
        current_pos = end;
        b = max_pos;
        temp = b;
        for(--temp; b != min_pos; --b, --temp)
        {
            if(c(*b, *temp) > 0)
            {
                swap(*temp, *b);
                current_pos = b;
            }
        }
        ++min_pos;
        if(current_pos == end)
        {
            return;
        }
    }
}

// bubble_sort
// not contain [end]
template<typename _ForwardIter,
         typename _Compare = Compare<decltype(*std::declval<_ForwardIter>())>>
void fsort(_ForwardIter beg, _ForwardIter end, _Compare c = _Compare())
{
    _ForwardIter last_pos = end;
    for(_ForwardIter v = beg; v != end; ++v)
    {
        _ForwardIter current_pos = end;
        _ForwardIter b = beg, temp = b;
        for(++temp; temp != last_pos; ++b, ++temp)
        {
            if(c(*temp, *b) > 0)
            {
                swap(*temp, *b);
                current_pos = temp;
            }
        }
        last_pos = current_pos;
        if(last_pos == end)
        {
            return;
        }
    }
}
// shell sort
// not contain [end]
template<typename _RandomIter,
         typename _Compare = Compare<decltype(*std::declval<_RandomIter>())>>
void ssort(_RandomIter beg, _RandomIter end, _Compare c = _Compare())
{
    size_type total_size = distance(beg, end);
    size_type fin = static_cast<size_type>(std::log2(total_size - 1));
    size_type _1 = static_cast<size_type>(std::pow(2, fin));
    size_type dist = _1 - 1;
    size_type i, j;
    while(dist > 0)
    {
        for(i = dist; i < total_size; ++i)
        {
            auto temp = *(beg + i);
            for(j = i; j >= dist; j -= dist)
            {
                if(c(temp, *(beg + j - dist)) > 0)
                {
                    *(beg + j) = *(beg + j - dist);
                }
                else break;
            }
            *(beg + j) = temp;
        }
        if(dist == 1)
            break;
        _1 /= 2;
        dist = _1 - 1;
    }
}

}

#endif // SORTER_H
