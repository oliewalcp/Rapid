#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "Core/Version.h"

class Exception
{
private:
    char *__reason;
public:
    Exception() : __reason(nullptr) { }
    Exception(const char *r) : __reason(r) { }

    virtual const char* what()
    { return __reason; }
};

#endif // EXCEPTION_H
