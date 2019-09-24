#ifndef COMPARE_H
#define COMPARE_H

namespace rapid
{

template<typename T1, typename T2>
struct Compare2
{
    int operator()(const T1 &arg1, const T2 &arg2) const
    {
        if(arg1 < arg2) return 1;
        if(arg2 < arg1) return -1;
        return 0;
    }
};

template<typename T>
struct Compare2<T, T>
{
    int operator()(const T &arg1, const T &arg2) const
    {
        if(arg1 < arg2) return 1;
        if(arg2 < arg1) return -1;
        return 0;
    }
};

template<typename T>
struct Compare : Compare2<T, T> { };

}
#endif // COMPARE_H
