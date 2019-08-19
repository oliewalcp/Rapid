#include "Exception.h"

using namespace rapid;

#define RegistExceptionImpl(e) \
    rapid::e::~e() { } \
    const char *e::what() \
    { return _M_reason; }

RegistExceptionImpl(Exception);
RegistExceptionImpl(IndexOutOfArrayException);
RegistExceptionImpl(SizeDoesNotMatchException);
RegistExceptionImpl(CannotParseFileException);
