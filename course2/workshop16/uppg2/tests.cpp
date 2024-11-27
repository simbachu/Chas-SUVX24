#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "ProductDB.h"

TEST_CASE("Dates"){
    REQUIRE_THROWS(Date::from_ints(9999, 99, 99));
    REQUIRE_THROWS(Date::from_ints(1,1,1));
    REQUIRE_THROWS(Date::from_ints(100000,12,11));
    REQUIRE_THROWS(Date::from_ints(1900, 2, 29));
    REQUIRE_THROWS(Date::from_ints(-400, 1, 1));
    REQUIRE_THROWS(Date::from_ints(2000, 13, 1));
    REQUIRE_THROWS(Date::from_ints(2000, 12, 32));
    REQUIRE_THROWS(Date::from_ints(2000, 12, -1));
    REQUIRE_NOTHROW(Date::from_ints(1970, 1, 1));
    REQUIRE_NOTHROW(Date::from_ints(2024, 11, 27));
    REQUIRE_NOTHROW(Date::from_ints(2000, 2, 29));
    REQUIRE_NOTHROW(Date::from_ints(2024, 2, 29));
 
    REQUIRE_THROWS(Date::from_string("9999-99-99"));
    REQUIRE_THROWS(Date::from_string("1900-02-29"));
    REQUIRE_THROWS(Date::from_string("2000-13-1"));
    REQUIRE_THROWS(Date::from_string("2000/12/32"));
    REQUIRE_THROWS(Date::from_string("2000 12 -1"));
    REQUIRE_NOTHROW(Date::from_string("1970-01-01"));
    REQUIRE_NOTHROW(Date::from_string("2024-11-27"));
    REQUIRE_NOTHROW(Date::from_string("2000-02-29"));
    REQUIRE_NOTHROW(Date::from_string("2024-02-29"));
}