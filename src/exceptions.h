#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

#include <exception>
#include <stdexcept>
#include <string>
#include <errno.h>

class DecodeException : public std::runtime_error {
public:
    DecodeException(const std::string& msg) :
        std::runtime_error(msg) {}
};

class EncodeException: public std::runtime_error {
public:
    EncodeException(const std::string& msg) :
        std::runtime_error(msg) {}
};

class InfoKeyNotFound: public std::runtime_error {
public:
    InfoKeyNotFound(const std::string& msg) :
        std::runtime_error(msg) {}
};

class InvalidTorrentFile : public std::runtime_error {
public:
    InvalidTorrentFile(const std::string& msg) :
        std::runtime_error(msg) {}
};

class SocketCreationException : public std::runtime_error {
public:
    SocketCreationException(const std::string& msg) :
        std::runtime_error(msg) {}
};

class SocketBindException : public std::runtime_error {
public:
    SocketBindException(const std::string& msg) :
        std::runtime_error(msg) {}
};

class SocketListenException : public std::runtime_error {
public:
    SocketListenException(const std::string& msg) :
        std::runtime_error(msg) {}
};

class SocketConnectException : public std::runtime_error {
public:
    SocketConnectException(const std::string& msg) :
        std::runtime_error(msg) {}
};

class SendingBytesFailedException : public std::runtime_error {
public:
    SendingBytesFailedException(const std::string& msg) :
        std::runtime_error(msg) {}
};

class ReadingBytesFailedException : public std::runtime_error {
public:
    ReadingBytesFailedException(const std::string& msg) :
        std::runtime_error(msg) {}
};

class EpollException : public std::runtime_error {
public:
    EpollException(const std::string& msg) :
        std::runtime_error(msg) {}
};

#endif // EXCEPTIONS_H_
