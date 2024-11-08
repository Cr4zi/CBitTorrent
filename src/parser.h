#ifndef PARSER_H_
#define PARSER_H_
#include <iostream>
#include <cstdlib>
#include <string_view>
#include <variant>
#include <vector>
#include <cmath>
#include <memory>
#include <unordered_map>

struct BencodeElement;
using BencodeElementPtr = std::shared_ptr<BencodeElement>;

typedef struct BencodeElement {
    using BencodeType = std::variant<int, std::string_view, std::vector<BencodeElementPtr>, std::unordered_map<std::variant<int, std::string_view>, BencodeElementPtr>>;
    BencodeType value;

    BencodeElement(int val) : value(val) {};
    BencodeElement(std::string_view str) : value(std::move(str)) {};
    BencodeElement(std::vector<BencodeElementPtr> vec) : value(std::move(vec)) {};
    BencodeElement(std::unordered_map<std::variant<int, std::string_view>, BencodeElementPtr> map) : value(std::move(map)) {};

    bool operator==(const BencodeElement& other) {
        return value == other.value;
    }
} BencodeElement;

bool is_int(int index, std::string_view& file_content);
bool is_string(int index, std::string_view& file_content);
bool is_list(int index, std::string_view& file_content);
bool is_dictionary(int index, std::string_view& file_content);
BencodeElementPtr parse_int(int& index, std::string_view& file_content);
BencodeElementPtr parse_string(int& index, std::string_view& file_content);
BencodeElementPtr parse_list(int& index, std::string_view& file_content);
BencodeElementPtr parse_dictionary(int& index, std::string_view& file_content);
int parse_file(std::string_view file_content);

#endif // PARSER_H_
