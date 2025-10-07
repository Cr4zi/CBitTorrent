#include "tracker.h"

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
