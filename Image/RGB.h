#ifndef RGB_H
#define RGB_H

#include "Core/TypeTraits.h"

namespace rapid
{

struct RGBA final
{
    unsigned char Blue;
    unsigned char Green;
    unsigned char Red;
    unsigned char Alpha;

    RGBA(unsigned char r = 0, unsigned char g = 0,
        unsigned char b = 0, unsigned char a = 255)
        : Blue(b), Green(g), Red(r), Alpha(a)
    { }
    RGBA(const RGBA &rgb) : Blue(rgb.Blue), Green(rgb.Green), Red(rgb.Red), Alpha(rgb.Alpha)
    { }
    RGBA(RGBA &&rgb) : Blue(rgb.Blue), Green(rgb.Green), Red(rgb.Red), Alpha(rgb.Alpha)
    { }

    RGBA operator*(double arg) const
    { return RGBA(*this) *= arg; }
    RGBA operator+(double arg) const
    { return RGBA(*this) += arg; }
    RGBA operator=(const RGBA &rgb)
    {
        Blue = rgb.Blue;
        Green = rgb.Green;
        Red = rgb.Red;
        Alpha = rgb.Alpha;
        return rgb;
    }
    RGBA operator*=(double arg);
    RGBA operator+=(double arg);
    bool operator==(const RGBA &rgb) const
    { return *reinterpret_cast<const unsigned int*>(this) == *reinterpret_cast<const unsigned int*>(&rgb); }

};

struct RGB_16 final
{
    unsigned short Blue : 5;
    unsigned short Green : 6;
    unsigned short Red : 5;

    RGB_16() = default;
    RGB_16(const unsigned char low, const unsigned char high)
    {
        Blue = low & 0b00011111;
        Green = static_cast<unsigned short>((low >> 5) & ((high & 0b00000111) << 3));
        Red = static_cast<unsigned short>(high >> 3);
    }
    RGB_16(const RGBA &rgb) : Blue(rgb.Blue), Green(rgb.Green), Red(rgb.Red)
    { }

    RGBA to_rgb() const
    { return RGBA(Blue, Green, Red); }
};

struct RGB_24 final
{
    unsigned char Blue;
    unsigned char Green;
    unsigned char Red;

    RGB_24() = default;
    RGB_24(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0)
        : Blue(b), Green(g), Red(r)
    { }
    RGB_24(const RGBA &rgb) : Blue(rgb.Blue), Green(rgb.Green), Red(rgb.Red)
    { }

    RGBA to_rgb() const
    { return RGBA(Blue, Green, Red); }
};

using RGB = RGB_24;
using RGB_32 = RGBA;

RGBA operator""_rgb(const char *str, unsigned long long size);

}

#endif // RGB_H
