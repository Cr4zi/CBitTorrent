#ifndef PEER_H_
#define PEER_H_

#include <string>
#include <cstdint>

class Peer {
private:
    std::string peer_id, ip;
    uint16_t port;
public:
    Peer(std::string peer_id, std::string ip, uint16_t port)
        : peer_id(peer_id),
          ip(ip),
          port(port)
        {};

    Peer(std::string ip, uint16_t port)
        : peer_id(""),
        ip(ip),
        port(port)
        {};
};

#endif // PEER_H_
