#include "Algorithm/Sorter.h"
using namespace rapid;

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
    int m[10]{10, 2, -9, 0, 200, 20, 2, 22, 1, 11};
    qsort(m, m + 10);
    for(int i : m)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    return 0;
}
