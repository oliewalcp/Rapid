#ifndef REFLECT_H
#define REFLECT_H

#include "Core/Version.h"
#include "Core/Memory.h"

namespace rapid
{

class Reflect
{
private:
    long __Func;

    inline void __init(const Reflect &arg)
    { __Func = arg.__Func; }

public:
    Reflect(long address = 0) : __Func(address) { }

    Reflect(Reflect && arg)
    { __init(std::forward<Reflect>(arg)); }
    Reflect(const Reflect &arg)
    { __init(arg); }

    template<typename ReturnType, typename ... ArgsType>
    ReturnType invoke(ArgsType ... arg)
    {
        using func = ReturnType(*)(ArgsType...);
        return (*reinterpret_cast<func *>(__Func))(arg...);
    }

    Reflect operator=(const Reflect &arg)
    {
        __init(arg);
        return *this;
    }

    Reflect operator=(long address)
    {
        __Func = address;
        return *this;
    }
};

#ifndef NDEBUG
//void foo()
//{
//    std::cout << __func__ << std::endl;
//}
void test_Reflect_main()
{
//    Reflect rf(&foo);
}
#endif

};

#endif // REFLECT_H
