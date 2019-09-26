#ifndef SORTER_H
#define SORTER_H

#include "Core/Compare.h"
#include "Core/Map.h"
#include <type_traits> // std::declval
#include <initializer_list> // std::initializer_list
#include <cmath> // std::log2  std::pow
#include <ctime> // std::time

namespace rapid
{

typedef unsigned long long size_type;

template<typename T = int>
void generate_random(std::initializer_list<T*> arg, T size)
{
    Map<T, T> generator;
    T remain = size;
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    while(remain > 0)
    {
        T ret = rand() % remain;
        auto it = generator.find(ret);
        if(it == generator.end())
        {
            for(T *a : arg)
            {
                a[size - remain] = ret;
            }
            --remain;
            auto exi = generator.find(remain);
            if(exi == generator.end())
            {
                generator[ret] = remain;
            }
            else
            {
                generator[ret] = exi->Second;
            }
        }
        else
        {
            for(T *a : arg)
            {
                a[size - remain] = ret;
            }
            --remain;
            auto exi = generator.find(remain);
            if(exi == generator.end())
            {
                it->Second = remain;
            }
            else
            {
                it->Second = exi->Second;
            }
        }
    }
}

template<typename T>
inline void swap(T &arg1, T &arg2)
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
template<typename _ForwardIter,
         typename _Compare = Compare<decltype(*std::declval<_ForwardIter>())>>
void fsort(_ForwardIter beg,
           _ForwardIter end,
           _Compare c = _Compare())
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

// bubble_sort
// not contain [end]
template<typename _BothIter,
         typename _Compare = Compare<decltype(*std::declval<_BothIter>())>>
void bsort(_BothIter beg,
           _BothIter end,
           _Compare c = _Compare())
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

// insertion sort
// not contain [end]
template<typename _BothIter,
         typename _Compare = Compare<decltype(*std::declval<_BothIter>())>>
void isort(_BothIter beg,
           _BothIter end,
           _Compare c = _Compare())
{
    _BothIter bound = beg;
    for(++bound; bound != end; ++bound)
    {
        auto temp(*bound);
        _BothIter it = bound;
        _BothIter v = it;
        while(v != beg)
        {
            if(c(temp, *--v) > 0)
            {
                *it = *v;
                it = v;
            }
            else break;
        }
        *it = temp;
    }
}

// merge
// not contain [end]
template<typename _ForwardIter1,
         typename _ForwardIter2 = _ForwardIter1,
         typename _OutputForwardIter = _ForwardIter1,
         typename _Compare = Compare2<
             decltype(*std::declval<_ForwardIter1>()),
             decltype(*std::declval<_ForwardIter2>())> >
void merge(_OutputForwardIter dst,
           _ForwardIter1 src1_beg, _ForwardIter1 src1_end,
           _ForwardIter2 src2_beg, _ForwardIter2 src2_end,
           _Compare c = _Compare())
{
    while(src1_beg != src1_end && src2_beg != src2_end)
    {
        *dst++ = c(*src2_beg, *src1_beg) > 0 ? *src2_beg++ : *src1_beg++;
    }
    while(src1_beg != src1_end)
    {
        *dst++ = *src1_beg++;
    }
    while(src2_beg != src2_end)
    {
        *dst++ = *src2_beg++;
    }
}
// merge sort
// not contain [end]
template<typename _RandomIter,
         typename _ResultRandomIter = _RandomIter,
         typename _Compare = Compare<decltype(*std::declval<_RandomIter>())>>
void msort(_ResultRandomIter result,
           _RandomIter beg,
           _RandomIter end,
           _Compare c = _Compare())
{
    size_type center = distance(beg, end) / 2;
    if(center > 0)
    {
        msort(result, beg, beg + center, c);
        msort(result + center, beg + center, end, c);
        merge(result, beg, beg + center, beg + center, end, c);
        for(_RandomIter it = beg; it != end; ++it)
        {
            *it = *result++;
        }
    }
}
// merge sort
// not contain [end]
template<typename _RandomIter,
         typename _Compare = Compare<decltype(*std::declval<_RandomIter>())>>
void msort(_RandomIter beg,
           _RandomIter end,
           _Compare c = _Compare())
{
    using _IterValueType = decltype(*std::declval<_RandomIter>());
    using _ValueType = typename RemoveReference<_IterValueType>::type;

    size_type dist = distance(beg, end);
    _ValueType *result = new _ValueType[dist];
    msort(result, beg, end, c);
    for(size_type i = 0; i < dist; ++i)
    {
        *beg++ = result[i];
    }
    delete[] result;
}

// shell sort
// not contain [end]
template<typename _RandomIter,
         typename _Compare = Compare<decltype(*std::declval<_RandomIter>())>>
void ssort(_RandomIter beg,
           _RandomIter end,
           _Compare c = _Compare())
{
    size_type total_size = distance(beg, end);
    if(total_size <= 1) return;
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

// get median for quick sort
// not contain [end]
template<typename _RandomIter,
         typename _Compare = Compare<decltype(*std::declval<_RandomIter>())>>
auto median(_RandomIter beg,
            _RandomIter end,
            _Compare c = _Compare())
    -> decltype(*std::declval<_RandomIter>())
{
    size_type dist = distance(beg, end);
    size_type center = dist / 2;
    _RandomIter left = beg, right = beg + dist - 1;
    if(c(*(left + center), *left) > 0)
    {
        swap(*(left + center), *left);
    }
    if(c(*right, *left) > 0)
    {
        swap(*right, *left);
    }
    if(c(*right, *(left + center)) > 0)
    {
        swap(*right, *(left + center));
    }
    swap(*(left + center), *(right - 1));
    return *(right - 1);
}

// quick sort
// not contain [end]
template<typename _RandomIter,
         typename _Compare = Compare<decltype(*std::declval<_RandomIter>())>>
void qsort(_RandomIter beg,
           _RandomIter end,
           _Compare c = _Compare())
{
    size_type right = distance(beg, end) - 1;
    if(right >= 10)
    {
        size_type i = 0, j = right - 1;

        auto pivot(median(beg, end, c));

        while(true)
        {
            while(c(*(beg + (++i)), pivot) > 0);
            while(c(pivot, *(beg + (--j))) > 0);
            if(i < j)
            {
                swap(*(beg + i), *(beg + j));
            }
            else break;
        }
        swap(*(beg + i), *(beg + right - 1));

        qsort(beg, beg + i);
        qsort(beg + i + 1, end);
    }
    else
    {
        isort(beg, end, c);
    }
}

}

#endif // SORTER_H
