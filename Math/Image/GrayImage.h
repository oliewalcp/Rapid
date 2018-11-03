#ifndef GRAYIMAGE_H
#define GRAYIMAGE_H
#include <typeinfo>
#include "../TypeBase.h"
#include "../../File/Image.h"

typedef unsigned char __uint8;

// support .bmp file currently
enum class Format : __uint8
{
    BMP,
    JPG,
    JPEG,
    PNG
};

//only two colors, black and white
class GrayImage : public _M_Base<__uint8>
{
public:
    GrayImage(const __uint32 line, const __uint32 column, const __uint8 default_value = 0);

    template<typename func = BMP>
    static GrayImage* parse(const char *filename, Format format = Format::BMP)
    {
        return reinterpret_cast<GrayImage*>(func::parse(filename));
    }
};

GrayImage::GrayImage(const __uint32 line, const __uint32 column, const __uint8 default_value)
{
    __column = column;
    for(__uint32 i = 0; i < line; i++)
        for(__uint32 j = 0; j < column; j++)
            set_value(i, j, default_value);
}


#endif // GRAYIMAGE_H
