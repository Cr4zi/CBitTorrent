#include "catch.hpp"
#include <string_view>

#include "../src/parser.h"


TEST_CASE( "Int Parser Tests", "[Int Parser]") {
    int index = 0;
    std::string_view s1 = "i42e";
    std::string_view s2 = "i-21e";

    REQUIRE( parse_int(index, s1) == 42 );
    index = 0;
    REQUIRE( parse_int(index, s2) == -21 );
    index = 2;
    std::string_view s3 = "avi123e";
    REQUIRE( parse_int(index, s3) == 123 );
}

TEST_CASE( "String Parser Tests", "[String Praser]") {
    int index = 2;
    std::string_view s1 = "xz3:helzx";
    REQUIRE( parse_string(index, s1) == "hel" );

    index = 0;
    std::string_view s2 = "19:AAAAAAAAAAAAAAAAAAA";
    REQUIRE( parse_string(index, s2) == "AAAAAAAAAAAAAAAAAAA" );
}
