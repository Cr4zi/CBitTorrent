#ifndef BASIC_SOCKET_H_
#define BASIC_SOCKET_H_

#include "../exceptions.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>

class BasicSocket {
private:
    int sock_fd;
    struct sockaddr_in addr;

    void set_nonblocking();
public:
    BasicSocket(int domain, int type, int protocol, int port, uint32_t interface, int backlog, bool non_blocking = true);
};


#endif // BASIC_SOCKET_H_
