#ifndef TORRENT_FILE_H
#define TORRENT_FILE_H

#include <fstream>
#include <string>
#include <iostream>

/**
 * BencodeElement, recursive element
 * Int
 * String(unsigned char *)
 * HashMap<unsigned char *,T>
 * List<T>
 */

class TorrentFile {
private:
    std::ifstream stream;

public:
    TorrentFile(const std::string& filename);
    ~TorrentFile();
};

#endif
