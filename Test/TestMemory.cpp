#include "TestCase.h"
#include "Core/Memory.h"
#include "Core/Range.h"

template<typename T>
static void print(const T *arg, const int size)
{
    for(int i : rapid::Range<T>(0, size))
    {
        std::cout << arg[i] << std::endl;
    }
}

void rapid::test_Memory_main()
{
    using TestType = int;
    constexpr TestType alen = 6, blen = 8, clen = 10;
    TestType *a = new TestType[alen]{1, 2, 3, 4, 5, 6};
    TestType *b = new TestType[blen];
    TestType *c = new TestType[clen]{0};
    std::cout << "---------------start-------------" << std::endl;
    print(b, blen);
    std::cout << "---------------mem_clear-------------" << std::endl;
    mem_clear(b, blen * sizeof(TestType));
    print(b, blen);
    std::cout << "-------------mem_copy---------------" << std::endl;
    mem_copy(b, a, alen * sizeof(TestType));
    print(b, blen);
    std::cout << "--------------mem_backward--------------" << std::endl;
    mem_backward(b, (blen - 2) * sizeof(TestType), 2 * sizeof(TestType));
    print(b, blen);
    std::cout << "------------mem_forward----------------" << std::endl;
    mem_forward(reinterpret_cast<char *>(b) + 4, (blen - 1) * sizeof(TestType), 1 * sizeof(TestType));
    print(b, blen);
    std::cout << "------------mem_scopy----------------" << std::endl;
    mem_scopy(c, a, alen * sizeof(TestType));
    print(c, clen);
    std::cout << "***************************" << std::endl;
    mem_scopy(c, reinterpret_cast<char *>(c) + 12, 20);
    print(c, clen);
    delete[] a;
    delete[] b;
    delete[] c;
}
