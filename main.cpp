#include "Core/Vector.h"
using namespace rapid;

int main()
{
    std::hash<int *> a;
    std::cout << a(new int(10)) << std::endl;
    return 0;
}
