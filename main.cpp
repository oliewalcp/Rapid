#include "Algorithm/NeuralNetwork.h"
using namespace rapid;
#include <iostream>

int main()
{
    BPNeuralNetwork n;
    n.set_weight(0, {
                     {0.2, -0.7},
                     {0.8, -0.5}
                 });
    n.set_output_weight({{0.3}, {0.5}});
    n.train({0.3, -0.7}, {0.1});
    n.train({0.4, -0.5}, {0.05});
    return 0;
}
