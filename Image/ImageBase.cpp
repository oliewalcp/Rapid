#include "ImageBase.h"
#include "Core/Exception.h"
#include <fstream>

rapid::ImageBaseInterface::~ImageBaseInterface() { }

rapid::RGB rapid::RGB::operator*=(double arg)
{
    if(arg < 0)
    {
        Red() = Green() = Blue() = 0;
    }
    else
    {
        if(static_cast<int>(Red()) * arg > 255)
        {
            Red() = 255;
        }
        else
        {
            Red() = static_cast<unsigned char>(Red() * arg);
        }
        if(static_cast<int>(Green()) * arg > 255)
        {
            Green() = 255;
        }
        else
        {
            Green() = static_cast<unsigned char>(Green() * arg);
        }
        if(static_cast<int>(Blue()) * arg > 255)
        {
            Blue() = 255;
        }
        else
        {
            Blue() = static_cast<unsigned char>(Blue() * arg);
        }
    }
    return *this;
}

rapid::RGB rapid::RGB::operator+=(double arg)
{
    int r = static_cast<int>(Red()), g = static_cast<int>(Green()), b = static_cast<int>(Blue());
    r += static_cast<int>(arg);
    b += static_cast<int>(arg);
    g += static_cast<int>(arg);
    if(r < 0)
    {
        Red() = 0;
    }
    else if(r > 255)
    {
        Red() = 255;
    }
    else
    {
        Red() = static_cast<unsigned char>(r);
    }
    if(r < 0)
    {
        Green() = 0;
    }
    else if(r > 255)
    {
        Green() = 255;
    }
    else
    {
        Green() = static_cast<unsigned char>(r);
    }
    if(r < 0)
    {
        Blue() = 0;
    }
    else if(r > 255)
    {
        Blue() = 255;
    }
    else
    {
        Blue() = static_cast<unsigned char>(r);
    }
    return *this;
}


rapid::BMP::~BMP()
{ clear(); }

void rapid::BMP::clear()
{
    if(__HeaderBlock == nullptr) return;
    const char *ch = reinterpret_cast<const char *>(__HeaderBlock);
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
    if(_DataContent != nullptr)
    {
        delete[] _DataContent;
    }
    file.seekg(0, std::ios::end);
    long long length = file.tellg();
    if(length < 54)
    {
        throw CannotParseFileException("CannotParseFileException: file size is to small!");
    }
    char *result = new char[static_cast<unsigned long long>(length)];
    file.read(result, length);
    __HeaderBlock = reinterpret_cast<HeaderBlock*>(&result[0]);
    __DescribeInfoBlock = reinterpret_cast<DescribeInfoBlock*>(&result[sizeof(HeaderBlock)]);
    if(__DescribeInfoBlock->DataSize <= 0)
    {
        throw CannotParseFileException("CannotParseFileException: there is no data!");
    }
    __ColorTable = reinterpret_cast<RGB*>(&result[sizeof(HeaderBlock) + sizeof(DescribeInfoBlock)]);
    _DataContent = &result[__data_begin_position()];
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
    if(m == nullptr || _DataContent == nullptr) return;
#ifdef cpp17
        [[maybe_unused]]
#endif
    unsigned char skip = 0;
    unsigned char pixel_byte = static_cast<unsigned char>(color_bit() / 8);
    // there is no color table
    if(reinterpret_cast<const char *>(_DataContent) == reinterpret_cast<const char *>(__ColorTable))
    {
        skip = (4 - (width() * pixel_byte) % 4) % 4;
        for(int i = width() - 1; i > 0; i--)
        {
            for(int j = 0; j < height(); j++)
            {
                if(pixel_byte == 4)
                {
                    char *ch = const_cast<char *>(&_DataContent[(width() - 1 - i) * pixel_byte + j * pixel_byte]);
                    m->set_value(i, j, *reinterpret_cast<RGB*>(ch));
                }
            }
        }
    }
    else
    {

    }
}
