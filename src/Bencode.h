#ifndef BENCODE_H_
#define BENCODE_H_
#include "Exceptions.h"

#include <string>
#include <variant>
#include <cstdint>
#include <map>
#include <vector>
#include <fstream>
#include <iostream>
#include <cctype>


struct BencodeDict;
struct BencodeList;
using BencodeElement = std::variant<int64_t, std::string, BencodeDict, BencodeList>;

struct BencodeList {
    std::vector<BencodeElement> list;
};

struct BencodeDict {
    std::map<std::string, BencodeElement> map;
};

BencodeDict decodeFile(std::ifstream& input);

BencodeElement decodeInt(std::ifstream& input);
BencodeElement decodeString(std::ifstream& input);
BencodeList decodeList(std::ifstream& input);
BencodeDict decodeDict(std::ifstream& input);

void consumeExpectedCharacter(std::ifstream& input, char expected);

#endif // BENCODE_H_
