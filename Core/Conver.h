#ifndef CONVER_H
#define CONVER_H

namespace rapid
{

#include <QString>
#include <QTextCodec>

#include <string>
#include <iostream>

QString getWord(QString &arg)
{
    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    QStringList s;
    for(int i = 0; i < arg.length(); i += 4)
    {
        s.append(arg.mid(i,4));
    }
    QString temp;
    for(const QString &t : s)
    {
        temp.append(t.toUShort(nullptr, 16));
    }
    QString result = codec->fromUnicode(temp);
    return result;
}


const char * utf8ToGb2312(const char *strUtf8)
{
    QTextCodec* utf8Codec = QTextCodec::codecForName("utf-8");
    QTextCodec* gb2312Codec = QTextCodec::codecForName("gb2312");

    QString strUnicode = utf8Codec ->toUnicode(strUtf8);
    QByteArray ByteGb2312 = gb2312Codec ->fromUnicode(strUnicode);
    return ByteGb2312.data();
}

//将16进制字符转换成10进制数字
unsigned char hexToInt(const char arg)
{
    if(arg >= 'a' && arg <= 'f')
    {
        return static_cast<unsigned char>(arg - 'a' + 10);//16进制的a表示10
    }
    else if(arg >= 'A' && arg <= 'F')
    {
        return static_cast<unsigned char>(arg - 'a' + 10);//16进制的A表示10
    }
    else if(arg >= '0' && arg <= '9')
    {
        return static_cast<unsigned char>(arg - '0');
    }
}

/* 将区位码字符串（16进制字符串）转换为区位码
 * param[result]:2个元素的数组，用于保存结果
 * param[code]:16进制字符串
 */
void CharToZoneBitCode(unsigned char *result, const char *code)
{
    //先转换高两位的区码
    result[0] = static_cast<unsigned char>(hexToInt(code[0]) << 4);
    result[0] += hexToInt(code[1]);
    //转换低两位的位码
    result[1] = static_cast<unsigned char>(hexToInt(code[2]) << 4);
    result[1] += hexToInt(code[3]);
}

QString gb2312ToUtf8(const char *strGB2312)
{
    QTextCodec* gb2312Codec = QTextCodec::codecForName("gb2312");

    QString strUtf8 = gb2312Codec->toUnicode(strGB2312);
    return strUtf8;
}

int test_main()
{
    std::string input;
    std::cin >> input;
    unsigned char *ch = new unsigned char[3]{0};
    CharToZoneBitCode(ch, input.data());
    QString str = gb2312ToUtf8((const char *)ch);
    return 0;
}

void ZoneBitCodeToChar(unsigned char *result, const char *code)
{
    //先转换高两位区位码
    result[0] = static_cast<unsigned char>((code[0] - '0') * 10 + code[1] - '0');
    result[0] += 0xA0;
    //转换低两位区位码
    result[1] = static_cast<unsigned char>((code[2] - '0') * 10 + code[3] - '0');
    result[1] += 0xA0;
}

};

#endif // CONVER_H
