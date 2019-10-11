#ifndef RGB_H
#define RGB_H

#include "Core/TypeTraits.h"

namespace rapid
{

struct RGB
{
private:
    union {
        unsigned int Total;
        struct {
            unsigned char Blue;
            unsigned char Green;
            unsigned char Red;
            unsigned char Alpha;
        } Part;
    } Data;
public:
    RGB(unsigned int arg)
    { Data.Total = arg; }
    RGB(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0, unsigned char a = 0)
    {
        Red() = r;
        Green() = g;
        Blue() = b;
        Alpha() = a;
    }
    RGB(const RGB &rgb)
    { Data.Total = rgb.Data.Total; }
    RGB(RGB &&rgb)
    { Data.Total = forward<RGB>(rgb).Data.Total; }

    unsigned char& Red()
    { return Data.Part.Red; }
    unsigned char& Green()
    { return Data.Part.Green; }
    unsigned char& Blue()
    { return Data.Part.Blue; }
    unsigned char& Alpha()
    { return Data.Part.Alpha; }
    unsigned char Red() const
    { return Data.Part.Red; }
    unsigned char Green() const
    { return Data.Part.Green; }
    unsigned char Blue() const
    { return Data.Part.Blue; }
    unsigned char Alpha() const
    { return Data.Part.Alpha; }

    RGB operator*(double arg) const
    { return RGB(*this) *= arg; }
    RGB operator+(double arg) const
    { return RGB(*this) += arg; }
    RGB operator=(const RGB &rgb)
    {
        Data.Total = rgb.Data.Total;
        return *this;
    }
    RGB operator=(unsigned int arg)
    {
        Data.Total = arg;
        return *this;
    }
    RGB operator*=(double arg);
    RGB operator+=(double arg);
    bool operator==(const RGB &rgb) const
    { return Data.Total == rgb.Data.Total; }

};


}

#endif // RGB_H
