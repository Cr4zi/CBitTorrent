#include "torrent_file.h"

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

TorrentFile::~TorrentFile() {}

std::string TorrentFile::calculateInfoHash() {
    std::map<std::string, BencodeElement>::iterator info = this->dict.map.find("info");
    if(info == this->dict.map.end())
        throw InfoKeyNotFound("Info Key not Found");

    std::string infoStr = encode(info->second);
    unsigned char* ibuf = reinterpret_cast<unsigned char *>(infoStr.data());
    unsigned char obuf[SHA_DIGEST_LENGTH];
    SHA1(ibuf, infoStr.size(), obuf);

    std::ostringstream oss;
    for(int i = 0; i < SHA_DIGEST_LENGTH; i++) {
        // turning bytex for example 0x12 to 12 etc..., if 1 charcater add fill 0
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(obuf[i]);
    }

    return oss.str();
}
