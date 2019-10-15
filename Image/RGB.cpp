#include "RGB.h"

static void operator_rgb(unsigned char &value, double arg)
{
    if(static_cast<int>(value) * arg > 255)
    {
        value = 255;
    }
    else
    {
        value = static_cast<unsigned char>(value * arg);
    }
}
static void limit_rgb(unsigned char &value, int target)
{
    if(target < 0)
    {
        value = 0;
    }
    else if(target > 255)
    {
        value = 255;
    }
    else
    {
        value = static_cast<unsigned char>(target);
    }
}

rapid::RGBA rapid::RGBA::operator*=(double arg)
{
    if(arg < 0)
    {
        Red = Green = Blue = 0;
    }
    else
    {
        operator_rgb(Red, arg);
        operator_rgb(Green, arg);
        operator_rgb(Blue, arg);
    }
    return *this;
}

rapid::RGBA rapid::RGBA::operator+=(double arg)
{
    int r = static_cast<int>(Red) + static_cast<int>(arg);
    int g = static_cast<int>(Green) + static_cast<int>(arg);
    int b = static_cast<int>(Blue) + static_cast<int>(arg);
    limit_rgb(Red, r);
    limit_rgb(Green, g);
    limit_rgb(Blue, b);
    return *this;
}

rapid::RGBA operator""_c(const char *str, unsigned long long size)
{
    unsigned char rgb[4]{0};
    rgb[3] = 255;
    unsigned char index = 0;
    const char *end = str + size;
    while(str != end)
    {
        if(*str != ' ')
        {
            rgb[index] = static_cast<unsigned char>(rgb[index] * 10 + *str - 48);
        }
        else ++index;
        str++;
    }
    return rapid::RGBA(rgb[0], rgb[1], rgb[2], rgb[3]);
}
