#include "encoder.h"
#include "BencodeElement.h"
#include <iostream>
#include <string>
#include <variant>
#include <map>

using Map = std::map<std::variant<int, std::string_view>, BencodeElementPtr>;

std::string encode_int(int value)
{
    std::string encoded = "i";
    int inserted_minus = 0;
    if(value < 0) {
        encoded.insert(1, 1, '-');
        value *= -1;
        inserted_minus = 1;
    }

    while(value != 0) {
        encoded.insert(1 + inserted_minus, 1, value % 10 + '0');
        value /= 10;
    }

    encoded += 'e';
    return encoded;
}

std::string encode_string(std::string_view str)
{
    std::string encoded;

    // I don't like this
    try {
        encoded = std::to_string(str.length());
    } catch(std::bad_alloc const& e) {
        std::cerr << e.what() << std::endl;
        encoded = "";
        return encoded;
    }

    encoded += ':';
    encoded += str;

    return encoded;
}

std::string encode_list(std::vector<BencodeElementPtr> vec)
{
    std::string encoded = "l";

    for(size_t i = 0; i < vec.size(); i++) {
        const int* val = std::get_if<int>(&(vec[i]->value));
        if(val) {
            encoded += encode_int(*val);
            continue;
        }

        const std::string_view* str = std::get_if<std::string_view>(&(vec[i]->value));
        if(str) {
            // Won't really cause problems if encoded_string returned an empty string
            encoded += encode_string(*str);
            continue;
        }

        const std::vector<BencodeElementPtr>* v = std::get_if<std::vector<BencodeElementPtr>>(&(vec[i]->value));
        if(v) {
            encoded += encode_list(*v);
            continue;
        }

        const Map* map = std::get_if<Map>(&(vec[i]->value));
        if(map) {
            encoded += encode_dictionary(*map);
            continue;
        }

        encoded = "";
        return encoded;
    }

    encoded += 'e';
    return encoded;
}

std::string encode_dictionary(Map map)
{
    std::string encoded = "d";

    for(const auto& pair: map) {

        // Key
        const std::string_view* str = std::get_if<std::string_view>(&(pair.first));
        if(str) {
            std::string s = encode_string(*str);
            if(s == "") {
                encoded = "";
                return encoded;
            }

            encoded += s;
        } else {
            const int* val = std::get_if<int>(&(pair.first));
            if(!val) {
                encoded = "";
                return encoded;
            }

            encoded += encode_int(*val);
        }

        // Value
        const int* val = std::get_if<int>(&((pair.second)->value));
        if(val) {
            encoded += encode_int(*val);
            continue;
        }

        const std::string_view* s = std::get_if<std::string_view>(&((pair.second)->value));
        if(s) {
            std::string res = encode_string(*s);
            if(res == "") {
                encoded = "";
                return res;
            }

            encoded += res;
            continue;
        }

        const std::vector<BencodeElementPtr>* v = std::get_if<std::vector<BencodeElementPtr>>(&((pair.second)->value));
        if(v) {
            encoded += encode_list(*v);
            continue;
        }

        const Map* map = std::get_if<Map>(&((pair.second)->value));
        if(map) {
            encoded += encode_dictionary(*map);
            continue;
        }

        encoded = "";
        return encoded;
    }

    encoded += 'e';
    return encoded;
}
