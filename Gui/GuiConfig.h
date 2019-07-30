#ifndef GUICONFIG_H
#define GUICONFIG_H

#ifdef __WIN32
#include <windows.h>
#include <cstring>
#define WIN32_PLATFORM

using SizeType = std::size_t;
using WinSizeType = long;
using WinStringType = wchar_t;
#define to_win_code(tar, src, len) mbstowcs(tar, src, len)

#endif

#define INCLUDE(x) include "OpenGL/include/"##x

#endif // GUICONFIG_H
