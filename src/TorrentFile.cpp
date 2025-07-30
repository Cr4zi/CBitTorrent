#include "TorrentFile.h"

TorrentFile::TorrentFile(const std::string& filename) {
    // std::ios_base::binary open file in binary mode
    this->stream.open(filename, std::ios_base::binary);
    if(this->stream.fail()) {
        std::cerr << "Failed to open file\n";
        this->stream.close();
    }
}

TorrentFile::~TorrentFile() {
    this->stream.close();
}
