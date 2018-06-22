#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include "Socket.h"

class UdpSocket : public Socket
{
public:
    void open(int family = IP_V4, int type = SOCK_STREAM, int protocol = 0);//创建套接字
    void sendTo(std::string message, int port, std::string target_ip = "");//发送数据
    void receiveLoop(function f, unsigned int listenPort, std::string listenIP = "", int queueSize = 2);//接受数据
    void close();
};
#endif // UDPSOCKET_H
