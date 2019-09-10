#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "Core/Version.h"
#include "Core/TypeTraits.h"

namespace rapid
{
class Exception
{
protected:
    char *_M_reason;
public:
    Exception(const char *r = nullptr) : _M_reason(remove_const(r)) { }
    virtual ~Exception();
    virtual const char* what();
};

#define RegistException(e) \
    class e : public Exception \
    { \
    public: \
        e(const char *r = nullptr) : Exception(r) { } \
        virtual ~e(); \
        virtual const char* what(); \
    }

RegistException(IndexOutOfArrayException);
RegistException(SizeDoesNotMatchException);
RegistException(CannotParseFileException);

};


#endif // EXCEPTION_H
