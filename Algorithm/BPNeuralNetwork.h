#ifndef BPNEURALNETWORK_H
#define BPNEURALNETWORK_H

#include <cmath>
#include <functional>

namespace rapid
{

inline double sigmoid(double x)
{ return 1 / (1 + pow(M_E, -x)); }

template<typename _ResultType,
         typename ... _Args>
std::function<_ResultType(_Args...)> __Activation;

template<typename _InputType = double,
         typename _OutputType = _InputType>
struct BPNeuralNetwork
{
    using InputType = _InputType;
    using OutputType = _OutputType;
    using SizeType = unsigned long long;

//    SizeType _M_layer_number;
    SizeType _M_node_number;


    void train(std::initializer_list<InputType> __in,
               std::initializer_list<OutputType> __out)
    { train(__in.begin(), __in.end(), __out.begin(), __out.end()); }

    template<typename _InputIterator,
             typename _OutputIterator>
    void train(_InputIterator __ibegin, _InputIterator __iend,
               _OutputIterator __obegin, _OutputIterator __oend)
    {

    }
};

}

#endif // BPNEURALNETWORK_H
