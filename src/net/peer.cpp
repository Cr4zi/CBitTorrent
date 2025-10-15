#include "peer.h"
#include <memory>
#include <sstream>

void Peer::send(std::ostringstream& oss) {
    std::string msg = oss.str();
    this->socket.sendBytes(msg);
}

void Peer::handshake(std::string info_hash, std::string my_peer_id) {
    std::string pstr = "BitTorrent protocol";
    std::ostringstream oss(std::ios::binary);
    
    uint8_t pstlen = static_cast<uint8_t>(pstr.size());
    oss.write(reinterpret_cast<const char *>(&pstlen), 1);

    oss.write(pstr.data(), pstr.size());

    char reserved[8] = {0};
    oss.write(reserved, 8);

    oss.write(info_hash.data(), info_hash.size());

    oss.write(my_peer_id.data(), my_peer_id.size());

    this->send(oss);
}

void Peer::keep_alive() {
	char keepalive[4] = {0};
	std::ostringstream oss(std::ios::binary);
	oss.write(keepalive, 4);

	this->send(oss);
}

ssize_t Peer::sendMsg(MessageType type, const std::vector<char>& payload) {
	if(this->choked) {
		return -1;
	}
	uint32_t length = htonl(payload.size() + 1);
	std::ostringstream oss(std::ios::binary);
	oss.write(reinterpret_cast<const char *>(&length), 4);
	oss.write(reinterpret_cast<const char *>(&type), 1);
	if(!payload.empty())
		oss.write(payload.data(), payload.size());

	this->send(oss);
	return 0;
}

std::shared_ptr<Message> Peer::read() {
	std::shared_ptr<Message> msg = std::make_shared<Message>(); 

	ssize_t bytes_read = 0;
	std::vector<char> data = this->socket.readBytes(bytes_read);
	if(bytes_read == 0) {
		std::cerr << "Failed to read bytes from peer" << std::endl;
		return nullptr;
	}
	for(int i = 0; i < 4; i++)
		msg->len = msg->len << 8 | static_cast<uint32_t>(data[i]);

	if(msg->len == 0)
		return msg;

	data.erase(data.begin(), data.begin() + 4);

	msg->id = MessageType(static_cast<uint32_t>(data[0]));
	data.erase(data.begin());

	if(!data.empty())
		msg->payload = data;

	return msg;
}
