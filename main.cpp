#include "Core/Vector.h"
#include "Algorithm/Sorter.h"

using namespace rapid;

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
    Vector<int> v({100, -1, 20, -90, -9, 0, 10, 40, 20, 1, -1});
    ssort(v.begin(), v.end());
    for(int k : v)
    {
        std::cout << k << " ";
    }
    std::cout << std::endl << "finish" << std::endl;

    return 0;
}
