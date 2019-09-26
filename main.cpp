#include <functional>
#include <iostream>

int func(int a, int b)
{
    return a + b;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
    std::function<int(int,int)> f = std::bind(&func, std::placeholders::_1, std::placeholders::_2);
    int sum = f(10, 20);
    std::cout << sum << std::endl;
    return 0;
}
