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
inline unsigned long address_to_integer(const void *a);

/* copy memory with positive order, not check overlapping areas
 * param[dst]: the begin pos of the destination memory
 * param[src]: the begin pos of the source memory
 * param[size]: the size required to copy
 */
void mem_copy(void *dst, void *src, const size_type size);

/* copy memory with reverse order, not check overlapping areas
 * param[dst]: the begin pos of the destination memory
 * param[src]: the begin pos of the source memory
 * param[size]: the size required to copy
 */
void mem_rcopy(void *dst, void *src, const size_type size);

/* copy memory with checking overlapping areas
 * param[dst]: the begin pos of the destination memory
 * param[src]: the begin pos of the source memory
 * param[size]: the size required to copy
 */
void mem_scopy(void *dst, void *src, const size_type size);

/* copy [size] memory from low byte to high byte
 * param[begin]: the begin pos to be operated memory
 * param[size]: the size of the memory being moved
 * param[move_distance]: the byte's distance required to move
 */
void mem_backward(void *begin, const size_type size, const size_type move_distance);

/* copy [size] memory from high byte to low byte
 * param[begin]: the begin pos to be operated memory
 * param[size]: the size of the memory
 * param[move_distance]: the byte's distance required to move
 */
void mem_forward(void *begin, const size_type size, const size_type move_distance);

/* compare two memory spaces
 * param[arg1]: memory pointer to be compared
 * param[arg2]: memory pointer to be compared
 * param[size]: size of comparison
 * return: if arg1 > arg2, return 1; if arg1 == arg2, return 0; if arg1 < arg2, return -1
 */
int mem_compare(void *arg1, void *arg2, const size_type size);

/* swap the values of two memory spaces
 * param[arg1]: memory pointer of one sapce
 * param[arg2]: memory pointer of another space
 * param[size]: byte size of swaping
 */
void mem_swap(void *arg1, void *arg2, const size_type size);

// set memory to 0
void mem_clear(void *dst, const size_type size);

template<typename T>
struct NodeBase
{
public:
    alignas(__alignof__(T)) unsigned char __Data[sizeof(T)];

    NodeBase() { }

    NodeBase(const T &arg)
    { construct(arg); }

    void construct(const T &arg)
    { ::new(address()) T(arg); }

    T* address()
    { return reinterpret_cast<T *>(&__Data[0]); }

    T content()
    { return *address(); }

    T& ref_content()
    { return *address(); }

    const T const_content()
    { return *address(); }

    const T& const_ref_content()
    { return *address(); }

    void clear()
    { mem_clear(address(), sizeof(NodeBase<T>)); }
};

};

#endif // MEMORY_H
