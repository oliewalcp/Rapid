#ifndef GRAYIMAGE_H
#define GRAYIMAGE_H
#include <typeinfo>
#include "Math/MathBase.h"
#include "File/Image.h"

typedef unsigned char __uint8;
typedef _M_Base<__uint8>*(*func)(const char *);

//only two colors, black and white
class GrayImage : public _M_Base<__uint8>
{
public:
    GrayImage(const __uint32 & line, const __uint32 & column, __uint8 default_value = 0) : _M_Base<__uint8>(line, column, default_value) { }

    GrayImage(__uint32 && line, __uint32 && column, __uint8 default_value = 0) : _M_Base<__uint8>(line, column, default_value) { }
    /* parse the image picture as gray image
     * param[func]: _template_ class or struct with implementing parse function and returning pointer of GrayImage's instance
     * param[filename]: the name of image
     * param[format]: the file format of image
     * return: pointer of GrayImage's instance. if can not parse, return nullptr
     */
    static GrayImage* parse(const char *filename, Format format = Format::BMP, func *f = nullptr)
    {
        try
        {
            switch (format)
            {
            case Format::BMP: return reinterpret_cast<GrayImage*>(BMP::parse_gray(filename));
            case Format::JPEG: return reinterpret_cast<GrayImage*>(JPEG::parse_gray(filename));
            case Format::JPG: return reinterpret_cast<GrayImage*>(JPG::parse_gray(filename));
            case Format::PNG: return reinterpret_cast<GrayImage*>(PNG::parse_gray(filename));
            case Format::CUSTOM: return reinterpret_cast<GrayImage*>((*f)(filename));
            default: return nullptr;
            }
        }
        catch(std::exception e)
        {
            std::cout << e.what() << std::endl;
            return nullptr;
        }
    }
    /* get [plane_num] plane of the matrix
     * param[plane_num]: the value of plane num. begin at 0, 0 at low
     */
    GrayImage* get_plane(__uint8 plane_num);
};

GrayImage *GrayImage::get_plane(__uint8 plane_num)
{
    __uint32 line_num = __data->size();
    __uint8 temp = 1 << plane_num;
    GrayImage *result = new GrayImage(line_num, __column);
    for(__uint32 i = 0; i < line_num; i++)
    {
        for(__uint32 j = 0; j < __column; j++)
        {
            __uint8 t = ((__data->at(i))[j] & temp) >> plane_num;
            result->set_value(i, j, t > 0 ? 255 : 0);
        }
    }
    return result;
}
#endif // GRAYIMAGE_H
