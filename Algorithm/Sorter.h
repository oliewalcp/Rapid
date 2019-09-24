#ifndef SORTER_H
#define SORTER_H

#include "Core/Compare.h"
#include "Core/Version.h"
#include "Core/TypeTraits.h"
#include <iostream>

namespace rapid
{

template<typename T>
void swap(T &arg1, T &arg2)
{
    std::cout << "occur swap: " << arg1 << " and " << arg2 << std::endl;
    T temp = arg1;
    arg1 = arg2;
    arg2 = temp;
}

// bubble_sort
template<typename _BothIter, typename _Compare>
void bsort(_BothIter beg, _BothIter end, _Compare c)
{
    _BothIter max_pos = end;
    _BothIter min_pos = beg;
    for(_BothIter v = beg; v != end; ++v)
    {
        _BothIter current_pos = end;
        for(_BothIter b = min_pos, temp = ++_BothIter(b); temp != max_pos; ++b)
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
        for(_BothIter n = max_pos, temp = --_BothIter(n); n != min_pos; --n)
        {
            if(c(*n, *temp) > 0)
            {
                swap(*temp, *n);
                current_pos = n;
            }
        }
        ++min_pos;
        if(current_pos == end)
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
//    Node *last_pos = nullptr;
//    for(Node *v = _M_head->Next; v != nullptr; v = v->Next)
//    {
//        Node *current_pos = nullptr;
//        for(Node *b = _M_head->Next; b->Next != last_pos; b = b->Next)
//        {
//            Node *temp = b->Next;
//            if(c(temp->data(), b->data()) > 0)
//            {
//                temp->swap(b);
//                current_pos = temp;
//            }
//        }
//        last_pos = current_pos;
//        if(last_pos == nullptr)
//        {
//            break;
//        }
//    }
    _ForwardIter last_pos = end;
    for(_ForwardIter v = beg; v != end; ++v)
    {
        _ForwardIter current_pos = end;
        for(_ForwardIter b = beg, temp = b;
            (++temp) != last_pos;
            ++b)
        {
            if(c(*temp, *b) > 0)
            {
                swap(*temp, *b);
                current_pos = b;
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
