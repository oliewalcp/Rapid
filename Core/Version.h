#ifndef VERSION_H
#define VERSION_H

#ifndef RAPID_1
#define RAPID_1
#endif 

#if __cplusplus < 201402L
#error "please support C++14, need __cplucplus >= 201402L"
#endif

#if __cplusplus >= 201703L
#define CONSTEXPR constexpr
#else
#define CONSTEXPR
#endif

#define un_use(x) (void)x

namespace rapid
{
    using size_type = unsigned long;
}

#endif // VERSION_H
