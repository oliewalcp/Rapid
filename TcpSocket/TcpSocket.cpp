#include "TcpSocket.h"
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <thread>
#include <queue>

/*设置协议族，作连接到准备
 * param[family]:协议族名称
 * param[type]:套接字类型
 * param[protocal]:
 */
void TcpSocket::open(int family, int type, int protocal)
{
    Family = family;
    Descriptor = socket(family, type, protocal);
    if(Descriptor < 0) throw CreateSocketException("创建连接失败");
}
/*发送消息
 * param[message]:需要发送到消息内容
 * param[target_ip]:发送消息到目标地址
 */
void TcpSocket::sendTo(std::string message, std::string target_ip)
{
    unsigned int messageLength = message.length();
    if(messageLength == 0) throw SendMessageException("");
    long long successSendSize = send(Descriptor, message.data(), messageLength, 0);
    if(successSendSize < 0 || messageLength != successSendSize) throw SendMessageException("");
}
/*连接远程服务器
 * param[remoteIP]:服务器IP地址
 * param[remotePort]:服务器端口号
 */
void TcpSocket::connectRemoteServer(std::string remoteIP, unsigned int remotePort)
{
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));//初始化结构体
    server_addr.sin_family = Family;
    server_addr.sin_port = htons(remotePort);//设置端口号
    inet_pton(Family, remoteIP.data(), &server_addr.sin_addr);//设置IP地址
    int log = connect(Descriptor, (sockaddr*)&server_addr, sizeof(server_addr));
    if(log != 0) throw ConnectException("连接远程服务器失败");
}
/*开启接收消息循环
 * param[f]:自定义消息处理函数
 * param[queueSize]:
 */
void TcpSocket::receiveLoop(function f, unsigned int listenPort, std::string listenIP, int queueSize)
{
    std::queue<int> freeThread;//记录空闲线程的下标
    std::thread *messageHandler[queueSize];
    for(int i = 0; i < queueSize; i++)
    {
        messageHandler[i] = new std::thread(f, "", "");
        freeThread.push(i);
    }
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = Family;
    server_addr.sin_port = htons(listenPort);
    int temp = 0;
    if(listenIP == "") temp = INADDR_ANY;
    else if(listenIP == "255.255.255.255") temp = INADDR_NONE;
    else if(listenIP == "127.0.0.1" || listenIP == "localhost") temp = INADDR_LOOPBACK;
    else temp = inet_addr(listenIP.data());
    server_addr.sin_addr.s_addr = htonl(temp);
    if(bind(Descriptor, (struct sockaddr*)&server_addr, sizeof(server_addr)) != 0)
        throw BindException("");
    if(listen(Descriptor, queueSize) != 0) throw BindException("");
    receive = true;
    //开始循环接收消息
    while(receive)
    {
        struct sockaddr_in client_addr;
        char cli_ip[INET_ADDRSTRLEN] = "";
        socklen_t cliaddr_len = sizeof(client_addr);
        //等待连接
        int connfd = accept(Descriptor, (struct sockaddr*)&client_addr, &cliaddr_len);
        //如果连接失败，则忽略
        if(connfd < 0) continue;
        inet_ntop(Family, &client_addr.sin_addr, cli_ip, INET_ADDRSTRLEN);
        char recv_buf[1024]{0};//1kB缓冲区
        std::string message = "";
        while(recv(connfd, recv_buf, sizeof(recv_buf), 0) > 0 ) // 接收数据
            message += recv_buf;
        while(freeThread.size() == 0);//等待空闲线程
	auto childThread = [&](){
		int index = freeThread.front();
		freeThread.pop();
		delete messageHandler[index];
		messageHandler[index] = new std::thread(f, cli_ip, message);
		messageHandler[index]->join();
		freeThread.push(index);
	};
	std::thread handler(childThread);
	handler.detach();
    }
    shutdown(Descriptor, SHUT_RDWR);
    delete[] messageHandler;
}

