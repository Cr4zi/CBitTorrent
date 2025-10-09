#include "basic_socket.h"
#include <cerrno>
#include <netdb.h>
#include <iostream>

BasicSocket::BasicSocket(int domain, int type, int protocol, bool non_blocking) {
    this->sock_fd = socket(domain, type, protocol);
    this->domain = domain;
    this->type = type;
    this->protocol = protocol;

    if(this->sock_fd < 0)
        throw SocketCreationException("Failed creating a basic socket.");

    if(non_blocking)
        this->set_nonblocking(this->sock_fd);

}

BasicSocket::~BasicSocket() {
    close(this->sock_fd);
}

int BasicSocket::getSockFd() {
    return this->sock_fd;
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
        throw SocketBindException("In bindSocket");
    }

    this->isBound = true;
}

void BasicSocket::listenSocket(int backlog) {
    if(!this->isBound) {
        throw SocketListenException("Socket is not binded");
    }

    if(listen(this->sock_fd, backlog) == -1) {
        throw SocketListenException("Listen");
    }
}

// when doing the UDP should add another parameter for type of socket
int BasicSocket::connectSocket(const char* host, uint16_t port) {
    struct addrinfo hints;
    struct addrinfo *result, *rp;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = this->domain;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    hints.ai_protocol = 0;

    int s = getaddrinfo(host, std::to_string(port).c_str(), &hints, &result);
    if(s != 0) {
        throw SocketConnectException("getaddrinfo");
    }

    int ret, sfd;
    for(rp = result; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        this->set_nonblocking(sfd);

        if(sfd == -1)
            continue;

        if((ret = connect(sfd, rp->ai_addr, rp->ai_addrlen)) != -1 || errno == EINPROGRESS)
            break;

        close(sfd);
    }

    freeaddrinfo(result);

    if(rp == NULL)
        throw SocketConnectException("Failed to find addr/connect");

    if(this->sock_fd != -1 && this->sock_fd != sfd)
        close(this->sock_fd);
    this->sock_fd = sfd;

    return ret;
}

void BasicSocket::sendBytes(std::string &msg) {
    if(send(this->sock_fd, msg.c_str(), msg.size(), 0) == -1) {
        if(errno == EAGAIN || errno == EWOULDBLOCK)
            throw SendingBytesFailedException("write EAGAIN or EWOULDBLOCK");
        else
            throw SendingBytesFailedException("write");
    }
}

std::vector<char> BasicSocket::readBytes(ssize_t &bytes_read) {
    if(bytes_read != 0)
        throw ReadingBytesFailedException("bytes read should be 0");

    ssize_t current_bytes_read = 0;

    const ssize_t CHUNK = 1024; // could be more or less, it just the amount of bytes each chunk will be...
    char buf[CHUNK];
    std::vector<char> data;

    while((current_bytes_read = read(this->sock_fd, buf, CHUNK)) > 0) {
        bytes_read += current_bytes_read;
        data.insert(data.end(), buf, buf + current_bytes_read);
    }

    if(current_bytes_read == -1) {
        if(errno == EAGAIN || errno == EWOULDBLOCK)
            throw ReadingBytesFailedException("read EAGAIN or EWOULDBLOCK");
        else
            throw ReadingBytesFailedException("read");
    }

    return data;
}
