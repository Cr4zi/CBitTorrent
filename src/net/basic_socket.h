#ifndef BASIC_SOCKET_H_
#define BASIC_SOCKET_H_

#include "../exceptions.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <arpa/inet.h>

class BasicSocket {
private:
    int sock_fd;
    struct sockaddr_in addr;
    int domain;

    bool isBound = false;
public:
    BasicSocket(int domain, int type, int protocol, bool non_blocking = true);
    static void set_nonblocking(int sock_fd);

    void bindSocket(int port);
    void listenSocket(int backlog);

    void connectSocket(const char *host, int port);

    // void readBytes();
};

#endif // BASIC_SOCKET_H_
