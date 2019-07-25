#include "Image/ImageBase.h"
using namespace rapid;

int main()
{
    RGB a;
    a.Red() = 255;

    std::cout << static_cast<int>(a.Red()) << std::endl;
    return 0;
}
