#ifndef ATOMIC_H
#define ATOMIC_H

#include "Version.h"
#include "TypeTraits.h"

namespace rapid
{

#ifdef _X86
#define compiler_output message
#endif

//#if defined(__clang__)
//    /* Clang/LLVM. ---------------------------------------------- */

//#elif defined(__ICC) || defined(__INTEL_COMPILER)
//    /* Intel ICC/ICPC. ------------------------------------------ */

//#elif defined(__GNUC__) || defined(__GNUG__)

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

//#elif defined(__HP_cc) || defined(__HP_aCC)
//    /* Hewlett-Packard C/aC++. ---------------------------------- */

//#elif defined(__IBMC__) || defined(__IBMCPP__)
//    /* IBM XL C/C++. -------------------------------------------- */

//#elif defined(_MSC_VER)
//    /* Microsoft Visual Studio. --------------------------------- */

//#elif defined(__PGI)
//    /* Portland Group PGCC/PGCPP. ------------------------------- */

//#elif defined(__SUNPRO_C) || defined(__SUNPRO_CC)
//    /* Oracle Solaris Studio. ----------------------------------- */

//#endif

template<typename T>
class Atomic
{
private:
    T _M_data;
    static constexpr void type_assert()
    {
        static_assert(
            IsSame<T, bool>::value ||
            IsSame<T, char>::value ||
            IsSame<T, unsigned char>::value ||
            IsSame<T, short>::value ||
            IsSame<T, unsigned short>::value ||
            IsSame<T, int>::value ||
            IsSame<T, unsigned int>::value ||
            IsSame<T, long>::value ||
            IsSame<T, unsigned long>::value ||
            IsSame<T, long long>::value ||
            IsSame<T, unsigned long long>::value ||
            IsSame<T, wchar_t>::value,
            "only support base data type");
    }
public:
    Atomic()
    {
        type_assert();
        memset(&_M_data, 0, sizeof(T));
    }
    Atomic(T value)
    {
        type_assert();
        _M_data = value;
    }

    T add_and_fetch(T value)
    { return sync_fetch_after_add(&_M_data, value); }
    T sub_and_fetch(T value)
    { return sync_fetch_after_sub(&_M_data, value); }

    bool operator==(T value)
    { return sync_bool_compare_and_swap(&_M_data, value, _M_data); }

    T operator++(int)
    { return sync_fetch_before_add(&_M_data, 1); }
    T operator++()
    { return sync_fetch_after_add(&_M_data, 1); }
    T operator--(int)
    { return sync_fetch_before_sub(&_M_data, 1); }
    T operator--()
    { return sync_fetch_after_sub(&_M_data, 1); }

    T operator+=(T value)
    { return sync_fetch_after_add(&_M_data, value); }
    T operator-=(T value)
    { return sync_fetch_after_sub(&_M_data, value); }
    T operator|=(T value)
    { return sync_fetch_after_or(&_M_data, value); }
    T operator&=(T value)
    { return sync_fetch_after_and(&_M_data, value); }
    T operator^=(T value)
    { return sync_fetch_after_xor(&_M_data, value); }

    T operator=(T value)
    { sync_value_compare_and_swap(&_M_data, _M_data, value); }

    friend std::ostream& operator<<(std::ostream &os, Atomic<T> &a)
    { return os << sync_value_compare_and_swap(&a._M_data, 0, a._M_data); }
};

}

#endif // ATOMIC_H
