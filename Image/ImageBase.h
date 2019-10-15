#ifndef IMAGEBASE_H
#define IMAGEBASE_H

#include "Core/Version.h"
#include "Core/TypeTraits.h"

namespace rapid
{

extern inline void adjust_memory_order4(void *begin);
extern inline void adjust_memory_order2(void *begin);

template<typename _Tp>
class Matrix;

struct RGBA;

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
    virtual Matrix<RGBA> to_matrix() = 0;
    virtual void parse(const Matrix<RGBA>&) = 0;
    virtual void write(const char *filename) = 0;
};


}

#endif // IMAGEBASE_H
