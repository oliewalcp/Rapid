#ifndef EXCEPTION_H
#define EXCEPTION_H
#include <exception>
#include <string>

class Exception : std::exception
{
private:
    std::string __msg;
public:
    Exception(std::string msg = "") : __msg(msg) {}
    ~Exception() {}
    virtual const char* what() { return __msg.data(); }
};

#endif // EXCEPTION_H
