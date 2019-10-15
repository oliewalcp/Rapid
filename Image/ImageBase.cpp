#include "ImageBase.h"

rapid::ImageBaseInterface::~ImageBaseInterface()
{ }

static inline void swap(char &a, char &b)
{
    char ch = a;
    a = b;
    b = ch;
}
void rapid::adjust_memory_order4(void *begin)
{
    char *c = static_cast<char *>(begin);
    swap(*c, *(c + 3));
    swap(*(c + 1), *(c + 2));
}

void rapid::adjust_memory_order2(void *begin)
{
    char *c = static_cast<char *>(begin);
    swap(*c, *(c + 1));
}
