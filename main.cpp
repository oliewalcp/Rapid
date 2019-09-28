#include "Algorithm/NeuralNetwork.h"
using namespace rapid;
#include <iostream>

int main()
{
    std::initializer_list<
        std::initializer_list<double>
    > in =
    {
        {0.3, -0.7},
        {0.4, -0.5},
    };
    std::initializer_list<
        std::initializer_list<double>
    > out =
    {
        {0.1},
        {0.05},
    };
    BPNeuralNetwork n;
    n.set_input_weight({
                           {0.2, -0.7},
                           {0.8, -0.5}
                       });
    n.train({0.3, -0.7}, {0.1});
    n.train({0.4, -0.5}, {0.05});
    return 0;
}
