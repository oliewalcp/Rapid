#ifndef SHAREPOINTER_H
#define SHAREPOINTER_H

namespace rapid
{
template<typename T>
class SharedPointer
{
private:
    using Type = T;
    using ConstReference = const Type&;
    using RvalueReference = Type &&;

public:
    SharedPointer();
    SharedPointer(SharedPointer<T> &sp);
    SharedPointer(SharedPointer<T> &&sp);

    static SharedPointer make_shared();
};

}

#endif // SHAREPOINTER_H
