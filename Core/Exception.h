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
    ~Exception() { }
    const char* what()
    { return _M_reason; }
};

#define RegistException(e) \
    class e : public Exception \
    { \
    public: \
        e(const char *r = nullptr) : Exception(r) { } \
    }

RegistException(IndexOutOfArrayException);
RegistException(SizeDoesNotMatchException);
RegistException(CannotParseFileException);

};


#endif // EXCEPTION_H
