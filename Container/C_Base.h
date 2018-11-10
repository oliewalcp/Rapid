#ifndef C_BASE_H
#define C_BASE_H

template<typename T>
struct _C_Base
{
    T Data;
    ~_C_Base()
    {
        Data.~T();
    }
};

struct Compare
{
    template<typename T1, typename T2>
    bool operator()(const T1& arg1, const T2& arg2) const
    { return arg1 < arg2; }
};

#endif // C_BASE_H
