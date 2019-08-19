#ifndef SHAREPOINTER_H
#define SHAREPOINTER_H

namespace rapid
{
template<typename T>
class SharedPointer
{
private:
    using Type = T;
    using ConstReference = const Type &;
    using RvalueReference = Type &&;
    using Pointer = Type *;


    SharedPointer();
public:
    SharedPointer(SharedPointer &sp);
    SharedPointer(SharedPointer &&sp);

    template<typename ... Args>
    static SharedPointer make_shared(Args ...);
};

}

#endif // SHAREPOINTER_H
