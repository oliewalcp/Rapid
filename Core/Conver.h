#ifndef CONVER_H
#define CONVER_H

namespace rapid
{

unsigned char hex_to_int(const char arg);
char int_to_hex(unsigned char arg);

/*
 * param[result]: __out__ 2 byte
 * param[code]: __in__ hex string, 4 byte
 */
void zone_bit_code_to_gb2312(char *result, const char *code);
/*
 * param[result]: __out__ hex string, 4 byte
 * param[code]: __in__ 2 byte
 */
void gb2312_to_zone_bit_code(unsigned char *result, const char *code);

#ifdef _WIN32
wchar_t *to_wchar(const char* str);
char* utf8_to_gb2312(const char* utf8);
#endif
};

#endif // CONVER_H
