#ifndef TYPETRAITS_H
#define TYPETRAITS_H

#include <bits/move.h>

namespace rapid
{

template<typename T, T v>
struct ReferenceBase
{ static constexpr T value = v; };

template<typename T, typename U>
struct IsSame : public ReferenceBase<bool, false> {};

template<typename T>
struct IsSame<T, T> : public ReferenceBase<bool, true> {};

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
struct IsPointer : public ReferenceBase<bool, false> {};

template<typename T>
struct IsPointer<T*> : public ReferenceBase<bool, true> {};

template<typename T>
T remove_const(const T arg)
{ return const_cast<T>(arg); }

template<typename T>
T& remove_const(const T& arg)
{ return const_cast<T&>(arg); }

template<typename T>
T* remove_const(const T* arg)
{ return const_cast<T*>(arg); }

template<typename T>
const T add_const(T arg)
{ return *const_cast<const T*>(&arg); }

template<typename T>
const T* add_const(T* arg)
{ return const_cast<const T*>(arg); }

};

#endif // TYPETRAITS_H
