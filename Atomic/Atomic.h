#ifndef ATOMIC_H
#define ATOMIC_H

#ifdef _X86
#define compiler_output message
#endif

#if __cplusplus < 201103L
#error please support C++11
#endif

#if defined(__clang__)
    /* Clang/LLVM. ---------------------------------------------- */

#elif defined(__ICC) || defined(__INTEL_COMPILER)
    /* Intel ICC/ICPC. ------------------------------------------ */

#elif defined(__GNUC__) || defined(__GNUG__)

//c++17 begin at gcc 7.1.0
//c++11 begin at gcc 4.7.0
#if __GNUC__ < 4 && __GNUC_MINOR__ < 7 && __GNUC_PATCHLEVEL__ < 0
#error please support C++11
#endif

#define sync_fetch_before_add __sync_fetch_and_add   //return value before add
#define sync_fetch_before_sub __sync_fetch_and_sub   //return value before sub
#define sync_fetch_before_or __sync_fetch_and_or
#define sync_fetch_before_and __sync_fetch_and_and
#define sync_fetch_before_xor __sync_fetch_and_xor
#define sync_fetch_before_nand __sync_fetch_and_nand

#define sync_fetch_after_add __sync_add_and_fetch   //return value before add
#define sync_fetch_after_sub __sync_sub_and_fetch   //return value before sub
#define sync_fetch_after_or __sync_or_and_fetch
#define sync_fetch_after_and __sync_and_and_fetch
#define sync_fetch_after_xor __sync_xor_and_fetch
#define sync_fetch_after_nand __sync_nand_and_fetch

#define sync_bool_compare_and_swap __sync_bool_compare_and_swap  //return compare result and set value
#define sync_value_compare_and_swap __sync_val_compare_and_swap  //return value before compare

#elif defined(__HP_cc) || defined(__HP_aCC)
    /* Hewlett-Packard C/aC++. ---------------------------------- */

#elif defined(__IBMC__) || defined(__IBMCPP__)
    /* IBM XL C/C++. -------------------------------------------- */

#elif defined(_MSC_VER)
    /* Microsoft Visual Studio. --------------------------------- */

#elif defined(__PGI)
    /* Portland Group PGCC/PGCPP. ------------------------------- */

#elif defined(__SUNPRO_C) || defined(__SUNPRO_CC)
    /* Oracle Solaris Studio. ----------------------------------- */

#endif

#include <ostream>
#include <string.h>

template<typename T>
class Atomic
{
private:
    T data;
    static constexpr void type_assert()
    {
        static_assert(
            std::is_same<T, char>::value ||
            std::is_same<T, unsigned char>::value ||
            std::is_same<T, short>::value ||
            std::is_same<T, unsigned short>::value ||
            std::is_same<T, int>::value ||
            std::is_same<T, unsigned int>::value ||
            std::is_same<T, long>::value ||
            std::is_same<T, unsigned long>::value ||
            std::is_same<T, long long>::value ||
            std::is_same<T, unsigned long long>::value,
            "only support integral type");
    }
public:
    Atomic()
    {
        type_assert();
        memset(&data, 0, sizeof(T));
    }
    Atomic(T value)
    {
        type_assert();
        data = value;
    }

    T add_and_fetch(T value)
    { return sync_fetch_after_add(&data, value); }
    T sub_and_fetch(T value)
    { return sync_fetch_after_sub(&data, value); }

    bool operator==(T value)
    { return sync_bool_compare_and_swap(&data, value, data); }

    T operator++(int)
    { return sync_fetch_before_add(&data, 1); }
    T operator++()
    { return sync_fetch_after_add(&data, 1); }
    T operator--(int)
    { return sync_fetch_before_sub(&data, 1); }
    T operator--()
    { return sync_fetch_after_sub(&data, 1); }

    T operator+=(T value)
    { return sync_fetch_after_add(&data, value); }
    T operator-=(T value)
    { return sync_fetch_after_sub(&data, value); }
    T operator|=(T value)
    { return sync_fetch_after_or(&data, value); }
    T operator&=(T value)
    { return sync_fetch_after_and(&data, value); }
    T operator^=(T value)
    { return sync_fetch_after_xor(&data, value); }

    friend std::ostream& operator<<(std::ostream &os, Atomic<T> &a)
    { return os << sync_value_compare_and_swap(&a.data, 0, a.data); }
};

#endif // ATOMIC_H
