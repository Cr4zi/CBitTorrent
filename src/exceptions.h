#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

#include <exception>
#include <stdexcept>
#include <string>

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

class SocketCreationException : public std::runtime_error {
public:
    SocketCreationException(const std::string& msg) :
        std::runtime_error(msg) {}
};

#endif // EXCEPTIONS_H_
