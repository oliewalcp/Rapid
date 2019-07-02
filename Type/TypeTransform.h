#ifndef TYPETRANSFORM_H
#define TYPETRANSFORM_H

#ifdef (__linux__)
#define to_wchar_pointer
#elif (_WIN32)
#include <windows.h>
#define to_wchar_pointer MultiByteToWideChar
#endif

#endif // TYPETRANSFORM_H
