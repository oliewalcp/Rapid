#ifndef IMAGE_H
#define IMAGE_H
#include "../Math/TypeBase.h"
#include "FileBase.h"
#include "../Exception.h"
#include <iostream>

struct RGB
{
private:
    typedef unsigned char __uint8;
public:
    __uint8 B = 0;
    __uint8 G = 0;
    __uint8 R = 0;
    __uint8 A = 0;
};

struct BMP
{
private:
    typedef unsigned long long __uint64;
    typedef unsigned int __uint32;
    typedef unsigned short __uint16;
    typedef unsigned char __uint8;

    static constexpr __uint64 FileLogo = 0;//文件头
    static constexpr __uint64 FileSize = 0x02;//文件大小
    static constexpr __uint64 Reserve = 0x06;//保留
    static constexpr __uint64 DATA_BeginPos = 0x0A;//图像数据区起始位置

    static constexpr __uint64 InfoSize = 0x0E;//图像描述信息块大小
    static constexpr __uint64 ImageWidth = 0x12;//图像宽度
    static constexpr __uint64 ImageHeight = 0x16;//图像高度
    static constexpr __uint64 PlaneNumber = 0x1A;//图像的面数
    static constexpr __uint64 ColorBit = 0x1C;//颜色位数  2色、16色、256色等等
    static constexpr __uint64 CompressWay = 0x1E;//数据压缩方式
    static constexpr __uint64 DataSize = 0x22;//数据区大小
    static constexpr __uint64 HorizontalPixel = 0x26;//水平每米像素数
    static constexpr __uint64 VerticalPixel = 0x2A;//垂直每米像素数
    static constexpr __uint64 USER_COLOR_NUM = 0x2E;//所用的颜色数
    static constexpr __uint64 COLOR_IMPORTANT = 0x31;//颜色的侧重性

    struct FILE_HEADER
    {
        __uint16 FileLogo = 0x4D42;
        __uint32 FileSize = 0;
        __uint32 Reserve = 0;
        __uint32 BeginPos = 1078;
    };

    struct FILE_INFO
    {
        __uint32 InfoSize = 40;
        __uint32 ImageWidth = 0;
        __uint32 ImageHeight = 0;
        __uint16 PlaneNumber = 1;
        __uint16 ColorBit = 8;
        __uint32 CompressWay = 0;
        __uint32 DataSize = 0;
        __uint32 HorizontalPixel = 2835;
        __uint32 VerticalPixel = 2835;
        __uint32 ColorUse = 0;
        __uint32 ColorImportant = 0;
    };

public:
    static _M_Base<__uint8>* parse(const char* filename)
    {
        FILE_HEADER *header = new FILE_HEADER;
        FILE_INFO *info = new FILE_INFO;
        const char *file_content = FileBaes::open(filename).data();
        __uint8 zero = 0;
        memcpy(info, file_content + InfoSize, sizeof(FILE_INFO));
        if(info->ColorBit > 8)
        {
            delete header;
            delete info;
            throw Exception("This is not a gray image");
        }
        memcpy(&header->BeginPos, file_content + DATA_BeginPos, sizeof(header->BeginPos));
        memcpy(&header->FileSize, file_content + FileSize, sizeof(header->FileSize));

        _M_Base<__uint8> *image = new _M_Base<__uint8>(info->ImageHeight, info->ImageWidth, zero);
        __uint64 index = header->BeginPos;
        __uint8 offset = info->ImageWidth % 4;
        if(offset != 0) offset = 4 - offset;
        for(__uint32 i = 0; i < image->line(); i++)
        {
            for(__uint32 j = 0; j < image->column(); j++)
            {
                image->set_value(i, j, file_content[index++]);
            }
            index += offset;
        }
        delete header;
        delete info;
        return image;
    }
    static void save(const char* filename, _M_Base<__uint8> *content)
    {
        struct FILE_HEADER *header = new struct FILE_HEADER;
        struct FILE_INFO *info = new struct FILE_INFO;
        info->ImageWidth = content->column();
        info->ImageHeight = content->line();
        info->DataSize = info->ImageHeight * info->ImageWidth;
        header->FileSize = 1078 + info->DataSize;
        struct RGB *rgb = new struct RGB[256];
        for(__uint16 i = 0; i < 256; i++)
        {
            rgb[i].R = i;
            rgb[i].B = i;
            rgb[i].G = i;
            rgb[i].A = 0;
        }
        char *file_content = new char[header->FileSize]{0};
        memcpy(file_content, &header->FileLogo, 2);
        memcpy(file_content + 2, &header->FileSize, 4);
        memcpy(file_content + 6, &header->Reserve, 4);
        memcpy(file_content + 10, &header->BeginPos, 4);
        memcpy(file_content + 14, info, 40);
        memcpy(file_content + 54, rgb, 1024);
        __uint64 index = 1078;
        __uint8 offset = info->ImageWidth % 4;
        if(offset != 0) offset = 4 - offset;
        for(__uint32 i = 0; i < content->line(); i++)
        {
            for(__uint32 j = 0; j < content->column(); j++)
            {
                file_content[index++] = content->get_value(i, j);
            }
            index += offset;
        }
        FileBaes::save(filename, file_content, header->FileSize);
        delete header;
        delete info;
        delete[] rgb;
    }
};

#endif // IMAGE_H
