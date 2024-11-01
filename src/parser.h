#ifndef PARSER_H_
#define PARSER_H_
#include <cstdlib>
#include <string_view>
#include <variant>
#include <vector>
#include <cmath>

bool is_int(int index, std::string_view& file_content);
bool is_string(int index, std::string_view& file_content);
bool is_list(int index, std::string_view& file_content);
int parse_int(int& index, std::string_view& file_content);
std::string_view parse_string(int& index, std::string_view& file_content);
int parse_file(std::string_view file_content);

#endif // PARSER_H_
