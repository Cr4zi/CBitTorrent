#include "catch.hpp"
#include <string_view>

#include "../src/parser.h"


TEST_CASE( "Parser tests", "[parser]") {
    int index = 0;
    std::string_view s1 = "i42e";
    std::string_view s2 = "i-21e";

    REQUIRE( parse_int(index, s1) == 42 );
    REQUIRE( parse_int(index, s2) == -42 );
    index = 2;
    std::string_view s3 = "avi123e";
    REQUIRE( parse_int(index, s3) == 123 );
}
