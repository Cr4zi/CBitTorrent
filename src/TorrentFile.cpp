#include "TorrentFile.h"

TorrentFile::TorrentFile(const std::string& filename) {
    // std::ios_base::binary open file in binary mode
    std::ifstream stream(filename, std::ios_base::binary);
    if(stream.fail()) {
        std::cerr << "Failed to open file\n";
        stream.close();
        return;
    }
    this->dict = decodeFile(stream);
    stream.close();
}

TorrentFile::~TorrentFile() {
}
