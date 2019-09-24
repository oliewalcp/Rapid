#ifndef SORTER_H
#define SORTER_H

#include "Core/Compare.h"
#include "Core/Version.h"

namespace rapid
{

// bubble_sort
template<typename _BothIter, typename _Compare>
void bsort(_BothIter beg, _BothIter end, _Compare c)
{
    _BothIter last_pos = end;
    for(_BothIter v = beg; v != end; ++v)
    {
        _BothIter current_pos = end;
        for(_BothIter b = beg, temp = ++_BothIter(b);
            temp != last_pos;
            ++b, ++temp)
        {
            if(c(*temp, *b) > 0)
            {
                temp._M_current->swap(b._M_current);
                current_pos = temp;
            }
        }
        last_pos = current_pos;
        if(last_pos == nullptr)
        {
            return;
        }
    }
}

template<typename _BothIter>
void bsort(_BothIter beg, _BothIter end)
{ bsort(beg, end, Compare<decltype (*beg)>()); }

// bubble_sort
template<typename _ForwardIter, typename _Compare>
void fsort(_ForwardIter beg, _ForwardIter end, _Compare c)
{
    _ForwardIter last_pos = end;
    for(_ForwardIter v = beg; v != end; ++v)
    {
        _ForwardIter current_pos = end;
        for(_ForwardIter b = beg, temp = ++_ForwardIter(b);
            temp != last_pos;
            ++b, ++temp)
        {
            if(c(*temp, *b) > 0)
            {
                temp._M_current->swap(b._M_current);
                current_pos = temp;
            }
        }
        last_pos = current_pos;
        if(last_pos == nullptr)
        {
            return;
        }
    }
}

template<typename _ForwardIter>
void fsort(_ForwardIter beg, _ForwardIter end)
{ fsort(beg, end, Compare<decltype (*beg)>()); }

}

#endif // SORTER_H
