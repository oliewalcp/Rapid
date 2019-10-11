#include "Algorithm/NeuralNetwork.h"
#include "Test/TestNeuralNetwork.h"
#include <iostream>

void rapid::test_NeuralNetwork_main()
{
    BPNeuralNetwork bp;
    bp.set_generation_number(1);
    bp.set_weight(0, {
                      {0.2, -0.7},
                      {0.8, -0.5}
                  });
    bp.set_output_weight({{0.3}, {0.5}});
    bp.train({0.3, -0.7}, {0.1});
    bp.train({0.4, -0.5}, {0.05});
    auto result = bp.predict({0.2, 0.1});
    std::cout << result[0] << std::endl;
}
