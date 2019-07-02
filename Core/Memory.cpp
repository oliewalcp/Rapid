#include "Memory.h"
//#include "Core/TypeTraits.h"

using uint64 = unsigned long long;
using uint32 = unsigned int;
using uint16 = unsigned short;
using uint8 = unsigned char;
using ulong = unsigned long;

#define null_return1(a) if(a == nullptr) return;
#define null_return2(a, b) if(a == nullptr || b == nullptr) return;

template<typename T>
static inline void copy_memory(void *dst, void *src)
{ *(reinterpret_cast<T *>(dst)) = *(reinterpret_cast<T *>(src)); }

/* copy memory with positive order, not check overlapping areas
 * param[dst]: the begin pos of the destination memory
 * param[src]: the begin pos of the source memory
 * param[count]: copy number
 */
template<typename T>
static void __mem_copy(void *dst, void *src, unsigned long count)
{
    char *d = reinterpret_cast<char *>(dst), *s = reinterpret_cast<char *>(src);
    while(count-- > 0)
    {
        copy_memory<T>(d, s);
        d += sizeof(T);
        s += sizeof(T);
    }
}

/* copy memory with reverse order, not check overlapping areas
 * param[dst]: the end pos + 1 of the destination memory
 * param[src]: the end pos + 1 of the source memory
 * param[count]: copy number
 */
template<typename T>
static void __mem_rcopy(void *dst, void *src, unsigned long count)
{
    char *d = reinterpret_cast<char *>(dst) - sizeof(T), *s = reinterpret_cast<char *>(src) - sizeof(T);
    while(count-- > 0)
    {
        copy_memory<T>(d, s);
        d -= sizeof(T);
        s -= sizeof(T);
    }
}

void rapid::mem_copy(void *dst, void *src, const unsigned long size)
{
    null_return2(dst, src);
    char *d = reinterpret_cast<char *>(dst), *s = reinterpret_cast<char *>(src);
    ulong count = size / 8;
    __mem_copy<uint64>(d, s, count);
    count = size * 8;
    __mem_copy<uint8>(d + count, s + count, size % 8);
}

void rapid::mem_rcopy(void *dst, void *src, const unsigned long size)
{
    null_return2(dst, src);
    char *d = reinterpret_cast<char *>(dst) + size, *s = reinterpret_cast<char *>(src) + size;
    ulong count = size / 8;
    __mem_rcopy<uint64>(d, s, count);
    count = count * 8;
    __mem_rcopy<uint8>(d - count, s - count, size % 8);
}

void rapid::mem_clear(void *dst, const unsigned long size)
{
    null_return1(dst);
    char *d = reinterpret_cast<char *>(dst);
    ulong count_8byte = size / 8;
    while(count_8byte-- > 0)
    {
        *(reinterpret_cast<uint64 *>(d)) = 0;
        d += 8;
    }
    uint8 count_remain = size % 8;
    while(count_remain-- > 0)
    {
        *(reinterpret_cast<uint64 *>(d)) = 0;
        d++;
    }
}

void rapid::mem_scopy(void *dst, void *src, const unsigned long size)
{
    null_return2(dst, src);
    char *d = reinterpret_cast<char *>(dst), *s = reinterpret_cast<char *>(src);
    if(address_to_int(dst) > address_to_int(src))
    {
        if(address_to_int(src) + size < address_to_int(dst))
        { mem_backward(s, size, address_to_int(dst) - address_to_int(src)); }
        else
        { mem_copy(d, s, size); }
    }
    else if(address_to_int(dst) < address_to_int(src))
    {
        if(address_to_int(dst) + size < address_to_int(src))
        { mem_forward(s, size, address_to_int(dst) - address_to_int(src)); }
        else
        { mem_copy(d, s, size); }
    }
}

void rapid::mem_backward(void *begin, const unsigned long size, const unsigned long move_distance)
{
    mem_rcopy(reinterpret_cast<char *>(begin) + move_distance, reinterpret_cast<char *>(begin), size);
}

void rapid::mem_forward(void *begin, const unsigned long size, const unsigned long move_distance)
{
    mem_copy(reinterpret_cast<char *>(begin) - move_distance, reinterpret_cast<char *>(begin), size);
}

#ifndef NDEBUG
#include "Core/Range.h"
template<typename T>
static void print(const T *arg, const int size)
{
    for(int i : rapid::Range(0, size))
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
#endif