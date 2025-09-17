#include "basic_socket.h"
#include <sys/socket.h>

BasicSocket::BasicSocket(int domain, int type, int protocol, bool non_blocking) {
    this->sock_fd = socket(domain, type, protocol);
    this->domain = domain;

    if(this->sock_fd < 0)
        throw SocketCreationException("Failed creating a basic socket.");

    if(non_blocking)
        this->set_nonblocking(this->sock_fd);

}

void BasicSocket::set_nonblocking(int sock_fd) {
    int curr_flags = fcntl(sock_fd, F_GETFL, 0);
    if(curr_flags < 0)
        throw SocketCreationException("Failed to get socket file descriptor flags.");

    if(fcntl(sock_fd, F_SETFL, curr_flags | O_NONBLOCK))
        throw SocketCreationException("Failed to set socket as non blocking.");
}

void BasicSocket::bindSocket(int port) {
    memset(&this->addr, 0, sizeof(this->addr));
    this->addr.sin_family = this->domain;
    this->addr.sin_addr.s_addr = INADDR_ANY;
    this->addr.sin_port = htons(port);

    if(bind(this->sock_fd, (struct sockaddr *)&this->addr, sizeof(this->addr)) == -1) {
        throw new SocketBindException("In bindSocket");
    }

    this->isBound = true;
}

void BasicSocket::listenSocket(int backlog) {
    if(!this->isBound) {
        throw new SocketListenException("Socket is not binded");
    }

    if(listen(this->sock_fd, backlog) == -1) {
        throw new SocketListenException("Listen");
    }
}

void BasicSocket::connectSocket(const char* host, int port) {
    memset(&this->addr, 0, sizeof(this->addr));
    this->addr.sin_family = this->domain;
    this->addr.sin_port = htons(port);

    if(inet_pton(this->domain, host, &this->addr.sin_addr) <= 0) {
        throw new SocketConnectException("inet_pton");
    }

    if(connect(this->sock_fd, (struct sockaddr *)&this->addr, sizeof(this->addr)) == -1) {
        throw new SocketConnectException("connect");
    }

}

/* THIS IS AN EXAMPLE OF READING 14 bytes to use later...
void BasicSocket::readBytes(){
    if(!this->isBound)
        return;

    while(1) {
        int conn_sock;
        socklen_t addrlen = sizeof(this->addr);
        if((conn_sock = accept(this->sock_fd, (struct sockaddr *)&this->addr, &addrlen)) == -1) {
            perror("Shit");
            return;
        }

        char buf[14];
        int bytes_read = read(conn_sock, buf, 14);
        printf("%s\n", buf);
    }
}
*/
