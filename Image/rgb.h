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
        unsigned char b = 0, unsigned char a = 255)
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

    RGB to_rgb() const
    { return RGB(Blue, Green, Red); }
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
    RGB_24(const RGB &rgb) : Blue(rgb.Blue), Green(rgb.Green), Red(rgb.Red)
    { }

    RGB to_rgb() const
    { return RGB(Blue, Green, Red); }
};

template<unsigned int>
struct ColorType
{ typedef RGB type; };

template<>
struct ColorType<1>
{ typedef unsigned char type; };

template<>
struct ColorType<2>
{ typedef unsigned short type; };

}

#endif // RGB_H
