#ifndef FILEBASE_H
#define FILEBASE_H

#include <fstream>

class FileBaes
{
private:
    typedef unsigned long long __uint64;
public:
    static std::string open(const char *filename)
    {
        std::ifstream file(filename);
        std::istreambuf_iterator<char> begin(file);
        std::istreambuf_iterator<char> end;
        std::string content(begin, end);
        file.close();
        return content;
    }
    static void save(const char *filename, const char* content, __uint64 length)
    {
        std::ofstream file(filename);
        file.write(content, length);
        file.close();
    }
};

#endif // FILEBASE_H
