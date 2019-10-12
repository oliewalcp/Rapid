#ifndef RGB_H
#define RGB_H

#include "Core/TypeTraits.h"

namespace rapid
{

struct RGB final
{
    unsigned char Blue;
    unsigned char Green;
    unsigned char Red;
    unsigned char Alpha;

    RGB(unsigned int arg)
    { this->operator=(arg); }
    RGB(unsigned char r = 0, unsigned char g = 0,
        unsigned char b = 0, unsigned char a = 0)
        : Blue(b), Green(g), Red(r), Alpha(a)
    { }
    RGB(const RGB &rgb)
    { this->operator=(rgb); }
    RGB(RGB &&rgb)
    { this->operator=(forward<RGB>(rgb)); }

    RGB operator*(double arg) const
    { return RGB(*this) *= arg; }
    RGB operator+(double arg) const
    { return RGB(*this) += arg; }
    RGB operator=(const RGB &rgb)
    {
        *reinterpret_cast<unsigned int*>(this) = *reinterpret_cast<const unsigned int*>(&rgb);
        return *this;
    }
    RGB operator=(unsigned int arg)
    {
        *reinterpret_cast<unsigned int*>(this) = arg;
        return *this;
    }
    RGB operator*=(double arg);
    RGB operator+=(double arg);
    bool operator==(const RGB &rgb) const
    { return *reinterpret_cast<const unsigned int*>(this) == *reinterpret_cast<const unsigned int*>(&rgb); }

};


}

#endif // RGB_H
