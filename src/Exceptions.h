#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

#include <exception>
#include <string>

class DecodeException : public std::exception {
private:
    std::string message;
public:
    DecodeException(const char *msg) :
    message(msg) {}

    const char* what() const noexcept {
        return this->message.c_str();
    }
};


class EncodeException: public std::exception {
private:
    std::string message;
public:
    EncodeException(const char *msg) :
    message(msg) {}

    const char* what() const noexcept {
        return this->message.c_str();
    }
};

class InfoKeyNotFound: public std::exception {
private:
    std::string message;
public:
    InfoKeyNotFound(const char *msg) :
    message(msg) {}

    const char* what() const noexcept {
        return this->message.c_str();
    }
};

#endif // EXCEPTIONS_H_
