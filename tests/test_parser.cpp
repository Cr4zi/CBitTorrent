#include "catch.hpp"
#include <string_view>

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
