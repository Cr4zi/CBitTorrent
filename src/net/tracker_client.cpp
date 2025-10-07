#include "tracker_client.h"
#include "tracker.h"
#include <cerrno>
#include <cstdio>
#include <sys/socket.h>


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
           << "&compact=1" << event << " HTTP/1.1\r\nHost: " << host << "\r\nUser-Agent: CBitTorrent/0.1\r\nConnection: close\r\n\r\n";

    return stream.str();
}

TrackerClient::TrackerClient(TorrentFile& file) : file(file) {
    this->generate_peer_id();
    this->generate_tracker_key_u32();
}

void TrackerClient::tracker_request(Event ev) {
    // for now only http
    std::vector<std::string> trackers = file.getTrackers();
    // std::cout << "--Trackers--" << std::endl;

    /*
    for(std::string tracker : trackers) {
        std::cout << tracker << std::endl;
    }
    */

    int len = trackers.size() < Globals::MAX_CONNECTIONS ? trackers.size() : Globals::MAX_CONNECTIONS;

    int events_ready;
    struct epoll_event events[len];
    std::vector<std::shared_ptr<Tracker>> sockets;

    int epoll_fd = epoll_create1(0);
    if(epoll_fd == -1) {
        throw new EpollException("epoll_create1");
    }

    int sockets_index = 0;
    int good_trackers = 0;
    // SOCK_STREAM because currently HTTP
    for(int i = 0; i < len; i++) {
        try {
            std::shared_ptr<Tracker> tracker = std::make_shared<Tracker>(AF_INET, SOCK_STREAM, 0, trackers[i]);

			bool success; // assuming that we connect successfully

			try {
				if(tracker->connect() == 0)
					success = true;
				else if(errno == EINPROGRESS)
					success = false;
				else {
					sockets.pop_back();
					continue;
				}
				std::cout << "Finished connecting " << success << std::endl;
			} catch(SocketConnectException* e) {
				std::cerr << "Couldn't connect: " << e->what() << std::endl;
				sockets.pop_back();
				continue;
			}
			struct epoll_event ev;
			ev.events = EPOLLIN | EPOLLOUT;
			ev.data.fd = tracker->getSockFd();

			if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, tracker->getSockFd(), &ev) == -1)
				tracker->state = ERROR;
			else {
				tracker->state = success ? SENDING : CONNECTING;
				good_trackers++;
			}
			sockets.emplace_back(tracker);

			sockets_index++;
        } catch (InvalidTrackerException* e) {
            // std::cerr << "Couldn't create a tracker" << e->what() << std::endl;
            continue;
        }
    }
    if(sockets.size() <= 0) {
        std::cerr << "Couldn't connect to trackers..." << std::endl;
        return;
    }

	for(size_t i = 0; i < sockets.size(); i++) {
		std::cout << "fd: " << sockets[i]->getSockFd() << " host: " << sockets[i]->get_host() << " state: "  << sockets[i]->state << std::endl;
	}

    while(true) {
        events_ready = epoll_wait(epoll_fd, events, sockets.size(), 0);
        // std::cout << "Events ready: " << events_ready << std::endl;

        for(int i = 0; i < events_ready; i++) {
            int fd = events[i].data.fd;
            const auto it = std::find_if(sockets.begin(), sockets.end(), [fd](std::shared_ptr<Tracker> tracker) {
                return tracker->getSockFd() == fd;
            });

            if(it == sockets.end())
                continue;

            std::shared_ptr<Tracker> tracker = *it;

            if(tracker->state == CONNECTING && events[i].events & EPOLLOUT) {
                int err{};
                socklen_t len = sizeof(err);
                getsockopt(fd, SOL_SOCKET, SO_ERROR, &err, &len);
                if(err == 0) {
					
					std::cout << "Connected successfully " << fd << std::endl;
                    tracker->state = SENDING;
				} else {
                    tracker->state = ERROR;
                    std::cerr << "Failed to connect to: " << tracker->get_host() << std::endl;
                }
            }

			if(tracker->state == SENDING && events[i].events & EPOLLOUT) {
                std::cout << "Sending: " << fd << " To: " << tracker->get_host() << std::endl;
				try {
					std::string request = this->prepare_request(tracker->get_host(), STARTED);
					tracker->sendBytes(request);
					tracker->state = RECEIVING;
				} catch(SendingBytesFailedException* e) {
					std::cout << e->what() << std::endl;
				}
            } else if(tracker->state == RECEIVING && events[i].events & EPOLLIN) {
				std::cout << "Reading: " << fd << " From: " << tracker->get_host() << std::endl;
				ssize_t bytes_read = 0;
				try {
					std::string data;
					char *buf = tracker->readBytes(bytes_read);
					tracker->state = DONE;
					data.assign(buf, bytes_read);
					std::cout << "Read Bytes: "<< bytes_read << std::endl;
					free(buf);
					
				} catch(ReadingBytesFailedException* e) {
					std::cerr << e->what() << "errno: " << errno << std::endl;
				}
            }
        }
    }
}
