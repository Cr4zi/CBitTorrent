#ifndef PARSER_H_
#define PARSER_H_
#include <iostream>
#include <cstdlib>
#include <string_view>
#include <variant>
#include <vector>
#include <cmath>
#include <memory>

struct BencodeElement;
using BencodeElementPtr = std::shared_ptr<BencodeElement>;

typedef struct BencodeElement {
    using BencodeType = std::variant<int, std::string_view, std::vector<BencodeElementPtr>>;
    BencodeType value;

    BencodeElement(int val) : value(val) {};
    BencodeElement(std::string_view str) : value(std::move(str)) {};
    BencodeElement(std::vector<BencodeElementPtr> vec) : value(std::move(vec)) {};

    bool operator==(const BencodeElement& other) {
        return value == other.value;
    }
} BencodeElement;

typedef struct BencodeValue {
    void operator()(int value) { std::cout << value << std::endl; };
    void operator()(std::string_view str) { std::cout << str << std::endl; };
    // void operator()(std::vector<BencodeElementPtr> vec) { std::cout };
} BencodeValue;

bool is_int(int index, std::string_view& file_content);
bool is_string(int index, std::string_view& file_content);
bool is_list(int index, std::string_view& file_content);
BencodeElementPtr parse_int(int& index, std::string_view& file_content);
BencodeElementPtr parse_string(int& index, std::string_view& file_content);
BencodeElementPtr parse_list(int& index, std::string_view& file_content);
int parse_file(std::string_view file_content);

#endif // PARSER_H_
