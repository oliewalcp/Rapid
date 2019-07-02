#ifndef IMAGE_H
#define IMAGE_H
#include "Math/MathBase.h"
#include "FileBase.h"
#include "Exception.h"
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
        __uint32 BeginPos = 54;

        void print()
        {
            std::cout << "file logo: " << FileLogo << std::endl;
            std::cout << "file size: " << FileSize << std::endl;
            std::cout << "reserve: " << Reserve << std::endl;
            std::cout << "begin position: " << BeginPos << std::endl;
        }
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

        void print()
        {
            std::cout << "file info size: " << InfoSize << std::endl;
            std::cout << "image width: " << ImageWidth << std::endl;
            std::cout << "image height: " << ImageHeight << std::endl;
            std::cout << "plane number: " << PlaneNumber << std::endl;
            std::cout << "color bit: " << ColorBit << std::endl;
            std::cout << "compress way: " << CompressWay << std::endl;
            std::cout << "data size: " << DataSize << std::endl;
            std::cout << "horizontal pixel: " << HorizontalPixel << std::endl;
            std::cout << "vertical pixel: " << VerticalPixel << std::endl;
            std::cout << "color use: " << ColorUse << std::endl;
            std::cout << "color important: " << ColorImportant << std::endl;
        }
    };

public:
    /* parse the bmp file
     * param[filename]: name of the bmp image file
     * return: a matrix of pixels, you must delete by yourself
     */
    static _M_Base<__uint8>* parse_gray(const char* filename)
    {
        std::cout << "read file: " << filename << std::endl;
        std::cout << "start parsing" << std::endl;
        __uint32 temp;
        const char *file_content = FileBase::open(filename, temp);
        std::cout << "read file content successfully" << std::endl;
        temp = 0;
        FILE_HEADER *header = new FILE_HEADER;
        FILE_INFO *info = new FILE_INFO;
        std::cout << "start reading header information" << std::endl;
        memcpy(info, file_content + INFO_SIZE, sizeof(FILE_INFO));
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
        std::cout << "finish reading header information" << std::endl;
        std::cout << "start reading data" << filename << std::endl;
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
        std::cout << "finish reading data" << filename << std::endl;
        delete[] file_content;
        delete header;
        delete info;
        std::cout << "finish reading operator" << std::endl;
        return image;
    }
    static _M_Base<RGB>* parse_color(const char* filename)
    {
        std::cout << "read file: " << filename << std::endl;
        std::cout << "start parsing" << std::endl;
        __uint32 pixel_byte_size;
        const char *file_content = FileBase::open(filename, pixel_byte_size);//[pixel_byte_size] is useless here
        std::cout << "read file content successfully" << std::endl;
        FILE_HEADER *header = new FILE_HEADER;
        FILE_INFO *info = new FILE_INFO;
        std::cout << "start reading header information" << std::endl;
        memcpy(info, file_content + INFO_SIZE, sizeof(FILE_INFO));
        if(info->ColorBit < 16)
        {
            delete header;
            delete info;
            throw Exception("This is not a gray image");
        }
        else pixel_byte_size = info->ColorBit / 8;
        memcpy(&header->BeginPos, file_content + DATA_BEGIN_POS, sizeof(header->BeginPos));
        memcpy(&header->FileSize, file_content + FILE_SIZE, sizeof(header->FileSize));
        memcpy(&header->FileLogo, file_content + FILE_LOGO, sizeof(header->FileLogo));
        memcpy(&header->Reserve, file_content + RESERVE, sizeof(header->Reserve));
        std::cout << "finish reading header information" << std::endl;
        std::cout << "start reading data" << filename << std::endl;
        _M_Base<RGB> *image = new _M_Base<RGB>(info->ImageHeight, info->ImageWidth, 0);
        __uint64 index = header->BeginPos;
        __uint8 offset = (info->ImageWidth * pixel_byte_size) % 4;
        if(offset != 0) offset = 4 - offset;
        for(__uint32 i = image->line() - 1; i > 0; i--)
        {
            for(__uint32 j = 0; j < image->column(); j++)
            {
                image->set_value(i, j, file_content[index++]);
            }
            index += offset;
        }
        std::cout << "finish reading data" << filename << std::endl;
        delete[] file_content;
        delete header;
        delete info;
        std::cout << "finish reading operator" << std::endl;
        return image;
    }
    /* save to a bmp file
     * param[filename]: name of the bmp image file
     * param[content]: a matrix of pixels
     */
    template<typename T>
    static void save_8bit(const char* filename, _M_Base<T> *content)
    {
        std::cout << "save as " << filename << std::endl;
        std::cout << "start writing header information" << std::endl;
        struct FILE_HEADER *header = new struct FILE_HEADER;
        struct FILE_INFO *info = new struct FILE_INFO;
        info->ImageWidth = content->column();
        info->ImageHeight = content->line();
        info->DataSize = content->column() * content->line();
        if(sizeof(T) == 1) header->BeginPos = 1078;
        header->FileSize = header->BeginPos + info->DataSize;
        memcpy(file_content, &header->FileLogo, 2);
        memcpy(file_content + FILE_SIZE, &header->FileSize, 4);
        memcpy(file_content + RESERVE, &header->Reserve, 4);
        memcpy(file_content + DATA_BEGIN_POS, &header->BeginPos, 4);
        memcpy(file_content + INFO_SIZE, info, 40);
        std::cout << "finish writing header information" << std::endl;
        std::cout << "start writing palette" << std::endl;
        char *file_content = new char[header->FileSize]{0};
        // write palette
        if(sizeof(T) == 1)
        {
            for(__uint16 i = 0; i < 1024; i++)
            {
                if(i % 4 != 3) file_content[54 + i] = i / 4;
                else file_content[54 + i] = 0;
            }
        }
        std::cout << "finish writing palette" << std::endl;
        std::cout << "start writing data" << std::endl;
        __uint64 index = header->BeginPos;
        __uint8 offset = info->ImageWidth % 4;
        if(offset != 0) offset = 4 - offset;
        for(__uint32 i = content->line() - 1; i > 0; i--)
        {
            for(__uint32 j = 0; j < content->column(); j++)
            {
                memcpy(file_content + index, &(content->get_value(i, j)), sizeof(T));
                index += sizeof(T);
            }
            index += offset;
        }
        std::cout << "finish writing data" << std::endl;
        FileBase::save(filename, file_content, header->FileSize);
        delete header;
        delete info;
        std::cout << "finish saving operator" << std::endl;
    }
    template<typename T>
    static void save_16bit(const char *filename, _M_Base<T> *content) {}
    template<typename T>
    static void save_24bit(const char *filename, _M_Base<T> *content) {}
    template<typename T>
    static void save_32bit(const char *filename, _M_Base<T> *content) {}
};

struct PNG
{
private:
    typedef unsigned int __uint8;
public:
    /* parse the bmp file
     * param[filename]: name of the bmp image file
     * return: a matrix of pixels, you must delete by yourself
     */
    static _M_Base<__uint8>* parse_gray(const char* filename)
    {

    }
    static _M_Base<RGB>* parse_color(const char* filename)
    {

    }
    /* save to a bmp file
     * param[filename]: name of the bmp image file
     * param[content]: a matrix of pixels
     */
    template<typename T>
    static void save_8bit(const char* filename, _M_Base<T> *content)
    {

    }
    template<typename T>
    static void save_16bit(const char *filename, _M_Base<T> *content) {}
    template<typename T>
    static void save_24bit(const char *filename, _M_Base<T> *content) {}
    template<typename T>
    static void save_32bit(const char *filename, _M_Base<T> *content) {}
};

struct JPG
{
private:
    typedef unsigned int __uint8;
public:
    /* parse the bmp file
     * param[filename]: name of the bmp image file
     * return: a matrix of pixels, you must delete by yourself
     */
    static _M_Base<__uint8>* parse_gray(const char* filename)
    {

    }
    static _M_Base<RGB>* parse_color(const char* filename)
    {

    }
    /* save to a bmp file
     * param[filename]: name of the bmp image file
     * param[content]: a matrix of pixels
     */
    template<typename T>
    static void save_8bit(const char* filename, _M_Base<T> *content)
    {

    }
    template<typename T>
    static void save_16bit(const char *filename, _M_Base<T> *content) {}
    template<typename T>
    static void save_24bit(const char *filename, _M_Base<T> *content) {}
    template<typename T>
    static void save_32bit(const char *filename, _M_Base<T> *content) {}
};

struct JPEG
{
private:
    typedef unsigned int __uint8;
public:
    /* parse the bmp file
     * param[filename]: name of the bmp image file
     * return: a matrix of pixels, you must delete by yourself
     */
    static _M_Base<__uint8>* parse_gray(const char* filename)
    {

    }
    static _M_Base<RGB>* parse_color(const char* filename)
    {

    }
    /* save to a bmp file
     * param[filename]: name of the bmp image file
     * param[content]: a matrix of pixels
     */
    template<typename T>
    static void save_8bit(const char* filename, _M_Base<T> *content)
    {

    }
    template<typename T>
    static void save_16bit(const char *filename, _M_Base<T> *content) {}
    template<typename T>
    static void save_24bit(const char *filename, _M_Base<T> *content) {}
    template<typename T>
    static void save_32bit(const char *filename, _M_Base<T> *content) {}
};

#endif // IMAGE_H
