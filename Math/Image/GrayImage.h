#ifndef GRAYIMAGE_H
#define GRAYIMAGE_H
#include <typeinfo>
#include "../TypeBase.h"
#include "ImageParsing.h"

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

    template<typename func = Image>
    static GrayImage* parse(const char *filename, Format format = Format::BMP)
    {
        if(typeid(func) != typeid(Image))
            return reinterpret_cast<GrayImage*>(func::parse(filename));
        else
        {
            switch (format) {
            case Format::BMP: return reinterpret_cast<GrayImage*>(func::parse_gray_image_by_bmp(filename));
            case Format::JPEG: return reinterpret_cast<GrayImage*>(func::parse_gray_image_by_jpeg(filename));
            case Format::JPG: return reinterpret_cast<GrayImage*>(func::parse_gray_image_by_jpg(filename));
            case Format::PNG: return reinterpret_cast<GrayImage*>(func::parse_gray_image_by_png(filename));
            }
        }
        return nullptr;
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
