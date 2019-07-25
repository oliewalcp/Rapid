#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "Version.h"
#include "TypeTraits.h"

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

class SizeDoesNotMatchException : public Exception
{
public:
    SizeDoesNotMatchException(const char *r = nullptr) : Exception(r) { }
    virtual ~SizeDoesNotMatchException() { }
    virtual const char* what();
};

class CannotParseFileException : public Exception
{
public:
    CannotParseFileException(const char *r = nullptr) : Exception(r) { }
    virtual ~CannotParseFileException() { }
    virtual const char* what();
};

};


#endif // EXCEPTION_H
