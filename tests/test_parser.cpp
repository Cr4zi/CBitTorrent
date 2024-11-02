#include "catch.hpp"
#include <memory>
#include <string_view>
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

TEST_CASE( "List Parser Tests", "[List Parser]") {
    int index = 0;

    std::string_view s1 = "l4:spami42ee";
    std::vector<BencodeElementPtr> vec;
    vec.push_back(std::make_shared<BencodeElement>("spam"));
    vec.push_back(std::make_shared<BencodeElement>(42));

    REQUIRE( vec == std::get<std::vector<BencodeElementPtr>>(parse_list(index, s1)->value) );

    /*
    std::string_view s2 = "lsaxi42ee";
    index = 0;
    REQUIRE( parse_list(index, s2) == nullptr );
    */

}
