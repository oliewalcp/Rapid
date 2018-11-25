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

    static constexpr __uint64 FILE_LOGO = 0;//file logo
    static constexpr __uint64 FILE_SIZE = 0x02;//the size of file
    static constexpr __uint64 RESERVE = 0x06;//reserve
    static constexpr __uint64 DATA_BEGIN_POS = 0x0A;//begin posigion of image data

    static constexpr __uint64 INFO_SIZE = 0x0E;//the size of image information block
    static constexpr __uint64 IMAGE_WIDTH = 0x12;//image width of pixel
    static constexpr __uint64 IMAGE_HEIGHT = 0x16;//image height of pixel
    static constexpr __uint64 PLANE_NUMBER = 0x1A;//the number of iamge plane
    static constexpr __uint64 COLOR_BIT = 0x1C;//the number of bits of one pixel taking up
    static constexpr __uint64 COMPRESS_WAY = 0x1E;//data compress way
    static constexpr __uint64 DATA_SIZE = 0x22;//the size of data
    static constexpr __uint64 HORIZONTAL_PIXEL = 0x26;//horizontal dpi
    static constexpr __uint64 VERTICAL_PIXEL = 0x2A;//vertical dpi
    static constexpr __uint64 USER_COLOR_NUM = 0x2E;//
    static constexpr __uint64 COLOR_IMPORTANT = 0x31;//

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
        int ImageHeight = 0;
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
    /* parse the bmp file
     * param[filename]: name of the bmp image file
     * return: a matrix of pixels, you must delete by yourself
     */
    static _M_Base<__uint8>* parse(const char* filename)
    {
        const char *file_content = FileBaes::open(filename);
        FILE_HEADER *header = new FILE_HEADER;
        FILE_INFO *info = new FILE_INFO;

        memcpy(info, file_content + INFO_SIZE, sizeof(FILE_INFO));
        std::cout << "********** input ***********" << std::endl;
        std::cout << "InfoSize = " << info->InfoSize << std::endl;
        std::cout << "ImageWidth = " << info->ImageWidth << std::endl;
        std::cout << "ImageHeight = " << info->ImageHeight << std::endl;
        std::cout << "PlaneNumber = " << info->PlaneNumber << std::endl;
        std::cout << "ColorBit = " << info->ColorBit << std::endl;
        std::cout << "CompressWay = " << info->CompressWay << std::endl;
        std::cout << "DataSize = " << info->DataSize << std::endl;
        std::cout << "HorizontalPixel = " << info->HorizontalPixel << std::endl;
        std::cout << "VerticalPixel = " << info->VerticalPixel << std::endl;
        std::cout << "ColorUse = " << info->ColorUse << std::endl;
        std::cout << "ColorImportant = " << info->ColorImportant << std::endl;
        if(info->ColorBit > 8)
        {
            delete header;
            delete info;
            throw Exception("This is not a gray image");
        }
        memcpy(&header->BeginPos, file_content + DATA_BEGIN_POS, sizeof(header->BeginPos));
        memcpy(&header->FileSize, file_content + FILE_SIZE, sizeof(header->FileSize));
        memcpy(&header->FileLogo, file_content + FILE_LOGO, sizeof(header->FileLogo));
        memcpy(&header->Reserve, file_content + RESERVE, sizeof(header->Reserve));
        std::cout << "********** input ***********" << std::endl;
        std::cout << "FileLogo = " << header->FileLogo << std::endl;
        std::cout << "FileSize = " << header->FileSize << std::endl;
        std::cout << "Reserve = " << header->Reserve << std::endl;
        std::cout << "BeginPos = " << header->BeginPos << std::endl;

        _M_Base<__uint8> *image = new _M_Base<__uint8>(info->ImageHeight, info->ImageWidth, 0);
        __uint64 index = header->BeginPos;
        __uint8 offset = info->ImageWidth % 4;
        if(offset != 0) offset = 4 - offset;
        for(__uint32 i = image->line() - 1; i > 0; i--)
        {
            for(__uint32 j = 0; j < image->column(); j++)
            {
                image->set_value(i, j, file_content[index++]);
            }
            index += offset;
        }
        delete[] file_content;
        delete header;
        delete info;
        return image;
    }
    /* save to a bmp file
     * param[filename]: name of the bmp image file
     * param[content]: a matrix of pixels
     */
    static void save(const char* filename, _M_Base<__uint8> *content)
    {
        struct FILE_HEADER *header = new struct FILE_HEADER;
        struct FILE_INFO *info = new struct FILE_INFO;
        info->ImageWidth = content->column();
        info->ImageHeight = content->line();
        info->DataSize = content->column() * content->line();

        header->FileSize = 1078 + info->DataSize;
        char *file_content = new char[header->FileSize]{0};
        for(__uint16 i = 0; i < 1024; i++)
        {
            if(i % 4 != 3) file_content[54 + i] = i / 4;
            else file_content[54 + i] = 0;
        }
        std::cout << "********** output ***********" << std::endl;
        std::cout << "InfoSize = " << info->InfoSize << std::endl;
        std::cout << "ImageWidth = " << info->ImageWidth << std::endl;
        std::cout << "ImageHeight = " << info->ImageHeight << std::endl;
        std::cout << "PlaneNumber = " << info->PlaneNumber << std::endl;
        std::cout << "ColorBit = " << info->ColorBit << std::endl;
        std::cout << "CompressWay = " << info->CompressWay << std::endl;
        std::cout << "DataSize = " << info->DataSize << std::endl;
        std::cout << "HorizontalPixel = " << info->HorizontalPixel << std::endl;
        std::cout << "VerticalPixel = " << info->VerticalPixel << std::endl;
        std::cout << "ColorUse = " << info->ColorUse << std::endl;
        std::cout << "ColorImportant = " << info->ColorImportant << std::endl;

        std::cout << "********** output ***********" << std::endl;
        std::cout << "BeginPos = " << header->BeginPos << std::endl;
        std::cout << "FileSize = " << header->FileSize << std::endl;
        std::cout << "FileLogo = " << header->FileLogo << std::endl;
        std::cout << "Reserve = " << header->Reserve << std::endl;

        memcpy(file_content, &header->FileLogo, 2);
        memcpy(file_content + FILE_SIZE, &header->FileSize, 4);
        memcpy(file_content + RESERVE, &header->Reserve, 4);
        memcpy(file_content + DATA_BEGIN_POS, &header->BeginPos, 4);
        memcpy(file_content + INFO_SIZE, info, 40);
        __uint64 index = 1078;
        __uint8 offset = info->ImageWidth % 4;
        if(offset != 0) offset = 4 - offset;
        for(__uint32 i = content->line() - 1; i > 0; i--)
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
    }
};

#endif // IMAGE_H
