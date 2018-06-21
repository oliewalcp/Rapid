#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include "Socket.h"

typedef SocketException ConnectException;

class TcpSocket : public Socket
{
private:
    bool AlreadyConnectRemote;
public:
    void open(int family = IP_V4, int type = SOCK_STREAM, int protocol = 0);//创建套接字
    void sendTo(std::string message, std::string target_ip = "");//发送数据
    void receiveLoop(function f, unsigned int listenPort, std::string listenIP = "", int queueSize = 2);//接受数据
    virtual void connectRemoteServer(std::string remoteIP, unsigned int remotePort);//连接远程服务器
    void close();
};
#endif // TCPSOCKET_H

