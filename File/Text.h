#ifndef TEXT_H
#define TEXT_H
#include "FileBase.h"
#include "Math/MathBase.h"

struct Text
{
public:
    static void save(const char* filename, const char *content, __uint32 length)
    {
        FileBase::save(filename, content, length, std::ios_base::in);
    }

    static void save(const char* filename, const std::string & o)
    {
        FileBase::save(filename, o.data(), o.length(), std::ios_base::in);
    }
};

#endif // TEXT_H
