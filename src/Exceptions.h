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


#endif // EXCEPTIONS_H_
