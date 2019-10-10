#ifndef BITSET_H
#define BITSET_H

#include "Core/Version.h"

namespace rapid
{

#if __cplusplus > 201703L
template<size_type _Num>
concept std_memory = requires { _Num % 2 == 0; }
#endif

template<size_type _Num,
         size_type _UnitBit = 1>
#if __cplusplus > 201703L
    requires std_memory<_UnitBit>
#endif
struct Bitset
{
    static constexpr size_type _S_num = _Num;
    static constexpr size_type _S_unit_bit = _UnitBit;
    static constexpr size_type _S_total_bit = _S_num * _S_unit_bit;
    static constexpr size_type _S_total_byte = _S_total_bit / 8 + _S_total_bit % 8 > 0 ? 1 : 0;

    using byte = unsigned char;

    byte _M_data[_S_total_byte];

    void set_value();
};

}

#endif // BITSET_H
