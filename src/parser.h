#ifndef PARSER_H_
#define PARSER_H_
#include <cstdlib>
#include <string_view>
#include <cmath>

int parse_int(int& index, std::string_view& file_content);
std::string_view parse_string(int& index, std::string_view& file_content);
int parse_file(std::string_view file_content);

#endif // PARSER_H_
