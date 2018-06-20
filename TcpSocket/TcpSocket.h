#ifndef CONNECTTOOLS_H
#define CONNECTTOOLS_H

#include <sys/socket.h>
#include <sys/types.h>
#include <string>
#include <atomic>

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
typedef SocketException ConnectException;
typedef SocketException BindException;
/* 自定义消息处理函数
 * param[source_ip]:消息发送方ip地址
 * param[message]:接收到的消息内容
 */
typedef void(*function)(std::string source_ip, std::string message);

class TcpSocket
{
    private:
        int Descriptor;//套接字描述符
        int Family;//协议族
        std::atomic<bool> receive;
    public:
        void open(int family = AF_INET, int type = SOCK_STREAM, int protocol = 0);//创建套接字
        void sendTo(std::string message, std::string target_ip);//发送数据
        void receiveLoop(function f, unsigned int listenPort, std::string listenIP = "", int queueSize = 2);//接受数据
        void connectRemoteServer(std::string remoteIP, unsigned int remotePort);//连接远程服务器
        void stopReceiveLoop() { receive = false; }
};

#endif // CONNECTTOOLS_H

