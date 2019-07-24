#include "Exception.h"

using namespace rapid;

const char *Exception::what()
{ return _reason; }

const char *IndexOutOfArrayException::what()
{ return _reason; }

const char *SizeDoesNotMatchException::what()
{ return _reason; }
