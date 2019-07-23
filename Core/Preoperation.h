#ifndef PREOPERATION_H
#define PREOPERATION_H

#include "Version.h"

namespace rapid
{

template<typename T, unsigned long size>
constexpr unsigned long get_array_length(const T (&)[size])
{ return size; }

#if __cplusplus >= 201703L

// calculate the sum of some number
template<typename T, T arg, T ... Num>
struct Sum
{
private:
    static constexpr T __get()
    {
        if constexpr (sizeof... (Num) == 0)
            return arg;
        else
            return arg + Sum<T, Num...>::value;
    }
public:
    static constexpr T value = __get();
};


// get the value of the [NeedIndex] index in the variable-length argument
// [NeedIndex] begin at 0
template<typename T, unsigned long NeedIndex, T ... arg>
struct Index
{
private:
    template<unsigned CurrentIndex, T now, T ... args>
    struct __Index
    {
    private:
        static constexpr T __get()
        {
            if constexpr (CurrentIndex == NeedIndex)
                return now;
            else
                return __Index<CurrentIndex + 1, args...>::value;
        }
    public:
        static constexpr T value = __get();
    };
public:
    static_assert (NeedIndex < sizeof... (arg), "Index is too large");
    static constexpr T value = __Index<0, arg...>::value;
};

#ifndef NDEBUG
void test_Sum_main()
{
    std::cout << "the sum of 3, 5, 10, -10, 100 is: " << Sum<int, 3, 5, 10, -10, 100>::value << std::endl;
}

void test_Index_main()
{
    std::cout << "the index of 1 in 3, 5, 10, -10, 200 is: " << Index<int, 1, 3, 5, 10, -10, 200>::value << std::endl;
}
#endif
#endif
};

#endif // PREOPERATION_H
