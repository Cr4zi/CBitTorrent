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

#include "BencodeElement.h"

bool is_int(int index, std::string_view& file_content);
bool is_string(int index, std::string_view& file_content);
bool is_list(int index, std::string_view& file_content);
bool is_dictionary(int index, std::string_view& file_content);
BencodeElementPtr parse_int(int& index, std::string_view& file_content);
BencodeElementPtr parse_string(int& index, std::string_view& file_content);
BencodeElementPtr parse_list(int& index, std::string_view& file_content);
BencodeElementPtr parse_dictionary(int& index, std::string_view& file_content);
BencodeElementPtr parse_file(std::string_view file_content);

#endif // PARSER_H_
