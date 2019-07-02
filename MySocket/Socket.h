#ifndef SOCKETEXCEPTION_H
#define SOCKETEXCEPTION_H

#include <sys/socket.h>
#include <sys/types.h>
#include <string>
#include <atomic>
#include <netinet/in.h>

#define IP_V4 AF_INET
#define IP_V6 AF_INET6

/* 自定义消息处理函数
 * param[source_ip]:消息发送方ip地址
 * param[message]:接收到的消息内容
 */
typedef void(*function)(std::string source_ip, std::string message);

class SocketException : public std::exception
{
    public:
        SocketException(std::string message="") : errorMessage(message){}
        std::string getErrorMessage(){ return errorMessage;}
    private:
        std::string errorMessage;//异常信息
};

typedef SocketException CreateSocketException;
typedef SocketException SendMessageException;
typedef SocketException BindException;

class Socket
{
protected:
    int Descriptor;//套接字描述符
    int Family;//协议族
    std::atomic<bool> receive;
public:
    virtual void open(int family = IP_V4, int type = SOCK_STREAM, int protocol = 0) = 0;//创建套接字
    virtual void sendTo(std::string message, std::string target_ip = "") = 0;//发送数据
    virtual void receiveLoop(function f, unsigned int listenPort, std::string listenIP = "", int queueSize = 2) = 0;//接受数据
    virtual void stopReceiveLoop() { receive = false; }
    virtual void close() = 0;
    virtual ~Socket(){}
};
#endif // SOCKETEXCEPTION_H
