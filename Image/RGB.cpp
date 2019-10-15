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
static unsigned char limit_value(int value1, int value2)
{
    if(255 - value1 > value2)
    {
        return static_cast<unsigned char>(value1 + value2);
    }
    else
    {
        return 255;
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
    Red = limit_value(static_cast<int>(Red), static_cast<int>(arg));
    Green = limit_value(static_cast<int>(Green), static_cast<int>(arg));
    Blue = limit_value(static_cast<int>(Blue), static_cast<int>(arg));
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

rapid::RGBA rapid::RGBA::operator+(const rapid::RGBA &rgb) const
{
    return RGBA(limit_value(Red, rgb.Red), limit_value(Green, rgb.Green),
                limit_value(Blue, rgb.Blue), limit_value(Alpha, rgb.Alpha));
}

rapid::RGB_24 rapid::RGB_24::operator+(const rapid::RGB_24 &rgb) const
{
    return RGB_24(limit_value(Red, rgb.Red),
                  limit_value(Green, rgb.Green),
                  limit_value(Blue, rgb.Blue));
}

rapid::RGB_24 rapid::RGB_24::operator*=(double arg)
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

rapid::RGB_24 rapid::RGB_24::operator+=(double arg)
{
    Red = limit_value(static_cast<int>(Red), static_cast<int>(arg));
    Green = limit_value(static_cast<int>(Green), static_cast<int>(arg));
    Blue = limit_value(static_cast<int>(Blue), static_cast<int>(arg));
    return *this;
}
