#ifndef TYPETRAITS_H
#define TYPETRAITS_H

namespace rapid
{

template<typename T, T v>
struct ReferenceBase
{ static constexpr T value = v; };

struct FalseType : ReferenceBase<bool, false> {};

struct TrueType : ReferenceBase<bool, true> {};

template<typename T, typename U>
struct IsSame : public FalseType {};

template<typename T>
struct IsSame<T, T> : public TrueType {};

template<typename T>
struct RemoveReference
{ typedef T type; };

template<typename T>
struct RemoveReference<T*>
{ typedef T type; };

template<typename T>
struct RemoveReference<const T*>
{ typedef T type; };

template<typename T>
struct RemoveReference<T&>
{ typedef T type; };

template<typename T>
struct RemoveReference<T&&>
{ typedef T type; };

template<typename T>
struct RemoveReference<const T&>
{ typedef T type; };

template<typename T>
struct RemoveReference<const T&&>
{ typedef T type; };

template<typename T>
struct IsPointer : FalseType {};

template<typename T>
struct IsPointer<T*> : TrueType {};

template<typename T>
struct IsLvalueReference : FalseType {};

template<typename T>
struct IsLvalueReference<T&> : TrueType {};

template<typename T>
struct IsRvalueReference : FalseType {};

template<typename T>
struct IsRvalueReference<T&&> : TrueType {};

template<typename T>
T remove_const(const T arg)
{ return const_cast<T>(arg); }

template<typename T>
T& remove_const(const T& arg)
{ return const_cast<T&>(arg); }

template<typename T>
T&& remove_const(const T&& arg)
{ return const_cast<T&&>(arg); }

template<typename T>
T* remove_const(const T* arg)
{ return const_cast<T*>(arg); }

template<typename T>
const T* add_const(T* arg)
{ return const_cast<const T*>(arg); }

template<typename T>
const T add_const(T arg)
{ return const_cast<const T>(arg); }

template<typename T>
const T& add_const(T &arg)
{ return const_cast<const T&>(arg); }

template<typename T>
const T&& add_const(T &&arg)
{ return const_cast<const T&&>(arg); }

// copy from standard library
template<typename T>
constexpr T&& forward(typename RemoveReference<T>::type &arg)
{ return static_cast<T&&>(arg); }

template<typename T>
constexpr T&& forward(typename RemoveReference<T>::type&& arg)
{
    static_assert(IsLvalueReference<T>::value, "template argument"
          " substituting _Tp is an lvalue reference type");
    return static_cast<T&&>(arg);
}

template<typename T>
constexpr typename RemoveReference<T>::type&& move(T &&arg)
{ return static_cast<typename RemoveReference<T>::type&&>(arg); }

};

#endif // TYPETRAITS_H
