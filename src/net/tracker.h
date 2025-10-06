#ifndef TRACKER_H_
#define TRACKER_H_

#include "basic_socket.h"
#include "../exceptions.h"
#include <string>
#include <iostream>
#include <charconv>

enum State {
    CONNECTING, SENDING, RECEIVING, DONE, ERROR
};

class Tracker : public BasicSocket {
private:
    std::string tracker_url, host;
    uint16_t port;
	
    void parse_port(std::string portStr);
    void parse_url();
public:
    Tracker(int domain, int type, int protocol, std::string tracker_url)
        : BasicSocket(domain, type, protocol, true), tracker_url(tracker_url) {
        this->parse_url();
    }

    State state;

    std::string get_host() { return this->host; }

	int connect();
};

#endif // TRACKER_H_
