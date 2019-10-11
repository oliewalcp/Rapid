#include "ImageBase.h"
#include "Core/Memory.h"
#include "Core/Exception.h"
#include "Core/Matrix.h"
#include <fstream>

rapid::ImageBaseInterface::~ImageBaseInterface() { }

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
    std::ifstream file;
    file.open(filename, std::ios::binary);
    if(!file.is_open())
    {
        throw CannotParseFileException("CannotParseFileException: cannot open file!");
    }
    if(_M_data_ontent != nullptr)
    {
        delete[] _M_data_ontent;
    }
    file.seekg(0, std::ios::end);
    long long length = file.tellg();
    if(length < 54)
    {
        throw CannotParseFileException("CannotParseFileException: file size is to small!");
    }
    char *result = new char[static_cast<unsigned long long>(length)];
    file.read(result, length);
    _M_header_block = reinterpret_cast<HeaderBlock*>(&result[0]);
    _M_describe_info_block = reinterpret_cast<DescribeInfoBlock*>(&result[sizeof(HeaderBlock)]);
    if(_M_describe_info_block->DataSize <= 0)
    {
        throw CannotParseFileException("CannotParseFileException: there is no data!");
    }
    _M_color_table = reinterpret_cast<RGB*>(&result[sizeof(HeaderBlock) + sizeof(DescribeInfoBlock)]);
    _M_data_ontent = &result[__data_begin_position()];
}

void rapid::BMP::parse(
#ifdef cpp17
        [[maybe_unused]]
#endif
Matrix<rapid::RGB> *m)
{

}

void rapid::BMP::to_matrix(Matrix<RGB> *m)
{
    if(m == nullptr || _M_data_ontent == nullptr) return;
#ifdef cpp17
        [[maybe_unused]]
#endif
    unsigned char skip = 0;
    unsigned char pixel_byte = static_cast<unsigned char>(color_bit() / 8);
    // there is no color table
    if(reinterpret_cast<const char *>(_M_data_ontent) == reinterpret_cast<const char *>(_M_color_table))
    {
        skip = (4 - (width() * pixel_byte) % 4) % 4;
        for(int i = width() - 1; i > 0; i--)
        {
            for(int j = 0; j < height(); j++)
            {
                if(pixel_byte == 4)
                {
                    char *ch = const_cast<char *>(&_M_data_ontent[(width() - 1 - i) * pixel_byte + j * pixel_byte]);
                    m->set_value(i, j, *reinterpret_cast<RGB*>(ch));
                }
            }
        }
    }
    else
    {

    }
}
