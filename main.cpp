#include <locale>
#include <string>
#include <codecvt>
#include <iostream>

int main()
{
//    setlocale(LC_ALL, "gb2312");

//    SizeType i;

//    UnicodeType* result = get_utf8_str(i, "测试");
//    std::wcout << result << std::endl;
//    std::string str("呵呵呵");
//    std::wstring_convert<std::codecvt_utf8<wchar_t>> convert;
//    std::wstring result = convert.from_bytes(str);
//    std::wcout << result << std::endl;
    std::cout << setlocale(LC_ALL, "chs") << std::endl;
    std::cout << "dfasgfhfaaaaaaaaaa" << std::endl;
    std::cout << "呵呵呵" << std::endl;
    std::cout << "dfasdfdgafga" << std::endl;
    return 0;
}
