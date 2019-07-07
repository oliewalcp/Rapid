#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "Core/Version.h"
#include "Core/TypeTraits.h"

namespace rapid
{
class Exception
{
protected:
    char *_reason;
public:
    Exception(const char *r = nullptr) : _reason(remove_const(r)) { }
    virtual ~Exception() { }

    virtual const char* what();
};

class IndexOutOfArrayException : public Exception
{
public:
    IndexOutOfArrayException(const char *r = nullptr) : Exception(r) { }
    virtual ~IndexOutOfArrayException() { }
    virtual const char* what();
};

const char *Exception::what()
{ return _reason; }

const char *IndexOutOfArrayException::what()
{ return _reason; }

};


#endif // EXCEPTION_H
