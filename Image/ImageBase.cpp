#include "ImageBase.h"
#include "Core/Memory.h"
#include "Core/Exception.h"
#include "Core/Matrix.h"
#include "Image/rgb.h"
#include <fstream>

rapid::ImageBaseInterface::~ImageBaseInterface()
{ }

void rapid::BMP::_F_adjust_memory_order4(char *begin)
{
    if
    #if __cplusplus >= 201703L
        constexpr
    #endif
        (IsLittleEndian::value)
    {
        char ch = *begin;
        *begin = *(begin + 3);
        *(begin + 3) = ch;
        ch = *(begin + 1);
        *(begin + 1) = *(begin + 2);
        *(begin + 2) = ch;
    }
}

void rapid::BMP::_F_adjust_memory_order2(char *begin)
{
    if
    #if __cplusplus >= 201703L
        constexpr
    #endif
        (IsLittleEndian::value)
    {
        char ch = *begin;
        *begin = *(begin + 1);
        *(begin + 1) = ch;
    }
}

void rapid::BMP::_F_adjust_memory_order()
{
    if(_M_header_block == nullptr || _M_describe_info_block == nullptr) return;
    _F_adjust_memory_order4(_M_header_block->FileSize);
    _F_adjust_memory_order4(_M_header_block->DataBeginPosition);
    _F_adjust_memory_order4(reinterpret_cast<char *>(&_M_describe_info_block->BlockSize));
    _F_adjust_memory_order4(reinterpret_cast<char *>(&_M_describe_info_block->ImageWidth));
    _F_adjust_memory_order4(reinterpret_cast<char *>(&_M_describe_info_block->ImageHeight));
    _F_adjust_memory_order2(reinterpret_cast<char *>(&_M_describe_info_block->PlaneNumber));
    _F_adjust_memory_order2(reinterpret_cast<char *>(&_M_describe_info_block->ColorBit));
    _F_adjust_memory_order4(reinterpret_cast<char *>(&_M_describe_info_block->CompressWay));
    _F_adjust_memory_order4(reinterpret_cast<char *>(&_M_describe_info_block->DataSize));
    _F_adjust_memory_order4(reinterpret_cast<char *>(&_M_describe_info_block->HorizontalPixel));
    _F_adjust_memory_order4(reinterpret_cast<char *>(&_M_describe_info_block->VerticalPixel));
    _F_adjust_memory_order4(reinterpret_cast<char *>(&_M_describe_info_block->ColorUsed));
    _F_adjust_memory_order4(reinterpret_cast<char *>(&_M_describe_info_block->ColorImportant));
}

rapid::BMP::~BMP()
{ clear(); }

void rapid::BMP::clear()
{
    if(_M_header_block == nullptr) return;
    const char *ch = reinterpret_cast<const char *>(_M_header_block);
    delete[] ch;
}

void rapid::BMP::parse(const char *filename)
{
    if(filename == nullptr) return;
    clear();
    std::ifstream file(filename, std::ios::binary | std::ios::in);
    if(!file)
    {
        throw CannotParseFileException("CannotParseFileException: cannot open file!");
    }
    file.seekg(0, std::ios::end);
    long long length = file.tellg();
    if(length < 54)
    {
        throw CannotParseFileException("CannotParseFileException: file size is to small!");
    }
    char *result = new char[static_cast<unsigned long long>(length) + 1];
    file.seekg(0);
    file.read(result, length);
    result[length] = 0;
    _M_header_block = reinterpret_cast<HeaderBlock*>(&result[0]);
    if(!is_bmp())
    {
        throw CannotParseFileException("CannotParseFileException: is not bmp file!");
    }
    _M_describe_info_block = reinterpret_cast<DescribeInfoBlock*>(&result[sizeof(HeaderBlock)]);
    if(_M_describe_info_block->DataSize <= 0)
    {
        throw CannotParseFileException("CannotParseFileException: there is no data!");
    }
    if(_F_color_table_length() != 0)
    {
        _M_color_table = reinterpret_cast<RGB*>(&result[sizeof(HeaderBlock) + sizeof(DescribeInfoBlock)]);
    }
    _M_data_content = &result[_F_data_begin_position()];
    _F_adjust_memory_order();
}

void rapid::BMP::parse(Matrix<RGB> *m)
{
    clear();
    int total_size = 54 + m->row() * m->column();
    char *content = new char[static_cast<unsigned long long>(total_size)];

    ::new(&content[0]) HeaderBlock();
    ::new(&content[14]) DescribeInfoBlock();

    _M_describe_info_block = reinterpret_cast<DescribeInfoBlock*>(&content[14]);
    _M_header_block = reinterpret_cast<HeaderBlock*>(&content[0]);
    _M_color_table = reinterpret_cast<RGB*>(&content[54]);
    _M_data_content = &content[1074];

    _M_describe_info_block->ImageWidth = m->column();
    _M_describe_info_block->ImageHeight = m->row();
    _M_describe_info_block->DataSize = m->row() * m->column();

    *reinterpret_cast<int *>(_M_header_block->FileSize) = total_size;
}

void rapid::BMP::to_matrix(Matrix<RGB> *m)
{
    if(m == nullptr || _M_data_content == nullptr) return;
#ifdef cpp17
        [[maybe_unused]]
#endif
    unsigned char skip = 0;
    unsigned char pixel_byte = static_cast<unsigned char>(color_bit() / 8);
    // there is no color table
    if(reinterpret_cast<const char *>(_M_data_content) == reinterpret_cast<const char *>(_M_color_table))
    {
        skip = (4 - (width() * pixel_byte) % 4) % 4;
        for(int i = width() - 1; i > 0; i--)
        {
            for(int j = 0; j < height(); j++)
            {
                if(pixel_byte == 4)
                {
                    char *ch = const_cast<char *>(&_M_data_content[(width() - 1 - i) * pixel_byte + j * pixel_byte]);
                    m->set_value(i, j, *reinterpret_cast<RGB*>(ch));
                }
            }
        }
    }
    else
    {

    }
}

void rapid::BMP::write(const char *filename)
{
    _F_adjust_memory_order();
    if(_M_data_content == nullptr)
    {
        throw CannotWriteFileException("CannotParseFileException: there is no data!");
    }
    std::ofstream file(filename, std::ios::binary | std::ios::out | std::ios::trunc);
    if(!file)
    {
        throw CannotWriteFileException("CannotParseFileException: cannot open file!");
    }
    file.write(reinterpret_cast<char *>(_M_header_block), file_size());
}
