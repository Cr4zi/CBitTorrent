#ifndef BENCODEELEMENT_H_
#define BENCODEELEMENT_H_

#include <cstdlib>
#include <string_view>
#include <variant>
#include <vector>
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



#endif // BENCODEELEMENT_H_
