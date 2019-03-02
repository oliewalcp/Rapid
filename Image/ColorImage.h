#ifndef COLORIMAGE_H
#define COLORIMAGE_H
#include "Math/MathBase.h"
#include "File/Image.h"

class ColorImage : public _M_Base<RGB>
{
public:
    ColorImage(const __uint32 & line, const __uint32 & column, __uint8 default_value = 0) : _M_Base<RGB>(line, column, default_value) { }

    ColorImage(__uint32 && line, __uint32 && column, __uint8 default_value = 0) : _M_Base<RGB>(line, column, default_value) { }
    /* parse the image picture as gray image
     * param[func]: _template_ class or struct with implementing parse function and returning pointer of GrayImage's instance
     * param[filename]: the name of image
     * param[format]: the file format of image
     * return: pointer of GrayImage's instance. if can not parse, return nullptr
     */
    template<typename func = BMP>
    static ColorImage* parse(const char *filename, Format format = Format::BMP)
    {
        try
        {
            switch (format)
            {
            case Format::BMP: return reinterpret_cast<ColorImage*>(BMP::parse_color(filename));
            case Format::JPEG: return reinterpret_cast<ColorImage*>(JPEG::parse(filename));
            case Format::JPG: return reinterpret_cast<ColorImage*>(JPG::parse(filename));
            case Format::PNG: return reinterpret_cast<ColorImage*>(PNG::parse(filename));
            case Format::CUSTOM: return reinterpret_cast<ColorImage*>(func::parse(filename));
            default: return nullptr;
            }
        }
        catch(std::exception e)
        {
            std::cout << e.what() << std::endl;
            return nullptr;
        }
    }
};

#endif // COLORIMAGE_H
