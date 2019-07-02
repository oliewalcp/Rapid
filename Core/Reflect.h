#ifndef REFLECT_H
#define REFLECT_H

#include "Core/Version.h"
#include "Core/Memory.h"

namespace rapid
{

template<typename Type>
CONSTEXPR long address_of_virtual_func(Type *arg)
{
    return *reinterpret_cast<long *>(*reinterpret_cast<long *>(&arg));
};

class Reflect
{
private:
    void *__Func;

    inline void __init(const Reflect &arg)
    { __Func = arg.__Func; }

public:
    Reflect(void *func = nullptr) : __Func(func) { }
    Reflect(Reflect && arg)
    { __init(std::forward<Reflect>(arg)); }
    Reflect(const Reflect &arg)
    { __init(arg); }

    template<typename ReturnType, typename ... ArgsType>
    ReturnType invoke(ArgsType ... arg)
    {
        typedef ReturnType(*func)(ArgsType ...);
        return (*reinterpret_cast<func *>(__Func))(arg...);
    }

    Reflect operator=(const Reflect &arg)
    { __init(arg); }
};

};

#endif // REFLECT_H
