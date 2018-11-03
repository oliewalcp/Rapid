#ifndef IMAGE_H
#define IMAGE_H
#include "../Math/TypeBase.h"
#include "FileBase.h"
#include "../Exception.h"

struct RGB
{
private:
    typedef unsigned char __uint8;
public:
    __uint8 r;
    __uint8 g;
    __uint8 b;
    __uint8 a;
};

struct BMP
{
private:
    typedef unsigned long long __uint64;
    typedef unsigned int __uint32;
    typedef unsigned short __uint16;
    typedef unsigned char __uint8;

    static constexpr __uint64 FILE_LOGO = 0;//文件头
    static constexpr __uint64 FILE_SIZE = 0x02;//文件大小
    static constexpr __uint64 RESERVE = 0x06;//保留
    static constexpr __uint64 DATA_BEGIN_POS = 0x0A;//图像数据区起始位置

    static constexpr __uint64 INFO_SIZE = 0x0E;//图像描述信息块大小
    static constexpr __uint64 IMAGE_WIDTH = 0x12;//图像宽度
    static constexpr __uint64 IMAGE_HEIGHT = 0x16;//图像高度
    static constexpr __uint64 PLANE_NUMBER = 0x1A;//图像的面数
    static constexpr __uint64 COLOR_BIT = 0x1C;//颜色位数  2色、16色、256色等等
    static constexpr __uint64 COMPRESS_WAY = 0x1E;//数据压缩方式
    static constexpr __uint64 DATA_SIZE = 0x22;//数据区大小
    static constexpr __uint64 HORIZONTAL_PIXEL = 0x26;//水平每米像素数
    static constexpr __uint64 VERTICAL_PIXEL = 0x2A;//垂直每米像素数
    static constexpr __uint64 USER_COLOR_NUM = 0x2E;//所用的颜色数

    struct FILE_HEADER
    {
        __uint16 file_logo = 0x424D;
        __uint32 file_size = 0;
        __uint32 reserve = 0;
        __uint32 begin_pos = 1078;
    };

    struct FILE_INFO
    {
        __uint32 info_size = 40;
        __uint32 image_width = 0;
        __uint32 image_height = 0;
        __uint16 plane_number = 1;
        __uint16 color_bit = 256;
        __uint32 compress_way = 0;
        __uint32 data_size = 0;
        __uint32 horizontal_pixel = 2835;
        __uint32 vertical_pixel = 2835;
        __uint32 color_use = 0;
    };

public:
    static _M_Base<__uint8>* parse(const char* filename)
    {
        FILE_HEADER *header = new FILE_HEADER;
        FILE_INFO *info = new FILE_INFO;
        const char *file_content = FileBaes::open(filename).data();
        __uint8 zero = 0;

        memcpy(&info->color_bit, file_content + COLOR_BIT, sizeof(info->color_bit));
        if(info->color_bit > 8)
        {
            delete header;
            delete info;
            throw Exception("This is not a gray image");
        }
        memcpy(&header->begin_pos, file_content + DATA_BEGIN_POS, sizeof(header->begin_pos));
        memcpy(&info->image_width, file_content + IMAGE_WIDTH, sizeof(info->image_width));
        memcpy(&info->image_height, file_content + IMAGE_HEIGHT, sizeof(info->image_height));

        _M_Base<__uint8> *image = new _M_Base<__uint8>(info->image_height, info->image_width, zero);
        __uint64 index = header->begin_pos;
        __uint8 offset = info->image_width % 4;
        if(offset != 0) offset = 4 - offset;
        for(__uint32 i = 0; i < image->line(); i++)
        {
            image->set_line_value((const __uint8 *)(file_content + index), i);
            index += image->column() + offset;
        }
        delete header;
        delete info;
        return image;
    }
    static void save(const char* filename, _M_Base<__uint8> *content)
    {

    }
};

#endif // IMAGE_H
