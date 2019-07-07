#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "Core/Version.h"

class Exception
{
protected:
    char *__reason;
public:
    Exception(const char *r = nullptr) : __reason(const_cast<char *>(r)) { }
    virtual ~Exception() { }

    virtual const char* what()
    { return __reason; }
};

class IndexOutOfArrayException : public Exception
{
public:
    IndexOutOfArrayException(const char *r = nullptr) : Exception(const_cast<char *>(r))
    { }
};

#endif // EXCEPTION_H
