#ifndef TORRENT_FILE_H
#define TORRENT_FILE_H
#include "bencode.h"

#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <string.h>
#include <openssl/sha.h>

class TorrentFile {
private:
    BencodeDict dict;

public:
    TorrentFile(const std::string& filename);
    ~TorrentFile();

    std::string calculateInfoHash();
    // start means http or udp, for now http is only implemented
    std::vector<std::string> getTrackers();
};

#endif
