#include "car_park/car_park.hpp"
#include "catch2/catch.hpp"

using namespace car_park;

TEST_CASE( "add_one", "[car_park]" ){
  REQUIRE(add_one(0) == 1);
  REQUIRE(add_one(123) == 124);
  REQUIRE(add_one(-1) == 0);
}
