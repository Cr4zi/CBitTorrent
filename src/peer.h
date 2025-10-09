#ifndef PEER_H_
#define PEER_H_

#include <string>

class Peer {
private:
    std::string peer_id, ip;
    int port;
public:
    Peer(std::string peer_id, std::string ip, int port)
        : peer_id(peer_id),
          ip(ip),
          port(port)
        {};

    Peer(std::string ip, int port)
        : peer_id(""),
        ip(ip),
        port(port)
        {};
};

#endif // PEER_H_
