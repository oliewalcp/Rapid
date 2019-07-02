#include "UdpSocket.h"
#include <string.h>
#include <arpa/inet.h>
#include <thread>

/*设置协议族，作连接到准备
 * param[family]:协议族名称
 * param[type]:套接字类型
 * param[protocol]:
 */
void UdpSocket::open(int family, int type, int protocol)
{
    Family = family;
    Descriptor = socket(family, type, protocol);
    if(Descriptor < 0) throw CreateSocketException("创建连接失败");
}
/*发送消息
 * param[message]:需要发送到消息内容
 * param[target_ip]:发送消息到目标地址
 */
void UdpSocket::sendTo(std::string message, int port, std::string target_ip)
{
    unsigned int messageLength = message.length();
    if(messageLength == 0) throw SendMessageException("");
    struct sockaddr_in opposite_addr;
    bzero(&opposite_addr, sizeof(opposite_addr));//初始化结构体
    opposite_addr.sin_family = Family;
    opposite_addr.sin_port = htons(port);//设置端口号
    opposite_addr.sin_addr.s_addr = inet_addr(target_ip.data());
    long long successSendSize = sendto(Descriptor, message.data(), messageLength, 0, (struct sockaddr*)&opposite_addr, sizeof(opposite_addr));
    if(successSendSize < 0 || messageLength != successSendSize) throw SendMessageException("");
}
/*开启接收消息循环
 * param[f]:自定义消息处理函数
 * param[queueSize]:
 */
void UdpSocket::receiveLoop(function f, unsigned int listenPort, std::string listenIP, int queueSize)
{
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = Family;
    server_addr.sin_port = htons(listenPort);
    int temp = 0;
    std::atomic<int> freeSize;//空闲的线程数
    freeSize = queueSize;
    if(listenIP == "") temp = INADDR_ANY;
    else if(listenIP == "255.255.255.255") temp = INADDR_NONE;
    else if(listenIP == "127.0.0.1" || listenIP == "localhost") temp = INADDR_LOOPBACK;
    else temp = inet_addr(listenIP.data());
    server_addr.sin_addr.s_addr = htonl(temp);
    if(bind(Descriptor, (struct sockaddr*)&server_addr, sizeof(server_addr)) != 0)
        throw BindException("");
    receive = true;
    //开始循环接收消息
    while(receive)
    {
        struct sockaddr_in client_addr;
        bzero(&client_addr, sizeof(client_addr));
        socklen_t addrlen = sizeof(client_addr);
        char cli_ip[INET_ADDRSTRLEN]{0};
        char recv_buf[1024]{0};//1kB缓冲区
        // 接收数据
        if(recvfrom(Descriptor, recv_buf, sizeof(recv_buf), 0, (struct sockaddr*)&client_addr, &addrlen) <= 0)
            continue;
        while(freeSize <= 0);//等待空闲线程
        inet_ntop(Family, &client_addr.sin_addr, cli_ip, INET_ADDRSTRLEN);//
        freeSize = freeSize - 1;
        std::thread handler([&](){
            f(cli_ip, recv_buf);
            freeSize = freeSize + 1;
        });
        handler.detach();
    }
}

void UdpSocket::close()
{
    shutdown(Descriptor, SHUT_RDWR);
}
