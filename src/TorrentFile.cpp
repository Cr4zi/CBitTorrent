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
    if (auto search = this->dict.map.find("announce"); search != this->dict.map.end())
        std::cout << "Found " << search->first << " " << std::get<std::string>(search->second) << '\n';
    else
        std::cout << "Not found\n";

    stream.close();
}

TorrentFile::~TorrentFile() {
}
