#ifndef IMAGEBASE_H
#define IMAGEBASE_H
#include "Core/Version.h"
#include "Core/Memory.h"

namespace rapid
{

struct RGB
{
private:
    union {
        unsigned int Total;
        struct {
            unsigned char Red;
            unsigned char Green;
            unsigned char Blue;
            unsigned char Alpha;
        } Part;
    } Data;
public:
    unsigned char& Red() { return Data.Part.Red; }
    unsigned char& Green() { return Data.Part.Green; }
    unsigned char& Blue() { return Data.Part.Blue; }
    unsigned char& Alpha() { return Data.Part.Alpha; }

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
    { Data.Total = std::forward<RGB>(rgb).Data.Total; }

    RGB operator*(double arg)
    { return RGB(*this) *= arg; }
    RGB operator+(double arg)
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
    bool operator==(const RGB &rgb)
    { return Data.Total == rgb.Data.Total; }
    bool operator==(RGB &&rgb)
    { return Data.Total == std::forward<RGB>(rgb).Data.Total; }

};

struct BMP
{
private:
    struct HeaderBlock
    {
        char FileFlag[2];
        int FileSize;
        int Reverse;

    };
    struct DescribeInfoBlock
    {

    };
    HeaderBlock *__HeaderBlock = new HeaderBlock;
    DescribeInfoBlock *__DescribeInfoBlock = new DescribeInfoBlock;

public:
};

}

#endif // IMAGEBASE_H
