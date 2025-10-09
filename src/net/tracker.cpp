#include "tracker.h"
#include <cstdint>
#include <memory>
#include <optional>
#include <sstream>
#include <variant>

void Tracker::parse_port(std::string portStr) {
    uint16_t port;
    auto [ptr, ec] = std::from_chars(portStr.data(), portStr.data() + portStr.size(), port);
    if(ec == std::errc())
        this->port = port;
    else if(ec == std::errc::invalid_argument)
        throw InvalidTrackerException("Port is not a number");
    else if(ec == std::errc::result_out_of_range)
        throw InvalidTrackerException("Port is not inside unsigned 16 bit int range");
}

void Tracker::parse_url() {
    /* removing the start */
	std::string start = "http://";
	if(this->tracker_url.find(start, 0) != 0) {

        throw InvalidTrackerException("Doesn't start with http://");
	}
	this->tracker_url.erase(0, start.length());

    size_t colon = this->tracker_url.find(':');

    // invalid tracker
    if(std::string::npos == colon)
        throw InvalidTrackerException("No port...");

    this->host = this->tracker_url.substr(0, colon);
    std::string portStr = this->tracker_url.substr(colon + 1); // it is actually port/announce so now we remove announce
    size_t slash = portStr.find('/');

    if(slash != std::string::npos)
        portStr = portStr.substr(0, slash);

    this->parse_port(portStr);

}
 
int Tracker::connect() {
    return this->connectSocket(this->host.c_str(), this->port);
}

template <typename T>
std::optional<T> Tracker::get_from_map(BencodeDict dict, std::string key) {
    auto find = dict.map.find(key);
    if(find == dict.map.end()) {
        std::cerr << "Couldn't find " << key << std::endl;
        return std::nullopt;
    }

    T element;
    try {
        element = std::get<T>(find->second);
    } catch(const std::bad_variant_access& e) {
        std::cout << key << ' ' << e.what() << std::endl;
        return std::nullopt;
    }

    return element;
}

ssize_t Tracker::parse_peers_list(BencodeList peers_list) {
	std::vector<BencodeElement> list = peers_list.list;
	ssize_t count = 0;
	try {
		for(auto ele : list) {
			BencodeDict dict = std::get<BencodeDict>(ele);
			std::string peer_id = this->get_from_map<std::string>(dict, "peer id").value();
			std::string ip = this->get_from_map<std::string>(dict, "ip").value();
			int64_t port_int64 = this->get_from_map<int64_t>(dict, "port").value();

			if(port_int64 < 0 || port_int64 > UINT16_MAX) {
				std::cerr << "Invalid port" << std::endl;
				return -1;
			}
			uint16_t port = static_cast<uint16_t>(port_int64);
			this->peers.emplace_back(std::make_shared<Peer>(peer_id, ip, port));
			count++;
		}
	} catch(const std::bad_variant_access& e) {
		std::cerr << "Elements should be dictionary<string, int/string>." << '\n' << e.what() << std::endl; 
		return -1;
	} catch(const std::bad_optional_access& e) {
		std::cerr << "Elements should be dictionary<string, int/string>." << '\n' << e.what() << std::endl; 
		return -1;
	}

	return count;
}

ssize_t Tracker::parse_peers_binary(std::string& peers_binary) { // inshalla this will work
	ssize_t peers = 0;
	for(size_t i = 0; i < peers_binary.size(); i+=6) {
		std::ostringstream ip_stream;
		for(int j = 0; j < 3; j++)
			ip_stream << static_cast<unsigned>(static_cast<unsigned char>(peers_binary[i+j])) << '.';
		ip_stream << static_cast<unsigned>(static_cast<unsigned char>(peers_binary[i+3]));

		uint16_t port = (static_cast<uint16_t>(peers_binary[i+4])) << 8 | (static_cast<uint16_t>(peers_binary[i+5]));
		this->peers.emplace_back(std::make_shared<Peer>(ip_stream.str(), port));
		peers++;
	}
	return peers;
}

ssize_t Tracker::generate_peers(std::vector<char>& response) {
	const std::string delm = "\r\n\r\n";

	auto it = std::search(response.begin(), response.end(), delm.begin(), delm.end());
	if(it == response.end()) {
		std::cerr << "Invalid http response" << std::endl;
		return -1;
	}

	size_t headers_end = std::distance(response.begin(), it) + delm.size();

	std::string body(response.begin() + headers_end, response.end());
	std::istringstream stream(body, std::ios_base::binary);
	BencodeDict dict;
	try {
		dict = decodeDict(stream);
	} catch(const DecodeException& e) {
		std::cout << "Invalid bencode" << std::endl;
		return -1;
	}

	std::optional<std::string> failure = this->get_from_map<std::string>(dict, "failure reason");
	if(failure != std::nullopt) {
		std::cout << failure.value_or("what the sigma") << std::endl;
		return -1;
	}

	try {
		int64_t interval = this->get_from_map<int64_t>(dict, "interval").value();
		if(interval <= 0) {
			std::cerr << "invalid interval" << std::endl;
			return -1;
		}
		this->interval = interval;
	} catch(const std::bad_optional_access& e) {
		std::cout << e.what() << std::endl;
		return -1;
	}

	try {
		BencodeList peers_dict = this->get_from_map<BencodeList>(dict, "peers").value();
		return this->parse_peers_list(peers_dict);
	} catch(const std::bad_optional_access& e) {
		std::cout << e.what() << std::endl; // we don't return because there could be peers as binary models
	}

	try {
		std::string peers_binary = this->get_from_map<std::string>(dict, "peers").value();
		return this->parse_peers_binary(peers_binary);
	} catch(const std::bad_optional_access& e) {
		std::cout << "no peers ?? " << e.what() << std::endl;
	}

	return -1;
}
