#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <cmath>
#include <functional>
#include <vector>

namespace rapid
{

inline double sigmoid(double x)
{ return 1 / (1 + pow(M_E, -x)); }

template<typename _ResultType,
         typename ... _Args>
std::function<_ResultType(_Args...)> __Activation;

template<typename _DataType = double,
         typename _WeightType = double>
struct BPNeuralNetwork
{
    using DataType = _DataType;
    using SizeType = unsigned long long;
    using WeightType = _WeightType;
    using WeightGroupType = std::vector<WeightType>;
    using WeightGroup = std::vector<WeightGroupType*>;
    using NodeType = std::vector<DataType>;
    using HideNodeType = std::vector<NodeType*>;
    using HideLayerDataType = std::vector<WeightGroup*>;

    NodeType _M_input;
    NodeType _M_output;
    HideNodeType _M_hide = HideNodeType(1);
    HideLayerDataType _M_weight;
    WeightGroupType _M_output_weight;

    BPNeuralNetwork()
    {
        std::function<double(double)> f = std::bind(&sigmoid, std::placeholders::_1);
        set_activation(f);
        set_hide_layer_number(1);
    }
    ~BPNeuralNetwork()
    { clear(); }

    void clear()
    {
        _F_clear(_M_hide, 1);
        _F_clear(_M_weight, 1);
        _F_clear(_M_input);
        _F_clear(_M_output);
        _F_clear(_M_output_weight);
    }

    /*
     * the number of [__in] is same as [__out]'s
     * the number of each group of [__in] or [__out] are same
     */
    void train(std::initializer_list<std::initializer_list<DataType>> __in,
               std::initializer_list<std::initializer_list<DataType>> __out)
    {
        auto __iit = __in.begin();
        auto __oit = __out.begin();
        for(; __iit != __in.end() && __oit != __out.end(); ++__iit, ++__oit)
        {
            train((*__iit).begin(), (*__iit).end(), (*__iit).size(),
                  (*__oit).begin(), (*__oit).end(), (*__oit).size());
        }
    }

    /*
     * train one group for one time. so, you should call for multiple times.
     */
    template<typename _InputIterator,
             typename _OutputIterator>
    void train(_InputIterator __ibegin, _InputIterator __iend, SizeType __is,
               _OutputIterator __obegin, _OutputIterator __oend, SizeType __os)
    {
        if(_M_input.capacity() != 0 && __is != _M_input.capacity()) return;
        if(_M_output.capacity() != 0 && __os != _M_output.capacity()) return;
        if(_M_input.capacity() == 0)
        {
            for(SizeType i = 0; i < _M_hide.capacity(); ++i)
            {
                _M_hide.push_back(new NodeType(__is));
            }
            for(SizeType i = 1; i < _M_weight.capacity(); ++i)
            {
                WeightGroup *group = new WeightGroup(__is);
                for(SizeType j = 0; j < __is; ++j)
                {
                    group->push_back(new WeightGroupType(__is));
                }
                _M_weight.push_back(group);
            }
            _M_input.resize(__is);
        }
        if(_M_output.capacity() == 0)
        {
            _M_output.resize(__os);
        }
        for(auto it = __obegin; it != __oend; ++it)
        {
            _M_output.push_back(*it);
        }
        for(auto it = __ibegin; it != __iend; ++it)
        {
            _M_input.push_back(*it);
        }
        _F_train();
    }

    template<typename _ResultType,
             typename ... _Args>
    void set_activation(std::function<_ResultType(_Args...)> &&__f)
    { __Activation<_ResultType, _Args...> = __f; }

    template<typename _ResultType,
             typename ... _Args>
    void set_activation(const std::function<_ResultType(_Args...)> &__f)
    { __Activation<_ResultType, _Args...> = __f; }

    void set_hide_layer_number(SizeType __s)
    { _F_clear(_M_weight, __s); }

    void set_input_weight(std::initializer_list<std::initializer_list<WeightType>> __w)
    { set_input_weight(__w.begin(), __w.end(), __w.size()); }

    template<typename _InputIterator>
    void set_input_weight(_InputIterator __begin,
                          _InputIterator __end,
                          SizeType __s)
    {
        if(_M_weight.capacity() != __s)
        {
            _F_clear(_M_weight, __s);
        }
        if(_M_weight.size() == 0)
        {
            WeightGroup *group = new WeightGroup(__s);
            for(SizeType j = 0; j < __s; ++j)
            {
                group->push_back(new WeightGroupType(__s));
            }
            _M_weight.push_back(group);
        }
        _F_set_input_weight(*(_M_weight[0]), __begin, __end, __s);
    }

    template<typename _InputIterator>
    void _F_set_input_weight(WeightGroup &__c,
                             _InputIterator __begin,
                             _InputIterator __end,
                             SizeType __s)
    {
        WeightGroup empty;
        __c.swap(empty);
        __c.resize(__s);
        for(auto it1 = __begin; it1 != __end; ++it1)
        {
            WeightGroupType *group = new WeightGroupType(__s);
            for(auto it2 = (*it1).begin(); it2 != (*it1).end(); ++it2)
            {
                group->push_back(*it2);
            }
            __c.push_back(group);
        }
    }

    template<typename T>
    void _F_clear(const T &arg, SizeType s = 0)
    {
        if
    #if __cplusplus >= 201703L
                constexpr
    #endif
                (!std::is_pointer<typename T::value_type>::value)
        {
            for(auto __i : arg)
            {
                if
            #if __cplusplus >= 201703L
                        constexpr
            #endif
                        (std::is_pointer<typename T::value_type>::value)
                {
                    _F_clear(*__i);
                }
                delete __i;
            }
        }
        T empty;
        arg.swap(empty);
        arg.resize(s);
    }

    void _F_train()
    {
        for(SizeType i = 0; i < _M_hide.capacity(); ++i)
        {

        }
    }
};

}

#endif // NEURALNETWORK_H
