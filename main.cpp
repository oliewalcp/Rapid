#include "Core/Vector.h"
using namespace rapid;
#include <type_traits>
#include <iostream>

union A { };
struct B { };
enum C { };
class D { };

template<typename T>
void foo()
{
    std::cout << __is_class(T) << std::endl;
}

int main()
{
    std::cout << "C is class ?: ";
    foo<C>();
    std::cout << "B is class ?: ";
    foo<B>();
    std::cout << "D is class ?: " << __is_class(D) << std::endl;
    std::cout << "A is union ?: " << __is_union(A) << std::endl;
    C c;
    std::cout << "c is enum ?: " << __is_enum(typeof(c)) << std::endl;
    return 0;
}
