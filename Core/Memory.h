#ifndef MEMORY_H
#define MEMORY_H

#include "Version.h"


namespace rapid {

template<typename Type>
CONSTEXPR long address_of_virtual_func(Type *arg)
{ return *reinterpret_cast<long *>(*reinterpret_cast<long *>(&arg)); };

/* change address to integer
 * param[a]: pointer
 */
inline unsigned long address_to_int(void *a)
{ return reinterpret_cast<unsigned long &>(a); }

/* copy memory with positive order, not check overlapping areas
 * param[dst]: the begin pos of the destination memory
 * param[src]: the begin pos of the source memory
 * param[size]: the size required to copy
 */
void mem_copy(void *dst, void *src, const unsigned long size);

/* copy memory with reverse order, not check overlapping areas
 * param[dst]: the begin pos of the destination memory
 * param[src]: the begin pos of the source memory
 * param[size]: the size required to copy
 */
void mem_rcopy(void *dst, void *src, const unsigned long size);

/* copy memory with checking overlapping areas
 * param[dst]: the begin pos of the destination memory
 * param[src]: the begin pos of the source memory
 * param[size]: the size required to copy
 */
void mem_scopy(void *dst, void *src, const unsigned long size);

/* copy [size] memory from low byte to high byte
 * param[begin]: the begin pos to be operated memory
 * param[size]: the size of the memory being moved
 * param[move_distance]: the byte's distance required to move
 */
void mem_backward(void *begin, const unsigned long size, const unsigned long move_distance);

/* copy [size] memory from high byte to low byte
 * param[begin]: the begin pos to be operated memory
 * param[size]: the size of the memory
 * param[move_distance]: the byte's distance required to move
 */
void mem_forward(void *begin, const unsigned long size, const unsigned long move_distance);

/* compare two memory spaces
 * param[arg1]: memory pointer to be compared
 * param[arg2]: memory pointer to be compared
 * param[size]: size of comparison
 * return: if arg1 > arg2, return 1; if arg1 == arg2, return 0; if arg1 < arg2, return -1
 */
int mem_compare(void *arg1, void *arg2, const unsigned long size);

/* swap the values of two memory spaces
 * param[arg1]: memory pointer of one sapce
 * param[arg2]: memory pointer of another space
 * param[size]: byte size of swaping
 */
void mem_swap(void *arg1, void *arg2, const unsigned long size);

// set memory to 0
void mem_clear(void *dst, const unsigned long size);

template<typename T>
struct NodeBase
{
private:
    using Type = T;
public:
    alignas(__alignof__(Type)) unsigned char __Data[sizeof(Type)];

    NodeBase() { }

    NodeBase(const Type &arg)
    { construct(arg); }

    void construct(const Type &arg)
    { ::new(address()) Type(arg); }

    Type* address()
    { return reinterpret_cast<Type *>(&__Data[0]); }

    Type content()
    { return *address(); }

    Type& ref_content()
    { return *address(); }

    const Type const_content()
    { return *address(); }

    const Type& const_ref_content()
    { return *address(); }

    void clear()
    { mem_clear(address(), sizeof(NodeBase<T>)); }
};

#ifndef NDEBUG
void test_Memory_main();
#endif
};

#endif // MEMORY_H
