#include "torrent_file.h"
#include "bencode.h"
#include "exceptions.h"
#include <stdexcept>
#include <variant>

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

    return std::string(reinterpret_cast<char *>(obuf), SHA_DIGEST_LENGTH);
}

// safest function to ever exist (trust)
std::vector<std::string> TorrentFile::getTrackers() {
    try {
        auto it = this->dict.map.find("announce-list");
        std::vector<std::string> vec;
        std::string announce = std::get<std::string>(this->dict.map["announce"]);
		vec.push_back(announce);


        if(it != this->dict.map.end()) {
            std::vector<BencodeElement> list = std::get<BencodeList>(it->second).list;
            for(BencodeElement ele : list) {
                std::string value = std::get<std::string>(std::get<BencodeList>(ele).list[0]);

				vec.push_back(value);
            }
        }

        return vec;
    } catch(const std::bad_variant_access &e) {
        throw InvalidTorrentFile(e.what());
    }
}
