#include "Conver.h"

unsigned char rapid::hex_to_int(const char arg)
{
    if(arg >= 'a' && arg <= 'f')
    {
        return static_cast<unsigned char>(arg - 'a' + 10);//aH = 10D
    }
    else if(arg >= 'A' && arg <= 'F')
    {
        return static_cast<unsigned char>(arg - 'a' + 10);//AH = 10D
    }
    else if(arg >= '0' && arg <= '9')
    {
        return static_cast<unsigned char>(arg - '0');
    }
    return 0;
}

char rapid::int_to_hex(unsigned char arg)
{
    if(arg <= 9) return static_cast<char>(arg + '0');
    if(arg <= 15) return static_cast<char>(arg - 10 + 'A');
    return static_cast<char>(arg);
}

void rapid::zone_bit_code_to_gb2312(char *result, const char *code)
{
    result[0] = static_cast<char>(hex_to_int(code[0]) << 4);
    result[0] += hex_to_int(code[1]);

    result[1] = static_cast<char>(hex_to_int(code[2]) << 4);
    result[1] += hex_to_int(code[3]);
}

void rapid::gb2312_to_zone_bit_code(unsigned char *result, const char *code)
{
    result[0] = static_cast<unsigned char>((code[0] - '0') * 10 + code[1] - '0');
    result[0] += 0xA0;

    result[1] = static_cast<unsigned char>((code[2] - '0') * 10 + code[3] - '0');
    result[1] += 0xA0;
}

#ifdef __linux
#include <cstring>
rapid::size_type rapid::utf8_len(const char *ansi)
{
    return strlen(ansi);
}

wchar_t *rapid::to_utf8(const char *ansi)
{
    return ansi;
}

char *rapid::to_ansi(const char *utf8)
{
    return utf8;
}
#elif defined __WIN32
#include <stringapiset.h>
#include "Core/Memory.h"

rapid::size_type rapid::utf8_len(const char *ansi)
{
    if(ansi == nullptr)
        return 0;
    return static_cast<size_type>(MultiByteToWideChar(CP_UTF8, 0, ansi, -1, nullptr, 0));
}

wchar_t *rapid::to_utf8(const char *ansi)
{
    if(ansi == nullptr) return nullptr;
    size_type len = static_cast<size_type>(MultiByteToWideChar(CP_UTF8, 0, ansi, -1, nullptr, 0));
    wchar_t *wstr = new wchar_t[static_cast<unsigned long long>(len + 1)];
    mem_clear(wstr, static_cast<size_type>((static_cast<unsigned long long>(len) + 1) * sizeof(wchar_t)));
    MultiByteToWideChar(CP_UTF8, 0, ansi, -1, wstr, static_cast<int>(len));
    return wstr;
}

char *rapid::to_ansi(const char *utf8)
{
    wchar_t *wstr = to_utf8(utf8);
    unsigned long long len = static_cast<unsigned long long>(WideCharToMultiByte(CP_ACP, 0, wstr, -1, nullptr, 0, nullptr, nullptr));
    char *str = new char[len + 1];
    mem_clear(str, static_cast<size_type>(len + 1));
    WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, static_cast<int>(len), nullptr, nullptr);
    if(wstr) delete[] wstr;
    return str;
}

#endif
