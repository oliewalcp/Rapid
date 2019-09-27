#include "Algorithm/NeuralNetwork.h"
using namespace rapid;
#include <iostream>

template<typename T>
using ini = std::initializer_list<T>;

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
    std::vector<int> test(100);
    std::vector<int> emp;
    std::cout << test.capacity() << std::endl;
    test.swap(emp);

    std::cout << test.capacity() << std::endl;
    return 0;
}
