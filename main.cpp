#include "Image/ImageBase.h"
using namespace rapid;

int main()
{
    char *ch = new char[8]{0, 0, 0, 1, 0, 0, 0, 1};
    int *a = reinterpret_cast<int *>(&ch[0]), *b = reinterpret_cast<int *>(&ch[4]);
    std::cout << *b << std::endl;
    delete a;
    std::cout << *b << std::endl;
    return 0;
}
