#include "catch.hpp"
#include <memory>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <iostream>

#include "../src/parser.h"

TEST_CASE( "Type tests", "[Type Testing]") {
    int index = 2;
    std::string_view s1 = "x";
    REQUIRE( is_int(index, s1) == false );

    index = 1;
    std::string_view s2 = "z2:he";
    REQUIRE( is_string(index, s2 ) == true);

    index = 0;
    std::string_view s3 = "i123e";
    REQUIRE( is_int(index, s3) == true );

    index = 2;
    std::string_view s4 = "asl4:spami42ee";
    REQUIRE( is_list(index, s4) == true );

    index = 1;
    std::string_view s5 = "i42e";
    REQUIRE( is_int(index, s5) == false );

    index = 0;
    std::string_view s6 = "d2:hie";
    REQUIRE( is_dictionary(index, s6) == true );

    index = 2;
    REQUIRE( is_dictionary(index, s6) == false );

}

TEST_CASE( "Int Parser Tests", "[Int Parser]") {
    int index = 0;
    std::string_view s1 = "i42e";
    std::string_view s2 = "i-21e";

    REQUIRE( std::get<int>(parse_int(index, s1)->value) == 42 );
    index = 0;
    REQUIRE( std::get<int>(parse_int(index, s2)->value) == -21 );
    index = 2;
    std::string_view s3 = "avi123e";
    REQUIRE( std::get<int>(parse_int(index, s3)->value) == 123 );
}

TEST_CASE( "String Parser Tests", "[String Praser]") {
    int index = 2;
    std::string_view s1 = "xz3:helzx";
    REQUIRE( std::get<std::string_view>(parse_string(index, s1)->value) == "hel" );

    index = 0;
    std::string_view s2 = "19:AAAAAAAAAAAAAAAAAAA";
    REQUIRE( std::get<std::string_view>(parse_string(index, s2)->value) == "AAAAAAAAAAAAAAAAAAA" );
}

bool compare_vectors(std::vector<BencodeElementPtr> vec1, std::vector<BencodeElementPtr> vec2)
{
    if(vec1.size() != vec2.size()) return false;

    return std::equal(vec1.begin(), vec1.end(), vec2.begin(),
                      [](const BencodeElementPtr& a, const BencodeElementPtr& b) {
                          return *a == *b;
                      });
}

TEST_CASE( "List Parser Tests", "[List Parser]") {
    int index = 0;

    std::string_view s1 = "l4:spami42ee";
    std::vector<BencodeElementPtr> vec;
    vec.push_back(std::make_shared<BencodeElement>("spam"));
    vec.push_back(std::make_shared<BencodeElement>(42));
    REQUIRE( compare_vectors(vec, std::get<std::vector<BencodeElementPtr>>(parse_list(index, s1)->value)) );

    std::string_view s2 = "lsaxi42ee";
    index = 0;
    REQUIRE( parse_list(index, s2) == nullptr );

    vec.push_back(std::make_shared<BencodeElement>(21));
    index = 0;
    REQUIRE( !compare_vectors(vec, std::get<std::vector<BencodeElementPtr>>(parse_list(index, s1)->value)) );

}

bool compare_maps(std::unordered_map<std::variant<int, std::string_view>, BencodeElementPtr> map1, std::unordered_map<std::variant<int, std::string_view>, BencodeElementPtr> map2)
{
    if(map1.size() != map2.size()) return false;

    for(const auto& pair : map1) {
        auto val = map2.find(pair.first);

        // could not find a value with key as map1
        if(val == map2.end())
            return false;

        std::vector<BencodeElementPtr>* vec1 = std::get_if<std::vector<BencodeElementPtr>>(&(pair.second->value));
        std::vector<BencodeElementPtr>* vec2 = std::get_if<std::vector<BencodeElementPtr>>(&(val->second->value));


        // if both are vectors and aren't equal
        if(vec1 && vec2)
            if(!compare_vectors(*vec1, *vec2))
                return false;

        // if one of them is a vector and the other one isn't
        if((vec1 && !vec2) || (!vec1 && vec2))
            return false;

        if(pair.second->value != val->second->value)
            return false;
    }

    return true;
}

TEST_CASE( "Dictionary Parser Test", "[Dictionary Parser]" )
{
    int index = 0;
    std::string_view s1 = "d3:bar4:spam3:fooi42ee";
    std::unordered_map<std::variant<int, std::string_view>, BencodeElementPtr> map2 = {
        {"bar", std::make_shared<BencodeElement>("spam")},
        {"foo", std::make_shared<BencodeElement>(42)},
    };

    std::unordered_map<std::variant<int, std::string_view>, BencodeElementPtr> map1 = std::get<std::unordered_map<std::variant<int, std::string_view>, BencodeElementPtr>>(parse_dictionary(index, s1)->value);
    REQUIRE( map1.size() == 2 );

    REQUIRE( compare_maps(map1, map2) );

}
