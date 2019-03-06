#ifndef COUNTPOINTER_H
#define COUNTPOINTER_H

#include "Atomic/Atomic.h"
#include <ext/new_allocator.h>

#if defined(__GNUC__) || defined(__GNUG__)
#if __GNUC__ < 7 && __GNUC_MINOR__ < 1 && __GNUC_PATCHLEVEL__ < 0
#error please support C++17
#endif
#endif

template<typename Type, size_t Count = 1>
class CountPointer
{
private:
    typedef __gnu_cxx::new_allocator<Type> __Alloc;
    typedef unsigned long __uint32;
    typedef Atomic<__uint32> __Counter;
    typedef Type* __Pointer;
    static constexpr size_t __TypeSize = sizeof(Type);

    __Counter *__ReferenceCount;
    __Pointer __Data;

public:
    explicit CountPointer() : __ReferenceCount(nullptr), __Data(nullptr) {}
    explicit CountPointer(Type p) : __ReferenceCount(new __Counter(1))
    {
        __Alloc a = __Alloc();
        __Data = a.allocate(__TypeSize);
        if constexpr (std::is_pointer<Type>::value)
        {
            *__Data = p;
        }
        else
        {
            a.construct(__Data, p);
        }
    }
    explicit CountPointer(const CountPointer<Type, Count> &arg) : __Data(arg.__Data)
    {
        arg.__ReferenceCount->add_and_fetch(1);
        __ReferenceCount = arg.__ReferenceCount;
    }

    __uint32 use_count()
    { return __ReferenceCount != nullptr ? *(reinterpret_cast<__uint32*>(__ReferenceCount)) : 0; }

    __Pointer operator->()
    { return __Data; }

    Type operator*()
    { return *__Data; }

    __Pointer get()
    { return __Data; }

    __Pointer operator=(const CountPointer<Type, Count> &arg)
    {
        arg.__ReferenceCount->add_and_fetch(1);
        __ReferenceCount = arg.__ReferenceCount;
        __Data = arg.__Data;
    }

    void release()
    {
        if((__ReferenceCount->sub_and_fetch(1)) == 0)
        {
            delete __ReferenceCount;
            __ReferenceCount = nullptr;
            __Alloc a;
            if constexpr (std::is_pointer<Type>::value)
            {
                if(Count == 1) delete *__Data;
                else delete[] *__Data;
            }
            a.deallocate(__Data, __TypeSize);
            __Data = nullptr;
        }
    }
    ~CountPointer()
    { release(); }
};

#endif // COUNTPOINTER_H
