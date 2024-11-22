#ifndef ENCODER_H
#define ENCODER_H
#include "BencodeElement.h"

#include <string>
#include <string_view>
#include <variant>
#include <unordered_map>

std::string encode_int(int value);
std::string encode_string(std::string_view str);
std::string encode_list(std::vector<BencodeElementPtr> vec);
std::string encode_dictionary(std::unordered_map<std::variant<int, std::string_view>, BencodeElementPtr> map);
std::string encoder(BencodeElementPtr element);

#endif
