#ifndef BASIC_SOCKET_H_
#define BASIC_SOCKET_H_

#include "../exceptions.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <arpa/inet.h>

class BasicSocket {
private:
    int domain, type, protocol;

    struct sockaddr_in addr;

    bool isBound = false;
protected:
    int sock_fd;
public:
    BasicSocket(int domain, int type, int protocol, bool non_blocking = true);
    ~BasicSocket();

    int getSockFd();

    static void set_nonblocking(int sock_fd);

    void bindSocket(int port);
    void listenSocket(int backlog);

    int connectSocket(const char *host, uint16_t port);

    void sendBytes(std::string &msg);

    // when calling this function bytesRead should be 0
    char *readBytes(ssize_t &bytes_read);

};

#endif // BASIC_SOCKET_H_
