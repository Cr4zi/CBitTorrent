#include <iostream>
#include <sys/socket.h>
#include "torrent_file.h"
#include "net/tracker_client.h"

int main() {
    TorrentFile torrent("test.torrent");

    TrackerClient tracker_client(torrent);
    // std::cout << tracker_client.prepare_request(STARTED) << std::endl;
    tracker_client.tracker_request(STARTED);

    return 0;
}
