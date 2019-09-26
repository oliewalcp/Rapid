#ifndef GENETIC_H
#define GENETIC_H

#include <functional>
#include "Core/TypeTraits.h"

namespace rapid
{

template<typename _RetureType,
         typename ... _Args>
std::function<_RetureType(const _Args & ...)> __Fitness;

template<typename _FitnessType>
struct Genetic
{
    template<typename _Func,
             typename ... _BoundArgs>
    void set_fitness_function(_Func && __f, _BoundArgs && ... __args)
    { __Fitness<_FitnessType, _BoundArgs...> = std::bind(__f, __args ...); }

    template<typename ... _Args>
    _FitnessType fitness(const _Args & ... __args)
    { return __Fitness<_FitnessType, _Args...>(__args ...); }

    template<typename ... _Args>
    _FitnessType fitness(_Args && ... __args)
    { return __Fitness<_FitnessType, _Args...>(forward<_Args>(__args)...); }
};




}

#endif // GENETIC_H
