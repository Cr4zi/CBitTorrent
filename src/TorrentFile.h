#ifndef TORRENT_FILE_H
#define TORRENT_FILE_H
#include "Bencode.h"

#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <openssl/sha.h>

class TorrentFile {
private:
    BencodeDict dict;

public:
    TorrentFile(const std::string& filename);
    ~TorrentFile();

    std::string calculateInfoHash();
};

#endif
