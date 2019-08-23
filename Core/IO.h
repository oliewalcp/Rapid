#ifndef IO_H
#define IO_H

#include <iostream>

#define OUT_PUT_TYPE(type) \
    ConsoleOutput& operator<<(type arg)

namespace rapid
{

class ConsoleOutput
{
private:
    typedef std::ostream&(*endl)(std::ostream&);
public:
    OUT_PUT_TYPE(const char *);
    OUT_PUT_TYPE(bool);
    OUT_PUT_TYPE(char);
    OUT_PUT_TYPE(wchar_t);
    OUT_PUT_TYPE(char16_t);
    OUT_PUT_TYPE(char32_t);
    OUT_PUT_TYPE(short);
    OUT_PUT_TYPE(unsigned short);
    OUT_PUT_TYPE(int);
    OUT_PUT_TYPE(unsigned int);
    OUT_PUT_TYPE(long);
    OUT_PUT_TYPE(unsigned long);
    OUT_PUT_TYPE(long long);
    OUT_PUT_TYPE(unsigned long long);
    OUT_PUT_TYPE(float);
    OUT_PUT_TYPE(double);
    OUT_PUT_TYPE(std::string);
    OUT_PUT_TYPE(std::wstring);
    OUT_PUT_TYPE(endl);
};


extern ConsoleOutput out;

}

#endif // IO_H
