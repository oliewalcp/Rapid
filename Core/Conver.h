#ifndef CONVER_H
#define CONVER_H

namespace rapid
{

unsigned char hexToInt(const char arg)
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

/*
 * param[result]: __out__ 2 byte
 * param[code]: __in__ hex string
 */
void CharToZoneBitCode(unsigned char *result, const char *code)
{
    result[0] = static_cast<unsigned char>(hexToInt(code[0]) << 4);
    result[0] += hexToInt(code[1]);

    result[1] = static_cast<unsigned char>(hexToInt(code[2]) << 4);
    result[1] += hexToInt(code[3]);
}


void ZoneBitCodeToChar(unsigned char *result, const char *code)
{
    result[0] = static_cast<unsigned char>((code[0] - '0') * 10 + code[1] - '0');
    result[0] += 0xA0;

    result[1] = static_cast<unsigned char>((code[2] - '0') * 10 + code[3] - '0');
    result[1] += 0xA0;
}

};

#endif // CONVER_H
