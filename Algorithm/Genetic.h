#ifndef GENETIC_H
#define GENETIC_H

#include <iostream>
#include <functional>

namespace rapid
{

template<typename _ResultType,
         typename ... _Args>
std::function<_ResultType(_Args...)> __Fitness;

template<typename _ResultType,
         typename ... _Args>
std::function<_ResultType(_Args...)> __Cross;

template<typename _ResultType,
         typename ... _Args>
std::function<_ResultType(_Args...)> __Variation;

template<typename _FitnessType,
         typename _CharacteristicsType>
struct _Selector
{
    int operator()(const _CharacteristicsType &__c1, const _CharacteristicsType &__c2)
    {
        _FitnessType __f1 = __Fitness<_FitnessType, _CharacteristicsType>(__c1);
        _FitnessType __f2 = __Fitness<_FitnessType, _CharacteristicsType>(__c2);
        return __f2 < __f1;
//        if(__f2 < __f1) return 1;
//        if(__f1 < __f2) return -1;
//        return 0;
    }
};

template<typename _FitnessType,
         typename _CharacteristicsType>
struct Genetic
{
    using FitnessType = _FitnessType;
    using CharacteristicsType = _CharacteristicsType;
    using SizeType = unsigned long long;
    using FitnessFunctionType = std::function<FitnessType(CharacteristicsType)>;
    using CrossFunctionType = std::function<void(CharacteristicsType*, SizeType)>;
    using VariationFunctionType = std::function<void(CharacteristicsType*, SizeType)>;

    CharacteristicsType *_M_characteristics = nullptr;
    SizeType _M_char_size;

    ~Genetic()
    {
        if(_M_characteristics != nullptr)
        { delete[] _M_characteristics; }
    }

    SizeType capacity() const
    { return _M_char_size; }

    CharacteristicsType operator[](SizeType __i) const
    { return _M_characteristics[__i]; }

    void init(CharacteristicsType *__c, SizeType __capacity)
    {
        _M_characteristics = new CharacteristicsType[__capacity];
        for(SizeType i = 0; i < __capacity; ++i)
        { _M_characteristics[i] = __c[i]; }
        _M_char_size = __capacity;
    }

    void set_fitness_function(FitnessFunctionType &&__f)
    { __Fitness<FitnessType, CharacteristicsType> = std::forward<FitnessFunctionType>(__f); }

    void set_fitness_function(const FitnessFunctionType &__f)
    { __Fitness<FitnessType, CharacteristicsType> = __f; }

    FitnessType fitness(const CharacteristicsType &__args)
    { return __Fitness<FitnessType, CharacteristicsType>(__args); }

    FitnessType fitness(CharacteristicsType &&__args)
    { return __Fitness<FitnessType, CharacteristicsType>(std::forward<CharacteristicsType>(__args)); }

    void set_cross_function(CrossFunctionType &&__f)
    { __Cross<void, CharacteristicsType*, SizeType> = std::forward<CrossFunctionType>(__f); }

    void set_cross_function(const CrossFunctionType &__f)
    { __Cross<void, CharacteristicsType*, SizeType> = __f; }

    void set_variation_function(VariationFunctionType &&__f)
    { __Variation<void, CharacteristicsType*, SizeType> = std::forward<VariationFunctionType>(__f); }

    void set_variation_function(const VariationFunctionType &__f)
    { __Variation<void, CharacteristicsType*, SizeType> = __f; }

    void reproduce(SizeType __generation, SizeType __increment)
    {
        CharacteristicsType *temp = new CharacteristicsType[_M_char_size + __increment];
        for(SizeType j = 0; j < _M_char_size; ++j)
        {
            temp[j] = _M_characteristics[j];
        }
        _Selector<FitnessType, CharacteristicsType> __s;
        for(SizeType i = 0; i < __generation; ++i)
        {
            __Cross<void, CharacteristicsType*, SizeType>(temp, _M_char_size + __increment);
            __Variation<void, CharacteristicsType*, SizeType>(temp, _M_char_size + __increment);
            std::sort(temp, temp + _M_char_size + __increment, __s);
        }
        for(SizeType j = 0; j < _M_char_size; ++j)
        {
            _M_characteristics[j] = temp[j];
        }
    }

};




}

#endif // GENETIC_H
