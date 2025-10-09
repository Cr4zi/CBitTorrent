#ifndef TRACKER_CLIENT_H_ 
#define TRACKER_CLIENT_H_ 

#include "basic_socket.h"
#include "tracker.h"
#include "../torrent_file.h"
#include "../globals.h"
#include "../exceptions.h"
#include "../peer.h"

#include <openssl/rand.h>
#include <openssl/err.h>

#include <iostream>
#include <unordered_map>
#include <sys/epoll.h>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <memory>

/**
 * 1. Sending request to trackers
 * 2. Recv trackers' responses
 * 3. Create a list of peers
 */

enum Event{
    STARTED,STOPPED,COMPLETED
};

class TrackerClient {
private:
    TorrentFile file;
    std::string peer_id;
    uint32_t key;

    // KINDOF HARDCODED
    void generate_peer_id();
    void generate_tracker_key_u32();
    std::string percent_encode(std::string& str) const;
    std::string prepare_request(std::string host, Event ev);
    
    bool is_everyone_done(std::vector<std::shared_ptr<Tracker>>& trackers);
public:
    TrackerClient(TorrentFile& file);

    void tracker_request(Event ev);

};

#endif // TRACKER_CLIENT_H_
