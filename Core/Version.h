#ifndef VERSION_H
#define VERSION_H

#if __cplusplus < 201402L
#error "please support C++14, need __cplucplus >= 201402L"
#endif

#if __cplusplus >= 201703L
#define CONSTEXPR constexpr
#else
#define CONSTEXPR
#endif

#ifndef NDEBUG
#include <iostream>
#endif

#endif // VERSION_H
