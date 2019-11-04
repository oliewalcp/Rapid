#ifndef SOCKET_H
#define SOCKET_H

#ifdef __linux
#define socket_type int
#else
#define socket_type SOCKET
#endif

#ifdef __linux
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>

typedef int socket_t;

bool env_init() { return true; }
bool env_clean() { return true; }

#elif defined __WIN32
#include <winsock2.h>
typedef SOCKET socket_t;

//define struct
//#define sockaddr
//#define sockaddr_in

//define function
//#define close closesocket;
//#define send
//#define recv
//#define setsockopt
//#define socket
//#define accept
//#define connect
//#define htons


bool env_init()
{
    WORD version_requested = MAKEWORD(1, 1);
    WSADATA wsa_data;
    if(WSAStartup(version_requested, &wsa_data) != 0)
    {
        return false;
    }
    if(LOBYTE(wsa_data.wVersion) != 1 || HIBYTE(wsa_data.wVersion) != 1)
    {
        WSACleanup();
        return false;
    }
    return true;
}
bool env_clean()
{
    return WSACleanup();
}

#endif


#endif // SOCKET_H
