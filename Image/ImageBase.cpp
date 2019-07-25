#include "ImageBase.h"

rapid::RGB rapid::RGB::operator*=(double arg)
{
    Red() *= arg;
    Green() *= arg;
    Blue() *= arg;
    return *this;
}

rapid::RGB rapid::RGB::operator+=(double arg)
{
    Red() += arg;
    Green() += arg;
    Blue() += arg;
    return *this;
}
