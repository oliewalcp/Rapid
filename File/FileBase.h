#ifndef FILEBASE_H
#define FILEBASE_H

#include <fstream>

class FileBase
{
private:
    typedef unsigned int __uint32;
public:
    /* get all content of file
     * param[filename]: the name of file
     * return: char array of content
     */
    static std::string open(const char *filename)
    {
        std::ifstream file(filename);
        std::istreambuf_iterator<char> begin(file), end;
        return std::string(begin, end);
        /* read as std::string
        std::stringstream ss;
        ss << file.rdbuf();
        std::string content(ss.str());
        */
    }
    /* get all content of file
     * param[filename]: the name of file
     * param[length]: _out_ the byte size of file
     * return: char array of content, need to delete by yourself
     */
    static const char* open(const char *filename, __uint32 &length, const std::ios_base::openmode open_type = std::ios::binary)
    {
        std::ifstream file(filename, open_type);

        file.seekg(0, std::ios::end);    // go to the end
        length = file.tellg();           // report location (this is the length)
        file.seekg(0, std::ios::beg);    // go back to the beginning
        char *buffer = new char[length];    // allocate memory for a buffer of appropriate dimension
        file.read(buffer, length);       // read the whole file into the buffer
        file.close();
        return buffer;
    }
    /* save content to a file
     * param[filename]: the name of file
     * param[content]: will be wrote to a file
     * param[length]: the length of content
     * param[open_type]: open mode
     */
    static void save(const char *filename, const char* content, __uint32 length, const std::ios_base::openmode open_type = std::ios::binary)
    {
        std::ofstream file(filename, open_type);
        file.write(content, length);
        file.close();
    }
};

#endif // FILEBASE_H
