#ifndef IMAGEBASE_H
#define IMAGEBASE_H
#include "Core/Version.h"
#include "Core/Memory.h"
#include "Core/Matrix.h"

namespace rapid
{

struct RGB
{
private:
    union {
        unsigned int Total;
        struct {
            unsigned char Blue;
            unsigned char Green;
            unsigned char Red;
            unsigned char Alpha;
        } Part;
    } Data;
public:
    unsigned char& Red() { return Data.Part.Red; }
    unsigned char& Green() { return Data.Part.Green; }
    unsigned char& Blue() { return Data.Part.Blue; }
    unsigned char& Alpha() { return Data.Part.Alpha; }
    RGB(unsigned int arg)
    { Data.Total = arg; }
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

class ImageBaseInterface
{
protected:
    const char *_DataContent = nullptr;
public:
    virtual ~ImageBaseInterface();
    virtual bool check_file_type() = 0;
    virtual void parse(const char *filename) = 0;
    virtual void to_matrix(Matrix<RGB>*) = 0;
    virtual void parse(Matrix<RGB>*) = 0;
};

class BMP : public ImageBaseInterface
{
private:
    struct HeaderBlock
    {
        char FileFlag[2]{'B', 'M'};
        char FileSize[4]{0};
        char Reverse[4]{0};
        char DataBeginPosition[4]{0, 0, 0, 54};
    };
    struct DescribeInfoBlock
    {
        int BlockSize = 40;
        int ImageWidth = 0;
        int ImageHeight = 0;
        short PlaneNumber = 1;
        short ColorBit = 32;
        int CompressWay = 0;
        int DataSize = 0;
        int HorizontalPixel = 2835;
        int VerticalPixel = 2835;
        int ColorUsed = 0;
        int ColorImportant = 0;
    };
    HeaderBlock *__HeaderBlock = nullptr;
    RGB *__ColorTable = nullptr;
    DescribeInfoBlock *__DescribeInfoBlock = nullptr;

    int __data_begin_position()
    { return *reinterpret_cast<int *>(&__HeaderBlock->DataBeginPosition[0]); }
    int __color_table_length()
    {
        return static_cast<int>(static_cast<unsigned long long>(__data_begin_position()) - sizeof(HeaderBlock) - sizeof(DescribeInfoBlock));
    }
public:
    BMP(Matrix<RGB> *m)
    { parse(m); }
    BMP(const char *filename = nullptr)
    { parse(filename); }
    virtual ~BMP() override;

    int file_size()
    { return __HeaderBlock == nullptr ? 0 : *reinterpret_cast<int *>(&__HeaderBlock->FileSize[0]); }
    bool is_bmp()
    { return __HeaderBlock->FileFlag[0] == 'B' && __HeaderBlock->FileFlag[1] == 'M'; }
    int width()
    { return __DescribeInfoBlock == nullptr ? 0 : __DescribeInfoBlock->ImageWidth; }
    int height()
    { return __DescribeInfoBlock == nullptr ? 0 : __DescribeInfoBlock->ImageHeight; }
    short color_bit()
    { return __DescribeInfoBlock == nullptr ? 0 : __DescribeInfoBlock->ColorBit; }
    void clear();

    virtual bool check_file_type() override
    { return is_bmp(); }

    virtual void parse(const char *filename) override;
    virtual void parse(Matrix<RGB> *m) override;
    virtual void to_matrix(Matrix<RGB> *m) override;
};

}

#endif // IMAGEBASE_H
