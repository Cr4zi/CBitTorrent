#ifndef BENCODE_H_
#define BENCODE_H_
#include "../exceptions.h"

#include <string>
#include <variant>
#include <cstdint>
#include <map>
#include <vector>
#include <fstream>
#include <iostream>
#include <cctype>
#include <sstream>
#include <iomanip>


struct BencodeDict;
struct BencodeList;
using BencodeElement = std::variant<int64_t, std::string, BencodeDict, BencodeList>;

struct BencodeList {
    std::vector<BencodeElement> list;
};

struct BencodeDict {
    std::map<std::string, BencodeElement> map;
};

BencodeDict decodeFile(std::istream& input);

BencodeElement decodeInt(std::istream& input);
BencodeElement decodeString(std::istream& input);
BencodeList decodeList(std::istream& input);
BencodeDict decodeDict(std::istream& input);

void consumeExpectedCharacter(std::istream& input, char expected);

std::string encode(BencodeElement elem);
std::string encodeInt(int64_t value);
std::string encodeString(std::string str);
std::string encodeList(std::vector<BencodeElement> arr);
std::string encodeDict(std::map<std::string, BencodeElement> map);

#endif // BENCODE_H_
