#include "TestCase.h"
#include "Core/Range.h"

void rapid::test_Range_main()
{
    for(int i : Range<int>(1, 10))
    {
        std::cout << i << std::endl;
    }
}
