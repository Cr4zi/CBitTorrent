#include "catch.hpp"

#include "../src/encoder.h"
#include "../src/parser.h"

TEST_CASE( "Int Encoding Tests", "[Int Encoding]" ) {
    REQUIRE( encode_int(51) == "i51e" );

    REQUIRE( encode_int(-21) == "i-21e" );
}

TEST_CASE( "String Encoding Tests", "[String Encoding]" ) {
    REQUIRE( encode_string("foo") == "3:foo" );
    // I don't know what to test lmao
}

TEST_CASE( "List Encoding Tests", "[List Encoding]" ) {
    std::vector<BencodeElementPtr> vec;
    vec.push_back(std::make_shared<BencodeElement>("spam"));
    vec.push_back(std::make_shared<BencodeElement>(42));

    REQUIRE( encode_list(vec) == "l4:spami42ee" );

}

TEST_CASE( "Dictionary Encoding Tests", "[Dictionary Encoding]" ) {
    std::unordered_map<std::variant<int, std::string_view>, BencodeElementPtr> map = {
        {"foo", std::make_shared<BencodeElement>(42)},
        {"bar", std::make_shared<BencodeElement>("spam")},
    };

    REQUIRE( encode_dictionary(map) == "d3:bar4:spam3:fooi42ee" );

    std::vector<BencodeElementPtr> vec;
    vec.push_back(std::make_shared<BencodeElement>("a"));
    vec.push_back(std::make_shared<BencodeElement>("b"));
    std::unordered_map<std::variant<int, std::string_view>, BencodeElementPtr> map2 = {
        {"spam", std::make_shared<BencodeElement>(vec)},
    };

    REQUIRE( encode_dictionary(map2) == "d4:spaml1:a1:bee" );
}
