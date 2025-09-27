#include "catch2/catch.hpp"
#include "my_lib/ex_func.h"

TEST_CASE("My Simple test for iterable sum", "[sum_until]")
{
    REQUIRE(sum_until(0) == 0);
    REQUIRE(sum_until(2) == 3);
    REQUIRE(sum_until(6) == 21);
}

TEST_CASE("My Simple test for recursive sum", "[sum_until_rec]")
{
    REQUIRE(sum_until_rec(0) == 0);
    REQUIRE(sum_until_rec(-45) == 0);
    REQUIRE(sum_until_rec(2) == 3);
    REQUIRE(sum_until_rec(6) == 21);
}