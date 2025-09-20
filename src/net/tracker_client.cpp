#include "tracker_client.h"
#include <cctype>
#include <cerrno>
#include <cstdio>
#include <unordered_map>


void TrackerClient::generate_peer_id() {
    std::ostringstream oss;
    oss << "-CB0001-";
    unsigned char buf[12];
    int ret = RAND_bytes(buf, sizeof(buf));
    if(ret != 1) {
        std::cerr << "RAND_bytes" << std::endl;
        exit(-1);
    }

    char digits[] = "0123456789";
    size_t len = 10;
    for(int i = 0; i < 12; i++)
        oss << digits[buf[i] % len];

    this->peer_id = oss.str();
}


void TrackerClient::generate_tracker_key_u32() {
    this->key = 0;
    int ret = RAND_bytes((unsigned char *)&this->key, sizeof(this->key));
    if(ret != 1) {
        std::cerr << "RAND_bytes" << std::endl;
        exit(-1);
    }
}

std::string TrackerClient::percent_encode(std::string& str) const {
    std::ostringstream oss;
    oss << std::uppercase << std::hex;
    for(unsigned char c : str) {
        if(std::isalpha(c) || std::isdigit(c) || c == '.' || c == '-' || c == '_' || c == '~')
            oss << c;
        else
            oss << '%' << std::setw(2) << std::setfill('0') << static_cast<int>(c);
    }

    return oss.str();
}


std::string TrackerClient::prepare_request(std::string host, Event ev) {
    std::ostringstream stream;

    std::string hash = this->file.calculateInfoHash();

    std::string event;
    switch(ev) {
        case STARTED: {
            std::ostringstream oss;
            oss << "&event=started&key=" << std::hex << std::setw(8) << std::setfill('0') << this->key;
            event = oss.str();
            break;
        }
        case STOPPED:
            event = "&event=stopped";
            break;
        case COMPLETED:
            event = "&event=completed";
            break;
        default:
            event = "";
    }

    // for now uploaded, downloaded and left are 0
    stream << "GET /announce?info_hash=" << this->percent_encode(hash) << "&peer_id="
        << this->peer_id << "&port=" << Globals::PORT << "&uploaded=" << 0 << "&downloaded=" << 0 << "&left=" << 0
           << "&compact=1" << event << " HTTP/1.1\r\nHost: " << host << "\r\nUser-Agent: CBitTorrent/0.1\r\nConnection: close\r\n";

    return stream.str();
}

TrackerClient::TrackerClient(TorrentFile& file) : file(file) {
    this->generate_peer_id();
    this->generate_tracker_key_u32();
}

void TrackerClient::tracker_request(Event ev) {
    std::unordered_map<int, std::string> fd_to_hostname;

    // for now only http
    std::vector<std::string> trackers = file.getTrackers(std::string("http://"));
    std::cout << "--Trackers--" << std::endl;

    for(std::string tracker : trackers) {
        std::cout << tracker << std::endl;
    }

    int len = trackers.size() < Globals::MAX_CONNECTIONS ? trackers.size() : Globals::MAX_CONNECTIONS;

    int events_ready;
    struct epoll_event einf[len], events[len];
    std::vector<BasicSocket> sockets;

    int epoll_fd = epoll_create1(0);
    if(epoll_fd == -1) {
        throw new EpollException("epoll_create1");
    }

    // SOCK_STREAM because currently HTTP
    for(int i = 0; i < len; i++) {
        sockets.push_back(BasicSocket(AF_INET, SOCK_STREAM, 0, true));

        size_t colon = trackers[i].find(':');

        // invalid tracker
        if(std::string::npos == colon)
            continue;

        std::string hostname = trackers[i].substr(0, colon);
        int port;
        std::string portStr = trackers[i].substr(colon + 1); // it is actually port/announce so now we remove announce
        size_t slash = portStr.find('/');

        if(std::string::npos == slash)
            port = std::stoi(portStr.c_str());
        else
            port = std::stoi(portStr.substr(0, slash).c_str());

        if(sockets[i].connectSocket(hostname.c_str(), port) == 0) {
            sockets[i].sendBytes(this->prepare_request(hostname, ev).c_str());
        } else if(errno == EINPROGRESS) {
            einf[i].events = EPOLLOUT | EPOLLIN;
            einf[i].data.fd = sockets[i].getSockFd();

            if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sockets[i].getSockFd(), &einf[i]) == -1) {
                std::cerr << "Couldn't add epoll" << std::endl; // I don't think we should end the program if this happens
            }
        } else {
            std::cerr << "Failed to connect to " << hostname << ":" << port << std::endl;
        }

        fd_to_hostname.insert({sockets[i].getSockFd(), hostname});
    }

    while(true) {
        events_ready = epoll_wait(epoll_fd, events, Globals::MAX_CONNECTIONS, 3000);

        for(int i = 0; i < events_ready; i++) {
            BasicSocket* sock = nullptr;
            for(size_t j = 0; j < sockets.size(); j++) {
                if(sockets[j].getSockFd() == events[i].data.fd) {
                    sock = &sockets[j];
                }
            }

            if(events[i].events & EPOLLOUT) {
                std::cout << "Sending: " << events[i].data.fd << std::endl;
                sock->sendBytes(this->prepare_request(fd_to_hostname[events[i].data.fd], ev).c_str());
            } else if(events[i].events & EPOLLOUT) {
                char *buf;
                sock->readBytes(buf);
                std::cout << "Recv: " << buf << "from" << fd_to_hostname[events[i].data.fd] << std::endl;
            }
        }
    }
}
