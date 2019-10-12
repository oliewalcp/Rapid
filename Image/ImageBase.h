#ifndef IMAGEBASE_H
#define IMAGEBASE_H

#include "Core/Version.h"
#include "Core/TypeTraits.h"

namespace rapid
{

template<typename _Tp>
class Matrix;

struct RGB;

enum class ImageType : unsigned char
{
    BMP,
    UNKNOWN
};

class ImageBaseInterface
{
protected:
    const char *_M_data_content = nullptr;
public:
    virtual ~ImageBaseInterface();
    virtual void parse(const char *filename) = 0;
    virtual void to_matrix(Matrix<RGB>*) = 0;
    virtual void parse(Matrix<RGB>*) = 0;
    virtual void write(const char *filename) = 0;
};

class BMP : public ImageBaseInterface
{
private:
    struct HeaderBlock
    {
        char FileFlag[2]{'B', 'M'};
        char FileSize[4]{0};
        char Reverse[4]{0};
        char DataBeginPosition[4];

        HeaderBlock()
        { *reinterpret_cast<unsigned int *>(DataBeginPosition) = 54; }
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
    HeaderBlock *_M_header_block = nullptr;
    RGB *_M_color_table = nullptr;
    DescribeInfoBlock *_M_describe_info_block = nullptr;

    inline void _F_adjust_memory_order4(char *begin);
    inline void _F_adjust_memory_order2(char *begin);
    inline void _F_adjust_memory_order();
    int _F_data_begin_position() const
    { return *reinterpret_cast<int *>(&_M_header_block->DataBeginPosition[0]); }
    int _F_color_table_length() const
    { return static_cast<int>(
                    static_cast<unsigned long long>(_F_data_begin_position()) -
                    sizeof(HeaderBlock) - sizeof(DescribeInfoBlock));
    }
public:
    BMP(Matrix<RGB> *m)
    { parse(m); }
    BMP(const char *filename = nullptr)
    { parse(filename); }
    virtual ~BMP() override;

    int file_size() const
    { return _M_header_block == nullptr ? 0 : *reinterpret_cast<int *>(&_M_header_block->FileSize[0]); }
    bool is_bmp() const
    { return _M_header_block->FileFlag[0] == 'B' && _M_header_block->FileFlag[1] == 'M'; }
    int width() const
    { return _M_describe_info_block == nullptr ? 0 : _M_describe_info_block->ImageWidth; }
    int height() const
    { return _M_describe_info_block == nullptr ? 0 : _M_describe_info_block->ImageHeight; }
    short color_bit() const
    { return _M_describe_info_block == nullptr ? 0 : _M_describe_info_block->ColorBit; }
    void clear();

    virtual void parse(const char *filename) override;
    virtual void parse(Matrix<RGB> *m) override;
    virtual void to_matrix(Matrix<RGB> *m) override;
    virtual void write(const char *filename) override;
};

}

#endif // IMAGEBASE_H
