#ifndef TRACKER_H_
#define TRACKER_H_

#include "basic_socket.h"
#include "../exceptions.h"
#include "../peer.h"
#include "../bencode.h"

#include <algorithm>
#include <string>
#include <iostream>
#include <charconv>
#include <vector>
#include <memory>
#include <sstream>
#include <map>
#include <optional>
#include <chrono>

enum State {
    CONNECTING, SENDING, RECEIVING, DONE, ERROR
};

class Tracker : public BasicSocket {
private:
    std::string tracker_url, host, tracker_id;
    uint16_t port;
    int64_t interval;

	std::chrono::steady_clock::time_point last_time_sent;

    std::vector<std::shared_ptr<Peer>> peers;

    template <typename T>
    std::optional<T> get_from_map(BencodeDict dict, std::string key);

    ssize_t parse_peers_list(BencodeList peers_dict);
    ssize_t parse_peers_binary(std::string& peers_binary);
    
    void parse_port(std::string portStr);
    void parse_url();

public:
    State state;

    Tracker(int domain, int type, int protocol, std::string tracker_url)
        : BasicSocket(domain, type, protocol, true), tracker_url(tracker_url) {
        this->parse_url();
    }

    ~Tracker() { close(this->sock_fd); }

    ssize_t generate_peers(std::vector<char>& response);

    std::string get_host() { return this->host; }
    std::vector<std::shared_ptr<Peer>> get_peers() { return this->peers; }

	void sendBytes(std::string &msg) override;
	
	int connect();

	bool can_send();
};

#endif // TRACKER_H_
