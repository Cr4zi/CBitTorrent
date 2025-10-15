#ifndef PEER_H_
#define PEER_H_

#include "basic_socket.h"
#include <string>
#include <cstdint>
#include <sstream>
#include <arpa/inet.h>
#include <memory>
#include <iostream>

enum MessageType {
	CHOKE,
	UNCHOKE,
	INTERESTED,
	NOT_INTERESTED,
	HAVE,
	BITFIELD,
	REQUEST,
	PIECE,
	CANCEL,
	PORT,
	KEEP_ALIVE,
	HANDSHAKE
};

typedef struct{
    uint32_t len;
    MessageType id;
    std::vector<char> payload;
} Message;

class Peer {
private:
	
    std::string peer_id, ip;
    uint16_t port;

	bool choked, interested;

	BasicSocket socket;

	void send(std::ostringstream& oss);

public:
    Peer(std::string peer_id, std::string ip, uint16_t port)
        : peer_id(peer_id),
          ip(ip),
          port(port),
		  choked(true),
		  interested(false),
		  socket(AF_INET, SOCK_STREAM, 0, true)
        {
			socket.connectSocket(ip.c_str(), port);
		};

    Peer(std::string ip, uint16_t port)
        : peer_id(""),
		  ip(ip),
		  port(port),
		  choked(true),
		  interested(false),
		  socket(AF_INET, SOCK_STREAM, 0, true)
        {
			socket.connectSocket(ip.c_str(), port);
		};

    void setChoked(bool choked) { this->choked = choked; }

	void handshake(std::string info_hash, std::string my_peer_id);
    void keep_alive();
	ssize_t sendMsg(MessageType type, const std::vector<char>& payload);
    std::shared_ptr<Message> read();
};

#endif // PEER_H_
