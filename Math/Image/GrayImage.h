#ifndef GRAYIMAGE_H
#define GRAYIMAGE_H
#include <typeinfo>
#include "Math/TypeBase.h"
#include "File/Image.h"

typedef unsigned char __uint8;

//only two colors, black and white
class GrayImage : public _M_Base<__uint8>
{
public:
    GrayImage(const __uint32 line, const __uint32 column,  const __uint8 default_value = 0);

    /* parse the image picture as gray image
     * param[func]: _template_ class or struct with implementing parse function and returning pointer of GrayImage's instance
     * param[filename]: the name of image
     * param[format]: the file format of image
     * return: pointer of GrayImage's instance. if can not parse, return nullptr
     */
    template<typename func = BMP>
    static GrayImage* parse(const char *filename, Format format = Format::BMP)
    {
        try
        {
            switch (format)
            {
            case Format::BMP: return reinterpret_cast<GrayImage*>(BMP::parse(filename));
            case Format::JPEG: return reinterpret_cast<GrayImage*>(JPEG::parse(filename));
            case Format::JPG: return reinterpret_cast<GrayImage*>(JPG::parse(filename));
            case Format::PNG: return reinterpret_cast<GrayImage*>(PNG::parse(filename));
            case Format::CUSTOM: return reinterpret_cast<GrayImage*>(func::parse(filename));
            default: return nullptr;
            }
        }
        catch(Exception e)
        {
            std::cout << e.what() << std::endl;
            return nullptr;
        }
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
