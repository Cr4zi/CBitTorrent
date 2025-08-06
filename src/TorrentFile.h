#ifndef TORRENT_FILE_H
#define TORRENT_FILE_H
#include "Bencode.h"

#include <fstream>
#include <string>
#include <iostream>
#include <vector>

class TorrentFile {
private:
    BencodeDict dict;

public:
    TorrentFile(const std::string& filename);
    ~TorrentFile();
};

#endif
