#include "IO.h"
#include "Conver.h"
#include <iostream>

rapid::ConsoleOutput rapid::out;

#define REGIST_OUT_TYPE(type) \
    rapid::ConsoleOutput &rapid::ConsoleOutput::operator<<(type arg) \
    { \
        std::cout << arg; \
        return *this; \
    }

#define REGIST_WOUT_TYPE(type) \
    rapid::ConsoleOutput &rapid::ConsoleOutput::operator<<(type arg) \
    { \
        std::wcout << arg; \
        return *this; \
    }

rapid::ConsoleOutput &rapid::ConsoleOutput::operator<<(const char *ch)
{
#ifdef __WIN32
    char *result = to_ansi(ch);
    std::cout << result;
    delete[] result;
#else
    std::cout << ch;
#endif
    return *this;
}

REGIST_OUT_TYPE(bool)
REGIST_OUT_TYPE(char)
REGIST_WOUT_TYPE(wchar_t)
REGIST_OUT_TYPE(char16_t)
REGIST_OUT_TYPE(char32_t)
REGIST_OUT_TYPE(short)
REGIST_OUT_TYPE(unsigned short)
REGIST_OUT_TYPE(int)
REGIST_OUT_TYPE(unsigned int)
REGIST_OUT_TYPE(long)
REGIST_OUT_TYPE(unsigned long)
REGIST_OUT_TYPE(long long)
REGIST_OUT_TYPE(unsigned long long)
REGIST_OUT_TYPE(float)
REGIST_OUT_TYPE(double)
REGIST_OUT_TYPE(std::string)
REGIST_WOUT_TYPE(std::wstring)
REGIST_OUT_TYPE(out_stream)
