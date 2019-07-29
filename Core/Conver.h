#ifndef CONVER_H
#define CONVER_H

namespace rapid
{
// 'A' --> 10, 'F' ---> 15
unsigned char hex_to_int(const char arg)
{
    if(arg >= 'a' && arg <= 'f')
    {
        return static_cast<unsigned char>(arg - 'a' + 10);//aH = 10D
    }
    else if(arg >= 'A' && arg <= 'F')
    {
        return static_cast<unsigned char>(arg - 'A' + 10);//AH = 10D
    }
    else if(arg >= '0' && arg <= '9')
    {
        return static_cast<unsigned char>(arg - '0');
    }
    return 0;
}

char int_to_hex(unsigned char arg)
{
    if(arg <= 9)
    {
        return static_cast<char>(arg + '0');
    }
    else if(arg <= 15)
    {
        return static_cast<char>(arg - 10 + 'A');
    }
    return static_cast<char>(arg);
}

/*
 * param[result]: __out__ 2 byte
 * param[code]: __in__ hex string, 4 byte
 */
void zone_bit_code_to_Chinese(unsigned char *result, const char *code)
{
    result[0] = static_cast<unsigned char>(hex_to_int(code[0]) << 4);
    result[0] += hex_to_int(code[1]);

    result[1] = static_cast<unsigned char>(hex_to_int(code[2]) << 4);
    result[1] += hex_to_int(code[3]);
}

/*
 * param[result]: __out__ 4 byte
 * param[code]: __in__ Chinese string, 2 byte
 */
void Chinese_to_zone_bit_code(char *code, const char *str)
{
    code[0] = int_to_hex(static_cast<unsigned char>(str[0] & 0x0F));
    code[1] = int_to_hex(static_cast<unsigned char>(str[0] >> 4));
    code[2] = int_to_hex(static_cast<unsigned char>(str[1] & 0x0F));
    code[3] = int_to_hex(static_cast<unsigned char>(str[1] >> 4));
//    code[0] = static_cast<unsigned char>((str[0] - '0') * 10 + str[1] - '0');
//    code[0] += 0xA0;

//    code[1] = static_cast<unsigned char>((str[2] - '0') * 10 + str[3] - '0');
//    code[1] += 0xA0;
}

/*
 * param[result]:__out__ 4 byte
 * param[str]:__in__ 2 byte
 */
void char_to_gb2312(char *result, const char *str)
{

}

};

#endif // CONVER_H
