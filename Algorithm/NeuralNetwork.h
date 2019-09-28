#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <cmath>
#include <functional>
#include <vector>

namespace rapid
{

inline double sigmoid(double x)
{ return 1 / (1 + pow(M_E, -x)); }
template<typename _T1 = double,
         typename _T2 = _T1,
         typename _R = _T1>
inline double multiple(double x, double y)
{ return x * y; }
template<typename _T = double>
inline _T sum(_T __x, _T __y)
{ return __x + __y; }

template<typename _ResultType,
         typename ... _Args>
std::function<_ResultType(_Args...)> __Activation;
template<typename _ResultType,
         typename ... _Args>
std::function<_ResultType(_Args...)> __Multiple;
template<typename _ResultType,
         typename ... _Args>
std::function<_ResultType(_Args...)> __Sum;

template<typename _DataType = double,
         typename _WeightType = double>
struct alignas(8) BPNeuralNetwork
{
    template<typename T>
    using MatrixType = std::vector<T>;

    using DataType = _DataType;
    using DataGroup = MatrixType<DataType>;
    using SizeType = unsigned long long;
    using WeightType = _WeightType;
    using WeightGroupItem = MatrixType<WeightType>;
    using WeightGroup = MatrixType<WeightGroupItem*>;
    using WeightData = MatrixType<WeightGroup*>;

    WeightData _M_weight;
    WeightGroup _M_output_weight;
    SizeType _M_feedback = 0;

    BPNeuralNetwork()
    {
        std::function<double(double)> f1 = std::bind(&sigmoid, std::placeholders::_1);
        set_activation(f1);
        std::function<double(double, double)> f2 =
                std::bind(&multiple<DataType, DataType, WeightType>,
                          std::placeholders::_1,
                          std::placeholders::_2);
        set_multiple(f2);
        std::function<double(double, double)> f3 =
                std::bind(&sum<DataType>,
                          std::placeholders::_1,
                          std::placeholders::_2);
        set_sum(f3);
        set_hide_layer_number(1);
    }
    ~BPNeuralNetwork()
    { clear(); }

    void clear()
    {
        _F_realloc(_M_weight);
        _F_realloc(_M_output_weight);
        set_feedback_number(0);
    }

    /*
     * the number of [__in] is same as [__out]'s
     * the number of each group of [__in] or [__out] are same
     */
    void train(std::initializer_list<DataType> __in,
               std::initializer_list<DataType> __out)
    {
        train(__in.begin(), __in.end(), __in.size(),
              __out.begin(), __out.end(), __out.size());
    }

    /*
     * train one group for one time. so, you should call for multiple times.
     */
    template<typename _InputIterator,
             typename _OutputIterator>
    bool train(_InputIterator __ibegin, _InputIterator __iend, SizeType __is,
               _OutputIterator __obegin, _OutputIterator __oend, SizeType __os)
    {
        if(_M_weight.size() == 0 || __is != _M_weight[0]->size())
        {
            return false;
        }

        for(SizeType i = 1; i < _M_weight.capacity(); ++i)
        {
            WeightGroup *group = new WeightGroup(__is);
            for(SizeType j = 0; j < __is; ++j)
            {
                group->push_back(new WeightGroupItem(__is));
            }
            _M_weight.push_back(group);
        }
        _F_realloc(_M_output_weight, __is);
        for(SizeType i = 0; i < __is; ++i)
        {
            _M_output_weight.push_back(new WeightGroupItem(__os));
        }

        DataGroup __input, __output;
        _F_realloc(__input, __is);
        _F_realloc(__output, __os);
        for(auto it = __ibegin; it != __iend; ++it)
        {
            __input.push_back(*it);
        }
        for(auto it = __obegin; it != __oend; ++it)
        {
            __output.push_back(*it);
        }
        _F_train(__input, __output);
        return true;
    }

    template<typename _ResultType,
             typename ... _Args>
    void set_activation(std::function<_ResultType(_Args...)> &&__f)
    { __Activation<_ResultType, _Args...> = __f; }

    template<typename _ResultType,
             typename ... _Args>
    void set_activation(const std::function<_ResultType(_Args...)> &__f)
    { __Activation<_ResultType, _Args...> = __f; }

    template<typename _ResultType,
             typename ... _Args>
    void set_multiple(std::function<_ResultType(_Args...)> &&__f)
    { __Multiple<_ResultType, _Args...> = __f; }

    template<typename _ResultType,
             typename ... _Args>
    void set_multiple(const std::function<_ResultType(_Args...)> &__f)
    { __Multiple<_ResultType, _Args...> = __f; }

    template<typename _ResultType,
             typename ... _Args>
    void set_sum(std::function<_ResultType(_Args...)> &&__f)
    { __Sum<_ResultType, _Args...> = __f; }

    template<typename _ResultType,
             typename ... _Args>
    void set_sum(const std::function<_ResultType(_Args...)> &__f)
    { __Sum<_ResultType, _Args...> = __f; }

    void set_hide_layer_number(SizeType __s)
    {
        if(__s == 0) return;
        _M_weight.resize(__s);
    }

    void set_feedback_number(SizeType __s)
    { _M_feedback = __s; }

    void set_input_weight(std::initializer_list<std::initializer_list<WeightType>> __w)
    { set_input_weight(__w.begin(), __w.end(), __w.size()); }

    template<typename _InputIterator>
    void set_input_weight(_InputIterator __begin,
                          _InputIterator __end,
                          SizeType __s)
    {
        if(_M_weight.capacity() != __s)
        {
            _F_realloc(_M_weight, __s);
        }
        if(_M_weight.size() == 0)
        {
            _M_weight.push_back(new WeightGroup(__s));
        }
        _F_set_input_weight(*(_M_weight[0]), __begin, __end, __s);
    }
    SizeType layer_number() const
    { return _M_weight.capacity(); }
    SizeType input_number() const
    { return _M_weight.at(0)->at(0)->capacity(); }
    SizeType output_number() const
    { return _M_output_weight.at(0)->capacity(); }

    DataGroup predict(std::initializer_list<DataType> __d) const
    { return predict(__d.begin(), __d.end()); }
    template<typename _InputIterator>
    DataGroup predict(_InputIterator __begin, _InputIterator) const
    {
        DataGroup __input(input_number()), __output(output_number());
        for(SizeType __i = 0; __i < input_number(); ++__i)
        {
            __input.push_back(__begin++);
        }
        DataGroup __out(input_number());
        _F_input_to_hide(__input, __out);
        _F_hide_to_output(__out, __output);
        return __output;
    }

    WeightType _F_weight(SizeType __layer, SizeType __item, SizeType __index) const
    {
        if(__layer >= layer_number())
        {
            return _M_output_weight.at(__item)->at(__index);
        }
        return _M_weight.at(__layer)->at(__item)->at(__index);
    }

    void _F_layer_to_layer(DataGroup &__in,
                           DataGroup &__out,
                           SizeType __layer,
                           SizeType __item_num,
                           SizeType __data_num) const
    {
        for(SizeType j = 0; j < __item_num; ++j)
        {
            __out[j] = __Multiple<DataType, DataType, WeightType>(
                            __in[0],
                            _F_weight(__layer, j, 0));
            for(SizeType n = 1; n < __data_num; ++n)
            {
                __out[j] = __Sum<DataType, DataType, DataType>(
                                __out[j],
                                __Multiple<DataType, DataType, WeightType>(
                                    __in[n],
                                    _F_weight(__layer, j, n)));
            }
        }
    }

    void _F_input_to_hide(DataGroup &__in, DataGroup &__out) const
    {
        if(__out.capacity() != input_number())
        {
            return;
        }
        DataGroup __temp(input_number());
        DataGroup *__data[2] = {&__in, &__temp};
        SizeType i;
        for(i = 0; i < layer_number(); ++i)
        {
            DataGroup &__this = *__data[i % 2];
            DataGroup &__next = *__data[(i + 1) % 2];
            _F_layer_to_layer(__this, __next, i, input_number(), input_number());
        }
        __out.swap(*__data[i % 2]);
    }

    void _F_hide_to_output(DataGroup &__h, DataGroup &__o) const
    {
        if(__o.capacity() != output_number())
        {
            return;
        }
        _F_layer_to_layer(__h, __o, layer_number(), output_number(), input_number());
    }

    template<typename _InputIterator>
    void _F_set_input_weight(WeightGroup &__c,
                             _InputIterator __begin,
                             _InputIterator __end,
                             SizeType __s)
    {
        _F_realloc(__c, __s);
        for(auto it1 = __begin; it1 != __end; ++it1)
        {
            WeightGroupItem *group = new WeightGroupItem(__s);
            for(auto it2 = (*it1).begin(); it2 != (*it1).end(); ++it2)
            {
                group->push_back(*it2);
            }
            __c.push_back(group);
        }
    }

    template<typename T>
    void _F_realloc(T &arg, SizeType s = 0)
    {
        if
    #if __cplusplus >= 201703L
                constexpr
    #endif
                (std::is_pointer<typename T::value_type>::value)
        {
            for(auto __i : arg)
            {
                if
            #if __cplusplus >= 201703L
                        constexpr
            #endif
                        (std::is_pointer<typename T::value_type>::value)
                {
                    _F_realloc(*__i);
                }
                delete __i;
            }
        }
        T empty(s);
        arg.swap(empty);
    }

    void _F_train(DataGroup &__input, DataGroup &__output)
    {
        DataGroup __temp(input_number());
        for(SizeType i = 0; i < _M_feedback; ++i)
        {
            _F_input_to_hide(__input, __temp);
            _F_hide_to_output(__temp, __output);
        }
    }
};

}

#endif // NEURALNETWORK_H
