#include "basic_socket.h"

BasicSocket::BasicSocket(int domain, int type, int protocol, int port, uint32_t interface, int backlog, bool non_blocking) {
    this->sock_fd = socket(domain, protocol, port);

    if(this->sock_fd < 0) {
        throw SocketCreationException("Failed creating a basic socket.");
    }

    memset(&this->addr, 0, sizeof(this->addr));
    this->addr.sin_family = domain;
    this->addr.sin_port = htons(port);
    this->addr.sin_addr.s_addr = htonl(interface);

    if(bind(this->sock_fd, (struct sockaddr *)&this->addr, sizeof(struct sockaddr)) == -1)
        throw SocketCreationException("Failed to bind socket.");

    if(non_blocking)
        this->set_nonblocking();

    if(listen(this->sock_fd, backlog) == -1)
        throw SocketCreationException("Listen");

}


void BasicSocket::set_nonblocking() {
    int curr_flags = fcntl(this->sock_fd, F_GETFL, 0);
    if(curr_flags < 0)
        throw SocketCreationException("Failed to get socket file descriptor flags.");

    if(fcntl(this->sock_fd, F_SETFL, curr_flags | O_NONBLOCK))
        throw SocketCreationException("Failed to set socket as non blocking.");
}
