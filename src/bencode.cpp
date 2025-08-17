#include "bencode.h"
#include <string>
#include <variant>


BencodeDict decodeFile(std::ifstream& input) {
    switch(input.peek()) {
        case 'd':
            return decodeDict(input);
    }
    throw DecodeException("Invalid torrent file.");
}


BencodeElement decodeInt(std::ifstream& input) {
    bool negative = false;
    int64_t num {};
    consumeExpectedCharacter(input, 'i');

    if(input.peek() == '-') {
        negative = true;
        input.get(); // consume '-'
    }

    char ch {};
    // I use .peek() so that letter won't be consumed
    while(input.peek() != 'e') {
        ch = input.get();

        if(!isdigit(ch)) {
            throw DecodeException("Invalid character inside an integer.");
        }

        num = (num * 10) + (ch - '0');
    }

    consumeExpectedCharacter(input, 'e');
    return negative ? -num : num;
}

BencodeElement decodeString(std::ifstream& input) {
    std::string result {};
    std::string::size_type length {};

    char ch {};
    while(input.peek() != ':') {
        ch = input.get();
        if(!isdigit(ch)) {
            throw DecodeException(std::string(std::string("Invalid character in a string length.") + ch).c_str());
        }

        length = (length * 10) + (ch - '0');
    }

    consumeExpectedCharacter(input, ':');

    for(size_t i = 0; i < length; i++) {
        result += input.get();
    }

    return result;
}

BencodeList decodeList(std::ifstream& input) {
    std::vector<BencodeElement> list;
    consumeExpectedCharacter(input, 'l');

    while(input.peek() != 'e') {
        switch (input.peek()) {
            case 'd':
                list.push_back(decodeDict(input));
                break;
            case 'i':
                list.push_back(decodeInt(input));
                break;
            case 'l':
                list.push_back(decodeList(input));
                break;
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                list.push_back(decodeString(input));
                break;
            default:
                throw DecodeException("Invalid torrent file.");
        }
    }

    consumeExpectedCharacter(input, 'e');

    return BencodeList{list};
}

BencodeDict decodeDict(std::ifstream& input) {
    std::map<std::string, BencodeElement> map {};

    consumeExpectedCharacter(input, 'd');

    while(input.peek() != 'e') {

        try {
            std::string str = std::get<std::string>(decodeString(input));

            switch(input.peek()) {
                case 'd':
                    map.insert({str, decodeDict(input)});
                    break;
                case 'i':
                    map.insert({str, decodeInt(input)});
                    break;
                case 'l':
                    map.insert({str, decodeList(input)});
                    break;
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                    map.insert({str, decodeString(input)});
                    break;
                default:
                    throw DecodeException("Invalid torrent file.");
            }
        } catch (const std::bad_variant_access& ex) {
            std::cout << ex.what() << '\n';
            throw DecodeException("Didn't parse string correctly.");
        }

    }

    consumeExpectedCharacter(input, 'e');
    return BencodeDict {map};
}

void consumeExpectedCharacter(std::ifstream& input, char expected) {
    char ch = input.get();
    if(ch != expected) {
        throw DecodeException(std::string(std::string("Expected character ") + expected).c_str());
    }
}

std::string encode(BencodeElement elem) {
    std::string result{};


    if(std::holds_alternative<int64_t>(elem)) {
        int64_t val = std::get<int64_t>(elem);
        result = encodeInt(val);
    } else if(std::holds_alternative<std::string>(elem)) {
        std::string val = std::get<std::string>(elem);
        result = encodeString(val);
    } else if(std::holds_alternative<BencodeList>(elem)) {
        std::vector<BencodeElement> arr = std::get<BencodeList>(elem).list;
        result = encodeList(arr);
    } else if(std::holds_alternative<BencodeDict>(elem)) {
        std::map<std::string, BencodeElement> map = std::get<BencodeDict>(elem).map;
        result = encodeDict(map);
    } else {
        throw EncodeException("Failed to encode Bencode Element");
    }

    return result;
}

std::string encodeInt(int64_t value) {
    return "i" + std::to_string(value) + "e";
}

std::string encodeString(std::string str) {
    return std::to_string(str.length()) + ":" + str;
}

std::string encodeList(std::vector<BencodeElement> arr) {
    std::string result = "l";
    for(BencodeElement& elem: arr) {
        result.append(encode(elem));
    }
    result.append("e");
    return result;
}


std::string encodeDict(std::map<std::string, BencodeElement> map) {
    std::string result = "d";
    for(std::map<std::string, BencodeElement>::iterator it = map.begin(); it != map.end(); it++) {
        result.append(encodeString(it->first));
        result.append(encode(it->second));
    }
    result.append("e");

    return result;
}
