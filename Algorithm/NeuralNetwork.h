#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <cmath>
#include <functional>
#include <vector>

namespace rapid
{

using ErrorType = double;

inline double sigmoid(double x)
{ return 1 / (1 + pow(M_E, -x)); }
inline double tanh(double __x)
{ return std::tanh(__x); }
inline constexpr double softsign(double __x)
{ return __x / (1 + __x > 0 ? __x : -__x); }

inline double softsign_d(double __x)
{ return 1 / pow(1 + __x > 0 ? __x : -__x, 2); }
inline double tanh_d(double __x)
{ return 1 - pow(tanh(__x), 2); }

template<typename _T = double>
inline _T output_residual(_T __output, _T __sample)
{ return -(__output - __sample) * __output * (1 - __output); }

template<typename _T = double>
inline _T hide_residual(_T __pre_residual, _T __sigmoid)
{ return -__pre_residual * __sigmoid * (1 - __sigmoid); }

template<typename _T1 = double,
         typename _T2 = _T1,
         typename _R = _T1>
inline double multiple(double x, double y)
{ return x * y; }

template<typename _T = double>
inline _T sum(_T __x, _T __y)
{ return __x + __y; }

template<typename _T = double>
inline ErrorType error(_T __1, _T __2)
{ return pow(__1 - __2, 2); }

template<typename _ResultType,
         typename ... _Args>
std::function<_ResultType(_Args...)> __Activation;
template<typename _ResultType,
         typename ... _Args>
std::function<_ResultType(_Args...)> __Multiple;
template<typename _ResultType,
         typename ... _Args>
std::function<_ResultType(_Args...)> __Sum;
template<typename _ResultType,
         typename ... _Args>
std::function<_ResultType(_Args...)> __Error;
template<typename _ResultType,
         typename ... _Args>
std::function<_ResultType(_Args...)> __OResidual;
template<typename _ResultType,
         typename ... _Args>
std::function<_ResultType(_Args...)> __HResidual;

template<typename _DataType = double,
         typename _WeightType = double>
struct alignas(8) BPNeuralNetwork
{
    static constexpr ErrorType _ERROR_BOUND_ = 0.0001;
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
    SizeType _M_generation = 0;
    double _M_learn_efficency = 0.6;

    BPNeuralNetwork()
    {
        std::function<double(double)> f1 = std::bind(&sigmoid, std::placeholders::_1);
        set_activation(f1);
        std::function<double(double, double)> f2 =
                std::bind(&multiple<double, double, double>,
                          std::placeholders::_1,
                          std::placeholders::_2);
        set_multiple(f2);
        f2 = std::bind(&sum<double>,
                       std::placeholders::_1,
                       std::placeholders::_2);
        set_sum(f2);
        f2 = std::bind(&output_residual<double>,
                       std::placeholders::_1,
                       std::placeholders::_2);
        set_output_residual(f2);
        f2 = std::bind(&hide_residual<double>,
                       std::placeholders::_1,
                       std::placeholders::_2);
        set_hide_residual(f2);
        std::function<ErrorType(double, double)> f3 =
                std::bind(&error<double>,
                          std::placeholders::_1,
                          std::placeholders::_2);
        set_error(f3);
        set_hide_layer_number(1);
    }
    ~BPNeuralNetwork()
    { _F_clear(); }
    void clear()
    {
        _F_realloc(_M_weight, 1);
        _F_realloc(_M_output_weight);
        set_generation_number(0);
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

    template<typename _ResultType,
             typename ... _Args>
    void set_error(std::function<_ResultType(_Args...)> &&__f)
    { __Error<_ResultType, _Args...> = __f; }

    template<typename _ResultType,
             typename ... _Args>
    void set_error(const std::function<_ResultType(_Args...)> &__f)
    { __Error<_ResultType, _Args...> = __f; }

    template<typename _ResultType,
             typename ... _Args>
    void set_output_residual(std::function<_ResultType(_Args...)> &&__f)
    { __OResidual<_ResultType, _Args...> = __f; }

    template<typename _ResultType,
             typename ... _Args>
    void set_output_residual(const std::function<_ResultType(_Args...)> &__f)
    { __OResidual<_ResultType, _Args...> = __f; }

    template<typename _ResultType,
             typename ... _Args>
    void set_hide_residual(std::function<_ResultType(_Args...)> &&__f)
    { __HResidual<_ResultType, _Args...> = __f; }

    template<typename _ResultType,
             typename ... _Args>
    void set_hide_residual(const std::function<_ResultType(_Args...)> &__f)
    { __HResidual<_ResultType, _Args...> = __f; }

    void set_learning_efficency(double __e)
    { _M_learn_efficency = __e; }

    void set_hide_layer_number(SizeType __s)
    {
        if(__s == 0) return;
        _M_weight.resize(__s);
    }
    // param[__s]: 0--auto
    void set_generation_number(SizeType __s)
    { _M_generation = __s; }

    // param[__layer]: index begin at 0
    void set_weight(SizeType __layer,
                    std::initializer_list<std::initializer_list<WeightType>> __w)
    { set_weight(__layer, __w.begin(), __w.end(), __w.size()); }

    // param[__layer]: index begin at 0
    template<typename _InputIterator>
    void set_weight(SizeType __layer,
                    _InputIterator __begin,
                    _InputIterator __end,
                    SizeType __s)
    {
        if(_M_weight.capacity() <= __layer)
        {
            WeightData __weight(_M_weight);
            __weight.resize(layer_number() + 1);
            _M_weight.swap(__weight);
        }
        if(_M_weight.size() <= __layer)
        {
            WeightGroup *__group = new WeightGroup(__s);
            __group->clear();
            _M_weight.push_back(__group);
        }
        _F_set_weight(*(_M_weight[__layer]), __begin, __end, __s);
    }
    void set_output_weight(std::initializer_list<std::initializer_list<WeightType>> __w)
    { set_output_weight(__w.begin(), __w.end(), __w.size()); }
    template<typename _InputIterator>
    void set_output_weight(_InputIterator __begin, _InputIterator __end, SizeType __s)
    { _F_set_weight(_M_output_weight, __begin, __end, __s); }

    SizeType layer_number() const
    { return _M_weight.capacity(); }
    SizeType input_number() const
    { return _M_weight.at(0)->at(0)->capacity(); }
    SizeType output_number() const
    { return _M_output_weight.at(0)->capacity(); }

    DataGroup predict(std::initializer_list<DataType> __d) const
    { return predict(__d.begin(), __d.end()); }
    template<typename _InputIterator>
    DataGroup predict(_InputIterator __begin, _InputIterator = _InputIterator()) const
    {
        DataGroup __input(input_number()), __output(output_number());
        for(SizeType __i = 0; __i < input_number(); ++__i, ++__begin)
        {
            __input.push_back(*__begin);
        }
        _F_input_to_output(__input, __output);
        return __output;
    }

    void _F_clear()
    {
        _F_realloc(_M_weight);
        _F_realloc(_M_output_weight);
        set_generation_number(0);
    }
    WeightType _F_weight(SizeType __layer, SizeType __item, SizeType __index) const
    {
        if(__layer >= layer_number())
        {
            return _M_output_weight.at(__item)->at(__index);
        }
        return _M_weight.at(__layer)->at(__item)->at(__index);
    }

    void _F_layer_forward(const DataGroup &__in,
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
            __out[j] = __Activation<DataType, DataType>(__out[j]);
        }
    }

    void _F_layer_backward(const DataGroup &__in,
                           DataGroup &__out,
                           SizeType __layer,
                           SizeType __item_num,
                           SizeType __data_num) const
    {
        for(SizeType j = 0; j < __item_num; ++j)
        {
            DataType __sigmoid = __out[j];
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
            __out[j] = __HResidual<DataType, DataType, DataType>(__out[j], __sigmoid);
        }
    }

    void _F_input_to_output(const DataGroup &__in, DataGroup &__out) const
    {
        DataGroup __temp(input_number());
        _F_input_to_hide(__in, __temp);
        _F_hide_to_output(__temp, __out);
    }
    void _F_input_to_hide(const DataGroup &__in, DataGroup &__out) const
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
            _F_layer_forward(__this, __next, i, input_number(), input_number());
        }
        __out.swap(*__data[i % 2]);
    }

    void _F_hide_to_output(const DataGroup &__h, DataGroup &__o) const
    {
        if(__o.capacity() != output_number())
        {
            return;
        }
        _F_layer_forward(__h, __o, layer_number(), output_number(), input_number());
    }

    void _F_get_activation(const DataGroup &__in,
                        DataGroup &__out,
                        SizeType __layer,
                        SizeType __item_num,
                        SizeType __data_num) const
    {
        _F_realloc(__out, __item_num);
        _F_layer_forward(__in, __out, __layer, __item_num, __data_num);
    }

    template<typename _InputIterator>
    void _F_set_weight(WeightGroup &__c,
                       _InputIterator __begin,
                       _InputIterator __end,
                       SizeType __s) const
    {
        _F_realloc(__c, __s);
        for(auto it1 = __begin; it1 != __end; ++it1)
        {
            WeightGroupItem *group = new WeightGroupItem(__s);
            group->clear();
            for(auto it2 = (*it1).begin(); it2 != (*it1).end(); ++it2)
            {
                group->push_back(*it2);
            }
            __c.push_back(group);
        }
    }

    template<typename T>
    void _F_realloc(T &arg, SizeType s = 0) const
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
        arg.clear();
    }
    // get the activated value of each layer
    void _F_calculate_activated_value(const DataGroup &__input,
                                      DataGroup *__hide_layer_sigmoid,
                                      DataGroup &__out) const
    {
        _F_get_activation(__input, __hide_layer_sigmoid[0],
                          0, input_number(), input_number());
        for(SizeType l = 1; l < layer_number(); ++l)
        {
            _F_get_activation(__hide_layer_sigmoid[l - 1], __hide_layer_sigmoid[l],
                              l, input_number(), input_number());
        }
        _F_get_activation(__hide_layer_sigmoid[layer_number() - 1], __out,
                          layer_number(), output_number(), input_number());
    }
    // calculate the error.
    bool _F_calculate_error(const DataGroup &__output, const DataGroup &__out) const
    {
        if(_M_generation == 0) return false;
        for(SizeType n = 0; n < output_number(); ++n)
        {
            ErrorType __e = __Error<DataType, DataType, DataType>(__output[n], __out[n]);
            if(__e > _ERROR_BOUND_)
            {
                return false;
            }
        }
        return true;
    }
    // calculate the residual.
    void _F_calculate_residual(const DataGroup &__output,
                               DataGroup *__hide_layer_sigmoid,
                               DataGroup &__out) const
    {
        for(SizeType j = 0; j < output_number(); ++j)
        {
            __out[j] = __OResidual<DataType, DataType, DataType>(__out[j], __output[j]);
        }
        _F_layer_backward(__out, __hide_layer_sigmoid[layer_number() - 1],
                          layer_number(), input_number(), output_number());
        for(SizeType l = layer_number() - 1; l > 0; --l)
        {
            _F_layer_backward(__hide_layer_sigmoid[l], __hide_layer_sigmoid[l - 1],
                              l, input_number(), input_number());
        }
    }
    void _F_train(const DataGroup &__input, DataGroup &__output) const
    {
        DataGroup __temp(input_number()), __out;
        DataGroup *__hide_layer_sigmoid = new DataGroup[layer_number()];
        for(SizeType i = 0; _M_generation == 0 || i <_M_generation; ++i)
        {
            _F_calculate_activated_value(__input, __hide_layer_sigmoid, __out);
            bool __meet_end = _F_calculate_error(__output, __out);
            // calculate the residual.
            _F_calculate_residual(__output, __hide_layer_sigmoid, __out);
            // update weight

            if(__meet_end) break;
        }
        for(SizeType i = 0; i < layer_number(); ++i)
        {
            _F_realloc(__hide_layer_sigmoid[i]);
        }
        delete[] __hide_layer_sigmoid;
    }
};

}

#endif // NEURALNETWORK_H
