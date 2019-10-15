#include "BMP.h"
#include "Core/Exception.h"
#include "Core/Matrix.h"
#include "Image/RGB.h"
#include <fstream>

static unsigned char get_value(unsigned char ch,
                               unsigned char index,
                               unsigned char unit_len)
{
    static constexpr unsigned char BIT_1_CONSTANT[] = {1, 2, 4, 8, 16, 32, 64, 128};
    static constexpr unsigned char BIT_2_CONSTANT[] = {0b11, 0b1100, 0b110000, 0b11000000};
    unsigned char ret = 0;
    if(unit_len == 1)
    {
        ret = bool(ch & BIT_1_CONSTANT[index]);
    }
    else if(unit_len == 2)
    {
        ret = (ch & BIT_2_CONSTANT[index]) >> (index * 2);
    }
    else if(unit_len == 4)
    {
        ret = index == 0 ? ch & 0x0F : ch >> 4;
    }
    return ret;
}

static rapid::RGBA get_RGB(void *ch, unsigned char unit_byte)
{
    using namespace rapid;
    unsigned char *c = static_cast<unsigned char *>(ch);
    switch(unit_byte)
    {
    case 1:return RGBA(*c, *c, *c);
    case 2:return RGB_16(*c, *(c + 1)).to_rgb();
    case 3:return RGBA(*(c + 2), *(c + 1), *c);
    case 4:return RGBA(*(c + 2), *(c + 1), *c, *(c + 3));
    default:return RGBA();
    }
}

static rapid::RGBA get_RGB(void *ch, unsigned char index, unsigned char bit_len)
{
    using namespace rapid;
    unsigned char *c = static_cast<unsigned char *>(ch);
    unsigned char ret = get_value(*c, index, bit_len);
    return RGBA(ret, ret, ret);
}

static rapid::RGBA get_RGB(void *ch, unsigned char unit_byte,
                          unsigned char index, unsigned char bit_len)
{
    if(unit_byte == 0) return get_RGB(ch, index, bit_len);
    return get_RGB(ch, unit_byte);
}

rapid::BMP::~BMP()
{ clear(); }

void rapid::BMP::_F_adjust_memory_order()
{
    if
    #if __cplusplus >= 201703L
        constexpr
    #endif
        (IsBigEndian::value)
    {
        if(_M_header_block == nullptr || _M_describe_info_block == nullptr) return;
        adjust_memory_order4(_M_header_block->FileSize);
        adjust_memory_order4(_M_header_block->DataBeginPosition);
        adjust_memory_order4(&_M_describe_info_block->BlockSize);
        adjust_memory_order4(&_M_describe_info_block->ImageWidth);
        adjust_memory_order4(&_M_describe_info_block->ImageHeight);
        adjust_memory_order2(&_M_describe_info_block->PlaneNumber);
        adjust_memory_order2(&_M_describe_info_block->ColorBit);
        adjust_memory_order4(&_M_describe_info_block->CompressWay);
        adjust_memory_order4(&_M_describe_info_block->DataSize);
        adjust_memory_order4(&_M_describe_info_block->HorizontalPixel);
        adjust_memory_order4(&_M_describe_info_block->VerticalPixel);
        adjust_memory_order4(&_M_describe_info_block->ColorUsed);
        adjust_memory_order4(&_M_describe_info_block->ColorImportant);
    }
}
void rapid::BMP::clear()
{
    if(_M_header_block == nullptr) return;
    char *ch = reinterpret_cast<char *>(_M_header_block);
    ::operator delete[](ch);
    _M_header_block = nullptr;
    _M_color_table = nullptr;
    _M_describe_info_block = nullptr;
    _M_data_content = nullptr;
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
    char *result = static_cast<char *>(::operator new[](static_cast<std::size_t>(length)));
    file.seekg(0);
    file.read(result, length);
    file.close();
    _M_header_block = reinterpret_cast<HeaderBlock*>(&result[0]);
    if(!is_bmp())
    {
        throw CannotParseFileException("CannotParseFileException: is not bmp file!");
    }
    _M_describe_info_block = reinterpret_cast<DescribeInfoBlock*>(&result[sizeof(HeaderBlock)]);
    if(_M_describe_info_block->DataSize == 0)
    {
        _M_describe_info_block->DataSize = _M_describe_info_block->ImageWidth * _M_describe_info_block->ImageHeight;
    }
    if(_M_describe_info_block->DataSize <= 0)
    {
        throw CannotParseFileException("CannotParseFileException: there is no data!");
    }
    if(_F_color_table_length() != 0)
    {
        _M_color_table = reinterpret_cast<RGBA*>(&result[sizeof(HeaderBlock) + sizeof(DescribeInfoBlock)]);
    }
    _M_data_content = &result[_F_data_begin_position()];
    _F_adjust_memory_order();
}

void rapid::BMP::parse(const Matrix<RGBA> &m)
{
    clear();
    size_type total_size = 54 + static_cast<size_type>(m.row() * m.column()) * sizeof(RGBA);
    char *content = static_cast<char *>(::operator new(static_cast<std::size_t>(total_size)));
    unsigned char *data = reinterpret_cast<unsigned char *>(content + 54);

    for(Matrix<RGBA>::SizeType i = 0; i < m.row(); ++i)
    {
        for(Matrix<RGBA>::SizeType j = 0; j < m.column(); ++j)
        {
            RGBA rgba = m.get_value(i, j);
            *data = rgba.Blue;
            *(data + 1) = rgba.Green;
            *(data + 2) = rgba.Red;
            *(data + 3) = rgba.Alpha;
            data += 4;
        }
    }

    _M_header_block = reinterpret_cast<HeaderBlock*>(&content[0]);
    _M_describe_info_block = reinterpret_cast<DescribeInfoBlock*>(&content[14]);
    _M_data_content = &content[54];

    ::new(_M_header_block) HeaderBlock();
    ::new(_M_describe_info_block) DescribeInfoBlock();

    _M_describe_info_block->ImageWidth = m.column();
    _M_describe_info_block->ImageHeight = -m.row();
    _M_describe_info_block->DataSize = m.row() * m.column();

    *reinterpret_cast<int *>(_M_header_block->FileSize) = static_cast<int>(total_size);
}

void rapid::BMP::_F_write_matrix(Matrix<RGBA> &m, char *visit,
                                 unsigned char pixel_byte,
                                 int row, int column)
{
    unsigned char temp = static_cast<unsigned char>(8 / color_bit());
    RGBA rgb = get_RGB(visit, static_cast<unsigned char>(pixel_byte),
                      static_cast<unsigned char>(temp == 0 ? 0 : (column % temp)),
                      static_cast<unsigned char>(color_bit()));
    if(_M_describe_info_block->ImageHeight > 0)
    {
        m.set_value(height() - 1 - row, column, rgb);
    }
    else
    {
        m.set_value(row, column, rgb);
    }
}

char *rapid::BMP::_F_next_pixel(char *visit, int pixel_byte, int column)
{
    if(pixel_byte > 0)
    {
        return visit + pixel_byte;
    }
    else if(column != 0 && column % (8 / color_bit()) == 0)
    {
        return visit + 1;
    }
    return visit;
}

rapid::Matrix<rapid::RGBA> rapid::BMP::to_matrix()
{
    Matrix<RGBA> m(height(), width());
    float pixel_byte = static_cast<unsigned char>(color_bit() / 8);
    unsigned char skip = (4 - (width() * static_cast<unsigned char>(pixel_byte)) % 4) % 4;
    char *visit = const_cast<char *>(_M_data_content);
    if(_M_color_table == nullptr)
    {
        for(int i = 0; i < height(); ++i)
        {
            for(int j = 0; j < width(); ++j)
            {
                _F_write_matrix(m, visit, static_cast<unsigned char>(pixel_byte), i, j);
                visit = _F_next_pixel(visit, static_cast<int>(pixel_byte), j);
            }
            visit += skip;
        }
    }
    else
    {
        for(int i = height() - 1; i > 0; --i)
        {
            for(int j = 0; j < width(); ++j)
            {
                m.set_value(i, j, _M_color_table[get_value(static_cast<unsigned char>(*visit),
                                                           static_cast<unsigned char>(j % (8 / color_bit())),
                                                           static_cast<unsigned char>(color_bit()))]);
                visit = _F_next_pixel(visit, static_cast<int>(pixel_byte), j);
            }
            visit += skip;
        }
    }
    return m;
}

void rapid::BMP::write(const char *filename)
{
    _F_adjust_memory_order();
    if(_M_data_content == nullptr)
    {
        throw CannotWriteFileException("CannotWriteFileException: there is no data!");
    }
    std::ofstream file(filename, std::ios::binary | std::ios::out | std::ios::trunc);
    if(!file)
    {
        throw CannotWriteFileException("CannotWriteFileException: cannot open file!");
    }
    file.write(reinterpret_cast<char *>(_M_header_block), file_size());
    file.close();
}
