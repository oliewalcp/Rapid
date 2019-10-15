#ifndef BMP_H
#define BMP_H


#include "Image/ImageBase.h"

namespace rapid
{

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
    RGBA *_M_color_table = nullptr;
    DescribeInfoBlock *_M_describe_info_block = nullptr;

    void _F_adjust_memory_order();
    int _F_data_begin_position() const
    { return *reinterpret_cast<int *>(&_M_header_block->DataBeginPosition[0]); }
    int _F_color_table_length() const
    { return static_cast<int>(
                    static_cast<unsigned long long>(_F_data_begin_position()) -
                    sizeof(HeaderBlock) - sizeof(DescribeInfoBlock));
    }

    void _F_write_matrix(Matrix<RGBA> &m, char *visit, unsigned char pixel_bype,
                         int row, int column);
    char* _F_next_pixel(char *visit, int pixel_byte, int column);
public:

    BMP(const Matrix<RGBA> &m)
    { parse(m); }
    BMP(const char *filename = nullptr)
    { parse(filename); }
    virtual ~BMP() override;

    int file_size() const
    { return _M_header_block == nullptr ? 0 : *reinterpret_cast<int *>(&_M_header_block->FileSize[0]); }
    bool is_bmp() const
    { return _M_header_block == nullptr ? false :
                                          _M_header_block->FileFlag[0] == 'B' &&
                                          _M_header_block->FileFlag[1] == 'M'; }
    int width() const
    { return _M_describe_info_block == nullptr ? 0 : _M_describe_info_block->ImageWidth; }
    int height() const
    { return _M_describe_info_block == nullptr ? 0 : (_M_describe_info_block->ImageHeight < 0 ?
                                                          -_M_describe_info_block->ImageHeight :
                                                          _M_describe_info_block->ImageHeight); }
    short color_bit() const
    { return _M_describe_info_block == nullptr ? 0 : _M_describe_info_block->ColorBit; }
    void clear();

    virtual void parse(const char *filename) override;
    virtual void parse(const Matrix<RGBA> &m) override;
    virtual Matrix<RGBA> to_matrix() override;
    virtual void write(const char *filename) override;
};

}

#endif // BMP_H
